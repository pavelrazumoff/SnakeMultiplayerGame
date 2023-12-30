#include "InputManager.h"

#include <unordered_set>

InputManager::InputManager()
{
}

InputManager* InputManager::GetInstance()
{
	static InputManager instance;
	return &instance;
}

void InputManager::Initialize()
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	DWORD prevMode;
	GetConsoleMode(hInput, &prevMode);
	SetConsoleMode(hInput, prevMode | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
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
				OnKeyEvent(irInBuf[i].Event.KeyEvent);
				break;
			case MOUSE_EVENT:
				OnMouseEvent(irInBuf[i].Event.MouseEvent);
				break;
			case WINDOW_BUFFER_SIZE_EVENT:
				OnWindowResizeEvent(irInBuf[i].Event.WindowBufferSizeEvent);
				break;
			default:
				break;
		}
	}
}

void InputManager::OnKeyEvent(KEY_EVENT_RECORD ker)
{
	if (ker.bKeyDown == 1)
		InputEventsManager.Trigger("KeyPressEvent", reinterpret_cast<void*>(&ker.wVirtualKeyCode));
}

void InputManager::OnMouseEvent(MOUSE_EVENT_RECORD mer)
{
	static bool bLeftButtonDown = false;
	static bool bRightButtonDown = false;

	switch (mer.dwEventFlags)
	{
		case 0:
			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				bLeftButtonDown = true;
			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				bRightButtonDown = true;
			}
			else
			{
				if (bLeftButtonDown)
				{
					InputEventsManager.Trigger("MouseButtonClickEvent", reinterpret_cast<void*>(&mer));
					bLeftButtonDown = false;
				}

				if (bRightButtonDown)
				{
					InputEventsManager.Trigger("MouseButtonClickEvent", reinterpret_cast<void*>(&mer));
					bRightButtonDown = false;
				}
			}
			break;
		case DOUBLE_CLICK:
			InputEventsManager.Trigger("MouseDoubleClickEvent", reinterpret_cast<void*>(&mer));
			break;
		case MOUSE_MOVED:
			InputEventsManager.Trigger("MouseMoveEvent", reinterpret_cast<void*>(&mer));
			break;
		case MOUSE_WHEELED:
			InputEventsManager.Trigger("MouseWheelEvent", reinterpret_cast<void*>(&mer));
			break;
		default:
			break;
	}
}

void InputManager::OnWindowResizeEvent(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	InputEventsManager.Trigger("WindowResizeEvent", reinterpret_cast<void*>(&wbsr));
}
