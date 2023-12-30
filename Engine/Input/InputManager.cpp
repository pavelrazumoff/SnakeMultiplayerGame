#include "InputManager.h"

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
	INPUT_RECORD irInBuf[128];
	DWORD numEvents;

	if (!ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), irInBuf, 128, &numEvents))
	{
		DWORD err = GetLastError();
		// TODO: Handle error.
		return;
	}

	int savedIndexOfLastResizeEvent = -1;
	for (DWORD i = 0; i < numEvents; ++i)
	{
		switch (irInBuf[i].EventType)
		{
		case KEY_EVENT:
			OnKeyEvent(irInBuf[i].Event.KeyEvent);
			break;
		case MOUSE_EVENT:
			OnMouseEvent(irInBuf[i].Event.MouseEvent);
			break;
		case WINDOW_BUFFER_SIZE_EVENT:
			savedIndexOfLastResizeEvent = i;
			break;
		default:
			break;
		}
	}

	if (savedIndexOfLastResizeEvent >= 0)
		OnWindowResizeEvent(irInBuf[savedIndexOfLastResizeEvent].Event.WindowBufferSizeEvent);
}

void InputManager::OnKeyEvent(KEY_EVENT_RECORD ker)
{
	if (ker.bKeyDown == 1)
		InputEventsManager.Trigger("KeyPressEvent", reinterpret_cast<void*>(&ker.wVirtualKeyCode));
}

void InputManager::OnMouseEvent(MOUSE_EVENT_RECORD mer)
{
	switch (mer.dwEventFlags)
	{
		case 0:
			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{

			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{

			}
			else
			{
				// Some other button pressed.
			}
			break;
		case DOUBLE_CLICK:
			break;
		case MOUSE_MOVED:
			break;
		case MOUSE_WHEELED:
			break;
		default:
			break;
	}
}

void InputManager::OnWindowResizeEvent(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	InputEventsManager.Trigger("WindowResizeEvent", reinterpret_cast<void*>(&wbsr));
}
