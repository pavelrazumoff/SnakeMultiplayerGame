#pragma once

#include "GameLevel.h"
#include "Core/RCDataTypes.h"

#include <thread>

class GameEngine
{
public:
	GameEngine();
	virtual ~GameEngine();

	void Initialization(GameLevel* StartupLevel);
	void Run();

protected:
	/** Initialization. */

	void FillEngineProperties();

	/** Threads. */

	void StartThreads();
	void StopThreads();

	void WindowEventsThread();

	/** Events. */

	void OnWindowResizeEvent(const RC_SIZE& NewSize);

	/** Check for Events. */

	void CheckForWindowSizeChanged();

	/** Cleanup. */

	void Cleanup();

protected:
	TObjectPtr<GameLevel> CurrentLevel;

private:
	bool bIsRunning = false;

	/** Threads. */

	std::thread WindowEventsThreadHandle;

	/** Engine Properties. */

	RC_SIZE LastWndDimension = { 0 };
};
