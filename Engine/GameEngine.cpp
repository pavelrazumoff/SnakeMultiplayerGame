#include "GameEngine.h"

#include "GarbageCollector.h"
#include "Render/RenderManager.h"
#include "Input/InputManager.h"

#include "Core/RenderConsoleLibrary.h"

#include <chrono>
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
			GarbageCollector::GetInstance().Free();
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

void GameEngine::Initialization(GameLevel* StartupLevel)
{
	FillEngineProperties();

	InputManager::GetInstance()->Initialize();
	{
		InputManager::GetInstance()->RegisterInputCallback("KeyPressEvent", this, &GameEngine::HandleKeyPressEvent);
		InputManager::GetInstance()->RegisterInputCallback("WindowResizeEvent", this, &GameEngine::HandleWindowResizeEvent);
	}

	RenderManager::GetInstance()->Initialize();

	RenderConsoleLibrary::SetConsoleCaption(GameProperties.GameName.c_str());

	CurrentLevel = StartupLevel;
	if (CurrentLevel.Get()) CurrentLevel->OpenLevel();
}

void GameEngine::Run()
{
	using namespace std::chrono;

	float GarbageCollectorWaitTime = 0.0f;
	const float GarbageCollectorWaitTimeLimit = 2.0f;

	auto Start = high_resolution_clock::now();
	float DeltaTime = 0.0f;

	bIsRunning = true;
	StartThreads();

	while (bIsRunning)
	{
		std::unique_lock<std::shared_mutex> lock(EngineMainProcessMtx);

		// Do the game logic here.
		{
			if (!CurrentLevel.Get())
			{
				break;
			}

			CurrentLevel->Update(DeltaTime);
		}

		RenderManager::GetInstance()->Render();

		// Calculating the DeltaTime.
		{
			auto End = high_resolution_clock::now();

			DeltaTime = duration_cast<duration<float>>(End - Start).count();
			Start = End;
		}

		// Garbage collection.
		{
			GarbageCollectorWaitTime += DeltaTime;

			if (GarbageCollectorWaitTime >= GarbageCollectorWaitTimeLimit)
			{
				GarbageCollector::GetInstance().Update();
				GarbageCollectorWaitTime -= GarbageCollectorWaitTimeLimit;
			}
		}
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
		InputManager::GetInstance()->ReadInput();
	}
}

/*
	Events.
*/

void GameEngine::HandleKeyPressEvent(void* KeyCode)
{
	if (!KeyCode) { engine_assert(false); return; }

	WORD virtualKeyCode = *reinterpret_cast<WORD*>(KeyCode);
	switch (virtualKeyCode)
	{
		case VK_ESCAPE:
			StopEngine();
			break;
		default:
			break;
	}
}

void GameEngine::HandleWindowResizeEvent(void* wbsr)
{
	/*
		There is an issue when the console app window's size is being reduced.
		There appears the scroll bar, that hides prev drawing buffer.
	*/

	if (!wbsr) { engine_assert(false); return; }

	WINDOW_BUFFER_SIZE_RECORD _wbsr = *reinterpret_cast<WINDOW_BUFFER_SIZE_RECORD*>(wbsr);

	const RC_SIZE currentWndSize = LastWndDimension.load();
	if (_wbsr.dwSize.X == currentWndSize.cx &&
		_wbsr.dwSize.Y == currentWndSize.cy)
	{
		return;
	}

	LastWndDimension = { (RC_UINT)_wbsr.dwSize.X, (RC_UINT)_wbsr.dwSize.Y };

	std::unique_lock<std::shared_mutex> lock(EngineMainProcessMtx);

	RenderManager::GetInstance()->ReconstructRenderBuffer();

	if (CurrentLevel.Get())
	{
		CurrentLevel->ReconstructLevel();
	}
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
	GarbageCollector::GetInstance().Free();
	DestroyRootObject();
}
