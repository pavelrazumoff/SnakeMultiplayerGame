#pragma once

#include "TextBlock.h"

#include "Engine/Input/InputHandler.h"

class TextEditBox : public TextBlock, public IInputHandler
{
	typedef TextBlock Inheried;

public:
	TextEditBox();

	/** Extending the IInputHandler interface. */

	virtual bool PassInput(const InputState& is) override;

private:
	void ProcessKeyPressed(uint_least16_t keyCode);
	void ProcessKeyReleased(uint_least16_t keyCode);
	void ProcessToggledKeys(const InputState& is);

	void DropWaitKeys();

	bool ShouldDrawUpperCase() const;

private:
	bool bInFocus = false;

	std::vector<uint_least16_t> waitKeys2Release;
	std::string textBuffer;

	bool bShiftPressed = false;
	bool bCapsLock = false;
};
