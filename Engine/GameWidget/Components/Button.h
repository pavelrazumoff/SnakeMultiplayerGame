#pragma once

#include "GameWidget.h"
#include "../Primitives/BrushPrimitive.h"

#include "../Utility/AlignmentSettings.h"
#include "../Utility/LayoutSettings.h"

#include "Engine/Input/InputHandler.h"
#include "Engine/Events/EventDelegate.h"

class Button;

DECLARE_EVENT_DELEGATE(ButtonClickDelegate, Button*);

enum class ButtonState
{
	Default,
	Hovered,
	Pressed,
	Disabled
};

struct ButtonWidgetSettings
{
public:
	ButtonWidgetSettings();

protected:
	void RepositionBrushes(const RC_RECT& newRect);

protected:
	friend class Button;

public:
	bool bPressedOnClick = false; // If we press and then release the button, will it be pressed?

	TObjectPtr<BrushPrimitive> DefaultStyle;
	TObjectPtr<BrushPrimitive> HoveredStyle;
	TObjectPtr<BrushPrimitive> PressedStyle;
	TObjectPtr<BrushPrimitive> DisabledStyle;
};

class Button : public GameWidget, public IInputHandler
{
	typedef GameWidget Inheried;

public:
	Button();
	virtual ~Button();

	virtual void RepositionWidget(const RC_RECT& newRect) override;
	virtual void ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount) override;

	virtual void DrawWidget(RCTexture* RenderTargetTexture) override;

	/** Extending the IInputHandler interface. */

	virtual bool PassInput(const InputState& is) override;

	/**  */

	virtual RC_SIZE CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc) override;

	AlignmentSettings& GetAlignment() { return Alignment; }
	LayoutSettings& GetLayout() { return Layout; }
	ButtonWidgetSettings& GetButtonSettings() { return ButtonSettings; }

	/** Events. */

	ButtonClickDelegate& OnClickEvent() { return ClickEvent; }

protected:
	virtual GameObject* Clone(GameObject* _owner) const override;

	BrushPrimitive* GetCurrentBrush();

protected:
	AlignmentSettings Alignment;
	LayoutSettings Layout;
	ButtonWidgetSettings ButtonSettings;

	ButtonClickDelegate ClickEvent;

private:
	ButtonState State = ButtonState::Default;
};