#pragma once

#include "Core/RCDataTypes.h"
#include <vector>

enum class InputMouseButton
{
	None,

	Left,
	Right,
	Middle,
};

struct MouseCoords
{
	short X = 0;
	short Y = 0;
};

struct InputState
{
public:
	InputState& operator=(const InputState& other);

	void ResetFrameState();
	void ResetAll();

	void ConsumeInput() { bConsumed = true; }

	bool IsConsumed() const { return bConsumed; }

public:
	/** Mouse Input. */

	uint64_t MouseBtnPressedMask = 0;
	InputMouseButton MouseBtnClicked = InputMouseButton::None;
	InputMouseButton MouseBtnDoubleClicked = InputMouseButton::None;

	bool bMouseMoved = false;
	MouseCoords LastMousePos = { 0, 0 };
	MouseCoords LastMousePressPos = { 0, 0 };

	/** Keyboard Input. */

	std::vector<uint_least16_t> KeyPressedQueue;
	std::vector<uint_least16_t> KeyToggledQueue;

private:
	bool bConsumed = false;
};

// TODO: Put into namespace.
inline bool IsPointInsideRect(const MouseCoords& Point, const RC_RECT& Rect)
{
	return (Point.X >= Rect.left && Point.X <= Rect.right) &&
		(Point.Y >= Rect.top && Point.Y <= Rect.bottom);
}
