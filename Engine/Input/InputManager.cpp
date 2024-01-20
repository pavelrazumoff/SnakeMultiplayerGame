#include "InputManager.h"

#include "Engine/EngineUtility.h"
#include "Engine/Log/Logger.h"

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

	for (int i = numEvents - 1; i >= 0; --i)
	{
		WORD eventType = irInBuf[i].EventType;

		if ((i < (int)numEvents - 1) &&
			_CheckIfAlreadyHandled(irInBuf, i))
		{
			//std::string msg = "already handled eventType = " + std::to_string(eventType);
			//Logger::GetInstance().Write(msg.c_str());
			continue;
		}

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
	static uint64_t imbMaskPrev = 0;
	uint64_t imbMask = 0;

	if (mer.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
		imbMask |= ENUM2BIT(InputMouseButton::Left);
	if (mer.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
		imbMask |= ENUM2BIT(InputMouseButton::Right);
	if (mer.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
		imbMask |= ENUM2BIT(InputMouseButton::Middle);

	switch (mer.dwEventFlags)
	{
		case 0:
			{
				if (imbMask != 0)
				{
					MouseBtnPressEvent.Trigger(mer, imbMask);
				}

				const uint64_t imbReleaseMaskDiff = imbMaskPrev & (imbMask ^ imbMaskPrev);
				if (imbReleaseMaskDiff)
				{
					MouseBtnReleaseEvent.Trigger(mer, imbReleaseMaskDiff);
				}
			}
			break;
		case DOUBLE_CLICK:
			MouseDoubleClickEvent.Trigger(mer, imbMask);
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

	imbMaskPrev = imbMask;
}

void InputManager::_OnWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD& wbsr)
{
	WindowResizeEvent.Trigger(wbsr);
}

bool InputManager::_CheckIfAlreadyHandled(const INPUT_RECORD* irs, int indexToCheck) const
{
	for (int i = indexToCheck - 1; i >= 0; --i)
	{
		if (irs[i].EventType != irs[indexToCheck].EventType) continue;

		switch (irs[i].EventType)
		{
			case KEY_EVENT:
				if (irs[i].Event.KeyEvent.wVirtualKeyCode == irs[indexToCheck].Event.KeyEvent.wVirtualKeyCode &&
					irs[i].Event.KeyEvent.bKeyDown == irs[indexToCheck].Event.KeyEvent.bKeyDown)
					return true;
				break;
			case MOUSE_EVENT:
				if (irs[i].Event.MouseEvent.dwEventFlags == irs[indexToCheck].Event.MouseEvent.dwEventFlags)
					return true;
				break;
			case WINDOW_BUFFER_SIZE_EVENT:
				return true;
			default:
				break;
		}
	}

	return false;
}
