#include "GameEngine.h"

#include "GarbageCollector.h"
#include "Render/RenderManager.h"

#include "Core/RenderConsoleLibrary.h"

#include <chrono>
#include <windows.h>
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

GameEngine::GameEngine()
{
	InitializeRootObject();
}

GameEngine::~GameEngine()
{
	Cleanup();
}

void GameEngine::Initialization(GameLevel* StartupLevel)
{
	FillEngineProperties();

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

	RenderManager::GetInstance()->ReconstructRenderBuffer();

	while (true)
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

		// TODO: Handle Exit Game after user input.

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

	bIsRunning = false;
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

	// Window resize events.
	WindowEventsThreadHandle = std::thread(&GameEngine::WindowEventsThread, this);
}

void GameEngine::StopThreads()
{
	WindowEventsThreadHandle.join();
}

void GameEngine::WindowEventsThread()
{
	using namespace std::chrono;
	auto WndEventStart = high_resolution_clock::now();
	const float WndEventCheckWaitLimit = 0.5f;//0.05f;

	while (bIsRunning)
	{
		auto Now = high_resolution_clock::now();
		float TimePassed = duration_cast<duration<float>>(Now - WndEventStart).count();

		if (TimePassed >= WndEventCheckWaitLimit)
		{
			WndEventStart = Now;

			CheckForWindowSizeChanged();
		}
	}
}

/*
	Events.
*/

void GameEngine::OnWindowResizeEvent(const RC_SIZE& NewSize)
{
	/*
		There is an issue when the console app window's size is being reduced.
		There appears the scroll bar, that hides prev drawing buffer.
	*/

	std::unique_lock<std::shared_mutex> lock(EngineMainProcessMtx);

	RenderManager::GetInstance()->ReconstructRenderBuffer();

	if (CurrentLevel.Get())
	{
		CurrentLevel->ReconstructLevel();
	}
}

/*
	Check for Events.
*/

void GameEngine::CheckForWindowSizeChanged()
{
	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();

	if (consoleDim.cx != LastWndDimension.cx ||
		consoleDim.cy != LastWndDimension.cy)
	{
		LastWndDimension = consoleDim;
		OnWindowResizeEvent(consoleDim);
	}
}

/*
	Cleanup.
*/

void GameEngine::Cleanup()
{
	GarbageCollector::GetInstance().Free();
	DestroyRootObject();
}
