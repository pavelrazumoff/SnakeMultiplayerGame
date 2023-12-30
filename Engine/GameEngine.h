#pragma once

#include "GameLevel.h"
#include "Core/RCDataTypes.h"

#include <thread>

// TODO: Move to another place.
struct GamePropertiesInfo
{
	std::string GameName = "Game";
};

class GameEngine
{
public:
	GameEngine(const GamePropertiesInfo& props);
	virtual ~GameEngine();

	void Initialization(GameLevel* StartupLevel);
	void Run();

protected:
	/** Initialization. */

	void FillEngineProperties();

	/** Threads. */

	void StartThreads();
	void StopThreads();

	void InputEventsThread();

	/** Events. */

	void HandleKeyPressEvent(void* KeyCode);
	void HandleWindowResizeEvent(void* wbsr);

	/** Engine Stop and Cleanup. */

	void StopEngine();
	void Cleanup();

protected:
	GamePropertiesInfo GameProperties;

	TObjectPtr<GameLevel> CurrentLevel;

private:
	std::atomic<bool> bIsRunning = false;

	/** Threads. */

	std::thread InputEventsThreadHandle;

	/** Engine Properties. */

	std::atomic<RC_SIZE> LastWndDimension;
};
