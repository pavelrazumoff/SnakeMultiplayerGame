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
	LastMousePressPos = other.LastMousePressPos;

	// Keyboard Input.
	KeyPressedQueue = other.KeyPressedQueue;
	KeyToggledQueue = other.KeyToggledQueue;

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

	// Consumption.
	bConsumed = false;
}

void InputState::ResetAll()
{
	ResetFrameState();

	MouseBtnPressedMask = 0;
	LastMousePos = { 0, 0 };
	LastMousePressPos = { 0, 0 };

	// Keyboard Input.
	KeyPressedQueue.clear();
	KeyToggledQueue.clear();
}
