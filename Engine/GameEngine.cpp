#include "GameEngine.h"

#include "Other/TimeManager.h"
#include "GarbageCollector.h"
#include "Render/RenderManager.h"
#include "Input/InputManager.h"
#include "Level/LevelManager.h"
#include "CollisionDetection/CollisionManager.h"
#include "Log/Logger.h"
#include "EngineFactory.h"
#include "Player/PlayerManager.h"
#include "Other/ProfilerManager.h"
#include "Network/NetworkManager.h"

#include "Core/RenderConsoleLibrary.h"
#include "Draw/DrawConsoleLibrary.h"

#include "Math/MathLibrary.h"

#include <shared_mutex>

/*
	Engine Architecture.

	Needs to define game levels.
	Each level will have its own set of objects.
	Each object can be drawn on the screen in the specific order.

	So we define the Render function that will be called in the end of the main loop inside the Run function.
	It'll look for the current active level in the game and call its Render function.
	Each level will have its own Render function that will call the Render function of each object in the level in the specific order.

	Before each global Render call, we need to clear the screen (consider console limitations with frame rate,
	so maybe take this into an account and not clear/update the screen if there is nothing new to draw to or remove from it).

	There should not be any blocking functions in the game loop.
	So input handling should be done in a separate thread.
*/

static std::shared_mutex EngineMainProcessMtx;
static std::mutex InputStateMutex;

static void _Cleanup()
{
	// Reset the cursor to the bottom left corner so the service info starts to be printed from there.
	DrawConsoleLibrary::SetCursorToBottomLeft();

	LevelManager::GetInstance().Release();
	NetworkManager::GetInstance().Cleanup();
	InputManager::GetInstance().Release();
	GarbageCollector::GetInstance().Free();
	EngineFactory::GetInstance().Free();
	Logger::GetInstance().Release();
}

static BOOL WINAPI ConsoleHandlerRoutine(DWORD dwCtrlType)
{
	std::unique_lock<std::shared_mutex> lock(EngineMainProcessMtx);

	switch (dwCtrlType)
	{
		case CTRL_C_EVENT:
		case CTRL_CLOSE_EVENT:
		case CTRL_BREAK_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT:
		{
			_Cleanup();
			return TRUE;
		}
	}

	return FALSE;
}

GameEngine::GameEngine(const GamePropertiesInfo& props)
	: GameProperties(props)
{
	LastWndDimension = { 0, 0 };

	InitializeRootObject();
}

GameEngine::~GameEngine()
{
	Cleanup();
}

void GameEngine::Initialize(GameLevel* StartupLevel)
{
	FillEngineProperties();

	TimeManager::GetInstance().Initialize();
	GarbageCollector::GetInstance().Initialize();

	Logger::GetInstance().Initialize("engine_log.log");

	#if _DEBUG
	MathLibrary::SeedRandom(100);
	#else // _DEBUG
	MathLibrary::SeedRandom();
	#endif // _DEBUG

	InputManager::GetInstance().Initialize();
	{
		InputManager::GetInstance().OnKeyPressEvent().Subscribe(this, &GameEngine::HandleKeyPressEvent);
		InputManager::GetInstance().OnKeyReleaseEvent().Subscribe(this, &GameEngine::HandleKeyReleaseEvent);
		InputManager::GetInstance().OnKeyToggleEvent().Subscribe(this, &GameEngine::HandleKeyToggleEvent);

		InputManager::GetInstance().OnMouseBtnPressEvent().Subscribe(this, &GameEngine::HandleMouseButtonPressEvent);
		InputManager::GetInstance().OnMouseBtnReleaseEvent().Subscribe(this, &GameEngine::HandleMouseButtonReleaseEvent);

		InputManager::GetInstance().OnMouseDoubleClickEvent().Subscribe(this, &GameEngine::HandleMouseDoubleClickEvent);

		InputManager::GetInstance().OnMouseMoveEvent().Subscribe(this, &GameEngine::HandleMouseMoveEvent);
		InputManager::GetInstance().OnMouseWheelEvent().Subscribe(this, &GameEngine::HandleMouseWheelEvent);

		InputManager::GetInstance().OnWindowResizeEvent().Subscribe(this, &GameEngine::HandleWindowResizeEvent);
	}

	RenderManager::GetInstance().Initialize();
	RenderConsoleLibrary::SetConsoleCaption(GameProperties.GameName.c_str());

	NetworkManager::GetInstance().Initialize();

	{
		LevelManager::GetInstance().OnLevelOpenEvent().Subscribe(this, &GameEngine::HandleLevelOpenEvent);
		LevelManager::GetInstance().OnLevelCloseEvent().Subscribe(this, &GameEngine::HandleLevelCloseEvent);

		LevelManager::GetInstance().OpenLevel(StartupLevel);
	}

	CollisionManager::GetInstance().Initialize();

	EngineFactory::GetInstance().RegisterType("PlayerState", new PlayerState());
	PlayerManager::GetInstance().Initialize();

	ProfilerManager::GetInstance().Initialize();
}

