#include "InputUtility.h"

InputState& InputState::operator=(const InputState& other)
{
	if (this == &other) return *this;

	// Mouse Input.
	MouseBtnPressedMask = other.MouseBtnPressedMask;
	MouseBtnClicked = other.MouseBtnClicked;
	MouseBtnDoubleClicked = other.MouseBtnDoubleClicked;

	bMouseMoved = other.bMouseMoved;
	LastMousePos = other.LastMousePos;
	InitialMousePressPos = other.InitialMousePressPos;

	// Keyboard Input.
	KeyPressedQueue = other.KeyPressedQueue;

	// Consumption.
	bConsumed = other.bConsumed;

	return *this;
}

void InputState::ResetFrameState()
{
	// Mouse Input.
	MouseBtnClicked = InputMouseButton::None;
	MouseBtnDoubleClicked = InputMouseButton::None;

	bMouseMoved = false;

	// Reset consumed flag.
	bConsumed = false;
}

void InputState::ResetAll()
{
	ResetFrameState();

	MouseBtnPressedMask = 0;
	LastMousePos = { 0, 0 };
	InitialMousePressPos = { 0, 0 };

	// Keyboard Input.
	while (!KeyPressedQueue.empty())
		KeyPressedQueue.pop();
}
