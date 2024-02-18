#include "TextEditBox.h"

#include "Engine/GameObject/GameObjectUtility.h"
#include "Engine/GameWidget/GameWidgetManager.h"

#include <windows.h>

TextEditBox::TextEditBox()
{
	
}

bool TextEditBox::PassInput(const InputState& is)
{
	const RC_RECT& drawRect = Text->GetDrawRect();
	const MouseCoords& mousePos = is.LastMousePos;

	// Mouse focus.
	{
		const bool bMouseIntersect = IsPointInsideRect(mousePos, drawRect);
		const bool bLastMousePressIntersect = IsPointInsideRect(is.LastMousePressPos, drawRect);
		const bool bMouseStillPressed = (is.MouseBtnPressedMask != 0);

		if (!bInFocus)
		{
			if (bLastMousePressIntersect && bMouseIntersect &&
				!bMouseStillPressed)
			{
				bInFocus = true;
				return true;
			}
		}
		else if (bMouseStillPressed)
		{
			if (!bLastMousePressIntersect || !bMouseIntersect)
			{
				bInFocus = false;
				DropWaitKeys();
				return true;
			}
		}
	}

	// Keyboard events.
	if (bInFocus)
	{
		ProcessToggledKeys(is);

		for (auto it = waitKeys2Release.begin(); it != waitKeys2Release.end();)
		{
			if (std::find(is.KeyPressedQueue.begin(), is.KeyPressedQueue.end(), *it) == is.KeyPressedQueue.end())
			{
				ProcessKeyReleased(*it);
				it = waitKeys2Release.erase(it);
			}
			else ++it;
		}

		textBuffer = GetText().GetText();
		for (auto& keyCode : is.KeyPressedQueue)
		{
			if (std::find(waitKeys2Release.begin(), waitKeys2Release.end(), keyCode) == waitKeys2Release.end())
			{
				waitKeys2Release.push_back(keyCode);
				ProcessKeyPressed(keyCode);
			}
		}

		if (textBuffer != GetText().GetText())
		{
			SetText(textBuffer);
			return true;
		}
	}

	return false;
}

void TextEditBox::ProcessKeyPressed(uint_least16_t keyCode)
{
	if (keyCode >= VK_SPACE && keyCode <= '~')
	{
		char code = static_cast<char>(keyCode);
		textBuffer += ShouldDrawUpperCase() ? toupper(code) : tolower(code);
	}

	if (keyCode == VK_BACK)
	{
		if (textBuffer.size() > 0)
			textBuffer.pop_back();
	}

	if (keyCode == VK_SHIFT)
		bShiftPressed = true;
}

void TextEditBox::ProcessKeyReleased(uint_least16_t keyCode)
{
	if (keyCode == VK_SHIFT)
		bShiftPressed = false;
}

void TextEditBox::ProcessToggledKeys(const InputState& is)
{
	// TODO: Optimize this. Don't iterate each frame.
	bCapsLock = false;
	for (auto& keyCode : is.KeyToggledQueue)
	{
		if (keyCode == VK_CAPITAL)
			bCapsLock = true;
	}
}

void TextEditBox::DropWaitKeys()
{
	waitKeys2Release.clear();
}

bool TextEditBox::ShouldDrawUpperCase() const
{
	return bCapsLock || bShiftPressed;
}
