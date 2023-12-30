#pragma once

#include "GameWidget.h"
#include "../Primitives/BrushPrimitive.h"

#include "../Utility/AlignmentSettings.h"
#include "../Utility/LayoutSettings.h"

enum class ButtonState
{
	Default,
	Hovered,
	Pressed,
	Disabled
};

class Button : public GameWidget
{
	typedef GameWidget Inheried;

public:
	Button();
	virtual ~Button();

	virtual void RepositionWidget(const RC_RECT& newRect) override;
	virtual void DrawWidget(RCTexture* RenderTargetTexture) override;

	virtual RC_SIZE CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc) override;

	void ChangeButtonState(ButtonState newState) { State = newState; }

	AlignmentSettings& GetAlignment() { return Alignment; }
	LayoutSettings& GetLayout() { return Layout; }

protected:
	virtual GameObject* Clone() const override;

protected:
	TObjectPtr<BrushPrimitive> DefaultStyle;
	TObjectPtr<BrushPrimitive> HoveredStyle;
	TObjectPtr<BrushPrimitive> PressedStyle;
	TObjectPtr<BrushPrimitive> DisabledStyle;

	AlignmentSettings Alignment;
	LayoutSettings Layout;

private:
	ButtonState State = ButtonState::Default;
};