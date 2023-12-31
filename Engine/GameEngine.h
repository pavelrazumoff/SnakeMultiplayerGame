#pragma once

#include "GameLevel.h"
#include "Core/RCDataTypes.h"

#include "Input/InputUtility.h"

#include <windows.h>
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

	void HandleKeyPressEvent(WORD keyCode);
	void HandleKeyReleaseEvent(WORD keyCode);

	void HandleMouseClickEvent(const MOUSE_EVENT_RECORD& mer, InputMouseButton imb);
	void HandleMouseDoubleClickEvent(const MOUSE_EVENT_RECORD& mer);

	void HandleMouseMoveEvent(const MOUSE_EVENT_RECORD& mer);
	void HandleMouseWheelEvent(const MOUSE_EVENT_RECORD& mer);

	void HandleWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD& wbsr);

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
