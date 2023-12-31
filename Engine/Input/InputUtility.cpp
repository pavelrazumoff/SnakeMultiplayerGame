#include "InputUtility.h"

InputState& InputState::operator=(const InputState& other)
{
	if (this == &other) return *this;

	// Mouse Input.
	MouseBtnPressedMask = other.MouseBtnPressedMask;
	MouseBtnReleasedMask = other.MouseBtnReleasedMask;
	MouseBtnClicked = other.MouseBtnClicked;
	MouseBtnDoubleClicked = other.MouseBtnDoubleClicked;

	bMouseMoved = other.bMouseMoved;
	LastMousePos = other.LastMousePos;
	InitialMousePressPos = other.InitialMousePressPos;

	// Keyboard Input.
	KeyPressedQueue = other.KeyPressedQueue;
	KeyReleasedQueue = other.KeyReleasedQueue;

	// Consumption.
	bConsumed = other.bConsumed;

	return *this;
}

void InputState::Reset()
{
	// Mouse Input.
	MouseBtnPressedMask = 0;
	MouseBtnReleasedMask = 0;
	MouseBtnClicked = InputMouseButton::None;
	MouseBtnDoubleClicked = InputMouseButton::None;

	bMouseMoved = false;
	LastMousePos = { 0, 0 };
	InitialMousePressPos = { 0, 0 };

	// Keyboard Input.
	while (!KeyPressedQueue.empty())
		KeyPressedQueue.pop();

	while (!KeyReleasedQueue.empty())
		KeyReleasedQueue.pop();

	// Reset consumed flag.
	bConsumed = false;
}