void GameEngine::Run()
{
	TimeManager::GetInstance().StartClock();

	bIsRunning = true;
	StartThreads();

	while (bIsRunning)
	{
		std::unique_lock<std::shared_mutex> lock(EngineMainProcessMtx);

		// Input state.
		RefreshInputState();

		// Networking.
		NetworkManager::GetInstance().Update();

		// Do the game logic here.
		{
			if (!LevelManager::GetInstance().IsGamePaused())
				CollisionManager::GetInstance().UpdateTracking();

			LevelManager::GetInstance().PassInput(LastInputState);
			LevelManager::GetInstance().Update();
		}

		RenderManager::GetInstance().Render();

		TimeManager::GetInstance().Update();
		ProfilerManager::GetInstance().Update();
	}

	bIsRunning = false; // In case we did exit the loop with break.

	StopThreads();
}

/*
	Initialization.
*/

void GameEngine::FillEngineProperties()
{
	LastWndDimension = RenderConsoleLibrary::GetConsoleDimensions();
}

/*
	Threads.
*/

void GameEngine::StartThreads()
{
	// Handle close app events.
	SetConsoleCtrlHandler(ConsoleHandlerRoutine, TRUE);

	InputEventsThreadHandle = std::thread(&GameEngine::InputEventsThread, this);
}

void GameEngine::StopThreads()
{
	InputEventsThreadHandle.join();
}

void GameEngine::InputEventsThread()
{
	while (bIsRunning)
	{
		InputManager::GetInstance().ReadInput();
	}
}

/*
	Input.
*/

void GameEngine::RefreshInputState()
{
	std::lock_guard<std::mutex> lock(InputStateMutex);

	LastInputState = ActualInputState;
	ActualInputState.ResetFrameState();
}

/*
	Events.
*/

// Input Events.
void GameEngine::HandleKeyPressEvent(WORD keyCode)
{
	std::lock_guard<std::mutex> lock(InputStateMutex);

	auto& keyQueue = ActualInputState.KeyPressedQueue;
	if (std::find(keyQueue.begin(), keyQueue.end(), keyCode) == keyQueue.end())
		keyQueue.push_back(keyCode);
}

void GameEngine::HandleKeyReleaseEvent(WORD keyCode)
{
	std::lock_guard<std::mutex> lock(InputStateMutex);

	std::erase(ActualInputState.KeyPressedQueue, keyCode);
	const bool bOnlyOneKeyPressed = ActualInputState.KeyPressedQueue.empty();

	// TODO: Make exceptions here, of there will be some combos.
	if (!bOnlyOneKeyPressed) return;

	ProfilerEngineFeature pickedFeature = ProfilerEngineFeature::CollisionDetection;

	switch (keyCode)
	{
		case VK_ESCAPE:
			StopEngine();
			break;
		case VK_F1:
			pickedFeature = ProfilerEngineFeature::CollisionDetection;
			break;
		case VK_F2:
			pickedFeature = ProfilerEngineFeature::RenderScene;
			break;
		case VK_F3:
			pickedFeature = ProfilerEngineFeature::RenderWidgets;
			break;
		case VK_F4:
			pickedFeature = ProfilerEngineFeature::UpdateScene;
			break;
		default:
			break;
	}

	if (keyCode >= VK_F1 && keyCode <= VK_F4)
	{
		ProfilerManager::GetInstance().EnableEngineFeature(pickedFeature,
			!ProfilerManager::GetInstance().IsEngineFeatureEnabled(pickedFeature));
	}
}

void GameEngine::HandleKeyToggleEvent(WORD keyCode)
{
	std::lock_guard<std::mutex> lock(InputStateMutex);

	auto& keyQueue = ActualInputState.KeyToggledQueue;
	auto it = std::find(keyQueue.begin(), keyQueue.end(), keyCode);
	if (it == keyQueue.end())
		keyQueue.push_back(keyCode);
	else
		keyQueue.erase(it);
}

void GameEngine::HandleMouseButtonPressEvent(const MOUSE_EVENT_RECORD& mer, uint64_t imbMask)
{
	using namespace std::chrono;

	if (imbMask & ENUM2BIT(InputMouseButton::Left))
		LastLMBPressTime = high_resolution_clock::now();
	if (imbMask & ENUM2BIT(InputMouseButton::Right))
		LastRMBPressTime = high_resolution_clock::now();
	if (imbMask & ENUM2BIT(InputMouseButton::Middle))
		LastMMBPressTime = high_resolution_clock::now();

	std::lock_guard<std::mutex> lock(InputStateMutex);

	ActualInputState.LastMousePressPos = { mer.dwMousePosition.X, mer.dwMousePosition.Y };
	ActualInputState.MouseBtnPressedMask = imbMask;
}

