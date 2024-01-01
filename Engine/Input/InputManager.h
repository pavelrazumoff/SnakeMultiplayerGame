#pragma once

#include "../Events/EventManager.h"
#include "../Events/EventDelegate.h"

#include "InputUtility.h"

#include <windows.h>

#define DECLARE_EVENT_DELEGATE(EventType, ...) \
	using EventType = EventDelegate<__VA_ARGS__> 

DECLARE_EVENT_DELEGATE(KeyDelegate, WORD);

DECLARE_EVENT_DELEGATE(MousePressDelegate, const MOUSE_EVENT_RECORD&, uint64_t);

DECLARE_EVENT_DELEGATE(MouseDoubleClickDelegate, const MOUSE_EVENT_RECORD&, uint64_t);

DECLARE_EVENT_DELEGATE(MouseMoveDelegate, const MOUSE_EVENT_RECORD&);
DECLARE_EVENT_DELEGATE(MouseWheelDelegate, const MOUSE_EVENT_RECORD&);

DECLARE_EVENT_DELEGATE(WindowResizeDelegate, const WINDOW_BUFFER_SIZE_RECORD&);

class InputManager
{
public:
	InputManager();
	~InputManager() {}

	static InputManager& GetInstance();

	void Initialize();
	void Release();

	void ReadInput();

	#if USE_EVENT_MANAGER()
	template<typename T>
	void RegisterInputCallback(const std::string& event, T* instance, void (T::* method)(void*))
	{
		InputEventsManager.Subscribe(event, instance, method);
	}
	#endif // USE_EVENT_MANAGER()

	KeyDelegate& OnKeyPressEvent() { return KeyPressEvent; }
	KeyDelegate& OnKeyReleaseEvent() { return KeyReleaseEvent; }

	MousePressDelegate& OnMouseBtnPressEvent() { return MouseBtnPressEvent; }
	MousePressDelegate& OnMouseBtnReleaseEvent() { return MouseBtnReleaseEvent; }

	MouseDoubleClickDelegate& OnMouseDoubleClickEvent() { return MouseDoubleClickEvent; }

	MouseMoveDelegate& OnMouseMoveEvent() { return MouseMoveEvent; }
	MouseWheelDelegate& OnMouseWheelEvent() { return MouseWheelEvent; }

	WindowResizeDelegate& OnWindowResizeEvent() { return WindowResizeEvent; }

private:
	void _OnKeyEvent(const KEY_EVENT_RECORD& ker);
	void _OnMouseEvent(const MOUSE_EVENT_RECORD& mer);

	void _OnWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD& wbsr);

protected:
	KeyDelegate KeyPressEvent;
	KeyDelegate KeyReleaseEvent;

	MouseDoubleClickDelegate MouseDoubleClickEvent;

	MousePressDelegate MouseBtnPressEvent;
	MousePressDelegate MouseBtnReleaseEvent;

	MouseMoveDelegate MouseMoveEvent;
	MouseWheelDelegate MouseWheelEvent;

	WindowResizeDelegate WindowResizeEvent;

private:
	DWORD fdwSaveOldMode = 0;
};
