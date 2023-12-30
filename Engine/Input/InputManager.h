#pragma once

#include "../Events/EventManager.h"

#include <windows.h>

class InputManager
{
public:
	InputManager();
	~InputManager() {}

	static InputManager* GetInstance();

	void Initialize();
	void ReadInput();

	template<typename T>
	void RegisterInputCallback(const std::string& event, T* instance, void (T::* method)(void*))
	{
		InputEventsManager.Subscribe(event, instance, method);
	}

protected:
	void OnKeyEvent(KEY_EVENT_RECORD ker);
	void OnMouseEvent(MOUSE_EVENT_RECORD mer);

	void OnWindowResizeEvent(WINDOW_BUFFER_SIZE_RECORD wbsr);

protected:
	EventManager InputEventsManager;
};
