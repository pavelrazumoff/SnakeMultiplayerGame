#pragma once

#include "GameWidget.h"
#include "../Primitives/BrushPrimitive.h"

#include "../Utility/AlignmentSettings.h"
#include "../Utility/LayoutSettings.h"

#include "Engine/Input/InputHandler.h"

enum class ButtonState
{
	Default,
	Hovered,
	Pressed,
	Disabled
};

class Button : public GameWidget, public IInputHandler
{
	typedef GameWidget Inheried;

public:
	Button();
	virtual ~Button();

	virtual void RepositionWidget(const RC_RECT& newRect) override;
	virtual void DrawWidget(RCTexture* RenderTargetTexture) override;

	/** Extending the IInputHandler interface. */
	virtual bool PassInput(const InputState& is) override;

	virtual RC_SIZE CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc) override;

	void SetPressOnClick(bool _bPressOnClick) { bPressOnClick = _bPressOnClick; }

	AlignmentSettings& GetAlignment() { return Alignment; }
	LayoutSettings& GetLayout() { return Layout; }

protected:
	virtual GameObject* Clone() const override;

	BrushPrimitive* GetCurrentBrush();

protected:
	TObjectPtr<BrushPrimitive> DefaultStyle;
	TObjectPtr<BrushPrimitive> HoveredStyle;
	TObjectPtr<BrushPrimitive> PressedStyle;
	TObjectPtr<BrushPrimitive> DisabledStyle;

	AlignmentSettings Alignment;
	LayoutSettings Layout;

private:
	ButtonState State = ButtonState::Default;
	bool bPressOnClick = false;
};