void GameEngine::HandleMouseButtonReleaseEvent(const MOUSE_EVENT_RECORD& mer, uint64_t imbReleaseMask)
{
	// TODO: Come up with some kind of engine input mode, where there's mode that let player to interact only with widgets,
	// or only with game level (ignoring widgets), or both.

	// So there should be some thing called input consumption.
	// If user chooses the mode where he can interact with widgets and objects on level itself, we should track, if some widgets
	// can really handle the input, or we should just let it pass through and let the level handle it.

	// Introduce Hit-Testable mode, that has options like: OnlySelf, SelfAndChildren, NoHitTest.

	using namespace std::chrono;

	steady_clock::time_point Now = high_resolution_clock::now();
	float DeltaBtnPressTime = 0.0f;

	const InputMouseButton imbReleaseCheck[] = { InputMouseButton::Left, InputMouseButton::Right, InputMouseButton::Middle };
	steady_clock::time_point btnPressTimeCheck[] = { LastLMBPressTime, LastRMBPressTime, LastMMBPressTime };

	InputMouseButton clickTestBtn = imbReleaseCheck[0];
	steady_clock::time_point recentTime = btnPressTimeCheck[0];

	for (int i = 1; i < SIZEOFARRAY(imbReleaseCheck); ++i)
	{
		if (imbReleaseMask & ENUM2BIT(imbReleaseCheck[i]))
		{
			if (btnPressTimeCheck[i] > recentTime)
			{
				recentTime = btnPressTimeCheck[i];
				clickTestBtn = imbReleaseCheck[i];
			}
		}
	}

	DeltaBtnPressTime = duration_cast<duration<float>>(Now - recentTime).count();
	const bool bIsPressedLongEnough = DeltaBtnPressTime >= 0.2f;

	std::lock_guard<std::mutex> lock(InputStateMutex);

	ActualInputState.MouseBtnPressedMask &= ~imbReleaseMask;
	if (!bIsPressedLongEnough)
	{
		ActualInputState.MouseBtnClicked = clickTestBtn;
	}
}

void GameEngine::HandleMouseDoubleClickEvent(const MOUSE_EVENT_RECORD& mer, uint64_t imbMask)
{
	std::lock_guard<std::mutex> lock(InputStateMutex);

	InputMouseButton dblClickTestBtn = InputMouseButton::None;
	if (imbMask & ENUM2BIT(InputMouseButton::Left))
		dblClickTestBtn = InputMouseButton::Left;
	if (imbMask & ENUM2BIT(InputMouseButton::Right))
		dblClickTestBtn = InputMouseButton::Right;
	if (imbMask & ENUM2BIT(InputMouseButton::Middle))
		dblClickTestBtn = InputMouseButton::Middle;

	ActualInputState.MouseBtnDoubleClicked = dblClickTestBtn;
}

void GameEngine::HandleMouseMoveEvent(const MOUSE_EVENT_RECORD& mer)
{
	std::lock_guard<std::mutex> lock(InputStateMutex);
 
	ActualInputState.bMouseMoved = true;
	ActualInputState.LastMousePos = { mer.dwMousePosition.X, mer.dwMousePosition.Y };
}

void GameEngine::HandleMouseWheelEvent(const MOUSE_EVENT_RECORD& mer)
{
}

void GameEngine::HandleWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD& wbsr)
{
	/*
		There is an issue when the console app window's size is being reduced.
		There appears the scroll bar, that hides prev drawing buffer.
	*/

	const RC_SIZE currentWndSize = LastWndDimension.load();
	if (wbsr.dwSize.X == currentWndSize.cx &&
		wbsr.dwSize.Y == currentWndSize.cy)
	{
		return;
	}

	LastWndDimension = { (RC_UINT)wbsr.dwSize.X, (RC_UINT)wbsr.dwSize.Y };

	std::unique_lock<std::shared_mutex> lock(EngineMainProcessMtx);

	RenderManager::GetInstance().ReconstructRenderBuffer();
	CollisionManager::GetInstance().Reconstruct();

	if (auto pLevel = LevelManager::GetInstance().GetCurrentLevel())
	{
		pLevel->ReconstructLevel();
	}
}

// Level Events.
void GameEngine::HandleLevelOpenEvent(GameLevel* instigator)
{
}

void GameEngine::HandleLevelCloseEvent(GameLevel* instigator)
{
	// If there is an event of closing a level, it means that we want to close the app.
	StopEngine();
}

/*
	Engine Stop and Cleanup.
*/

void GameEngine::StopEngine()
{
	bIsRunning = false;
}

void GameEngine::Cleanup()
{
	_Cleanup();

	DestroyRootObject();
}
