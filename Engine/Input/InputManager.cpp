#include "InputManager.h"

#include "Engine/EngineUtility.h"

#include <unordered_set>

InputManager::InputManager()
{
}

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}

void InputManager::Initialize()
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	GetConsoleMode(hInput, &fdwSaveOldMode);

	const DWORD fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hInput, fdwMode))
	{
		DebugEngineTrap();
	}
}

void InputManager::Release()
{
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), fdwSaveOldMode);
}

void InputManager::ReadInput()
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	DWORD numEvents;
	if (!GetNumberOfConsoleInputEvents(hInput, &numEvents))
	{
		DWORD err = GetLastError();
		return;
	}

	if (!numEvents) return;

	INPUT_RECORD irInBuf[128];
	if (!ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), irInBuf, 128, &numEvents))
	{
		DWORD err = GetLastError();
		// TODO: Handle error.
		return;
	}

	std::unordered_set<WORD> HandledTypes;
	for (int i = numEvents - 1; i >= 0; --i)
	{
		WORD eventType = irInBuf[i].EventType;

		auto result = HandledTypes.insert(eventType);
		if (!result.second) continue;

		HandledTypes.insert(eventType);

		switch (eventType)
		{
			case KEY_EVENT:
				_OnKeyEvent(irInBuf[i].Event.KeyEvent);
				break;
			case MOUSE_EVENT:
				_OnMouseEvent(irInBuf[i].Event.MouseEvent);
				break;
			case WINDOW_BUFFER_SIZE_EVENT:
				_OnWindowResizeEvent(irInBuf[i].Event.WindowBufferSizeEvent);
				break;
			default:
				break;
		}
	}
}

void InputManager::_OnKeyEvent(const KEY_EVENT_RECORD& ker)
{
	if (ker.bKeyDown == 1)
		KeyPressEvent.Trigger(ker.wVirtualKeyCode);
	else
		KeyReleaseEvent.Trigger(ker.wVirtualKeyCode);
}

void InputManager::_OnMouseEvent(const MOUSE_EVENT_RECORD& mer)
{
	static bool bLeftButtonDown = false;
	static bool bRightButtonDown = false;

	switch (mer.dwEventFlags)
	{
		case 0:
			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				bLeftButtonDown = true;
				MouseBtnPressEvent.Trigger(mer, InputMouseButton::Left);
			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				bRightButtonDown = true;
				MouseBtnPressEvent.Trigger(mer, InputMouseButton::Right);
			}
			else
			{
				if (bLeftButtonDown)
				{
					MouseBtnReleaseEvent.Trigger(mer, InputMouseButton::Left);
					bLeftButtonDown = false;
				}

				if (bRightButtonDown)
				{
					MouseBtnReleaseEvent.Trigger(mer, InputMouseButton::Right);
					bRightButtonDown = false;
				}
			}
			break;
		case DOUBLE_CLICK:
			MouseDoubleClickEvent.Trigger(mer);
			break;
		case MOUSE_MOVED:
			MouseMoveEvent.Trigger(mer);
			break;
		case MOUSE_WHEELED:
			MouseWheelEvent.Trigger(mer);
			break;
		default:
			break;
	}
}

void InputManager::_OnWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD& wbsr)
{
	WindowResizeEvent.Trigger(wbsr);
}
