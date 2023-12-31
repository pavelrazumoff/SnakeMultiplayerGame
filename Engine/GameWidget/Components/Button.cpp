#include "Button.h"

#include "Engine/GameObject/GameObjectUtility.h"
#include "Core/RenderTextureLibrary.h"

ButtonWidgetSettings::ButtonWidgetSettings()
{
	DefaultStyle = CreateNewObject<BrushPrimitive>();
	{
		BrushStyle style;
		style.Image = std::make_shared<RCTexture>(8, 3);

		const TEX_PIXEL texData[] =
			"********"
			"*      *"
			"********";

		TEX_RECT rect = { 0, 0, 8, 3 };
		TEX_COORD coord = { 0, 0 };
		RenderTextureLibrary::FillTexture(style.Image.get(), texData, rect, GetTexWidth(rect), coord);
		RenderTextureLibrary::FillTextureColor(style.Image.get(), RenderConstants::GrayPixelColorRGB);

		DefaultStyle->SetBrushStyle(style);
	}

	HoveredStyle = GameObject::CloneObject(DefaultStyle.Get());
	{
		auto hoverStyle = HoveredStyle->GetBrushStyle();
		RenderTextureLibrary::FillTextureColor(hoverStyle.Image.get(), RenderConstants::LightGrayPixelColorRGB);
	}

	PressedStyle = GameObject::CloneObject(DefaultStyle.Get());
	{
		auto pressedStyle = PressedStyle->GetBrushStyle();
		RenderTextureLibrary::FillTextureColor(pressedStyle.Image.get(), RenderConstants::BluePixelColorRGB);
	}

	DisabledStyle = GameObject::CloneObject(DefaultStyle.Get());
	{
		auto disabledStyle = DisabledStyle->GetBrushStyle();
		RenderTextureLibrary::FillTextureColor(disabledStyle.Image.get(), RenderConstants::DarkGrayPixelColorRGB);
	}
}

void ButtonWidgetSettings::RepositionBrushes(const RC_RECT& newRect)
{
	BrushPrimitive* buttonBrushes[] = { DefaultStyle.Get(), HoveredStyle.Get(), PressedStyle.Get(), DisabledStyle.Get() };
	for (BrushPrimitive* brush : buttonBrushes)
	{
		if (!brush)
		{
			DebugEngineTrap();
			continue;
		}

		brush->UpdateDrawRect(newRect);
		brush->Construct();
	}
}

// *****************************************************************************************************

Button::Button()
{
	ButtonSettings.DefaultStyle->ChangeOwner(this);
	ButtonSettings.HoveredStyle->ChangeOwner(this);
	ButtonSettings.PressedStyle->ChangeOwner(this);
	ButtonSettings.DisabledStyle->ChangeOwner(this);

	AddWidgetComponent(&Alignment);
	AddWidgetComponent(&Layout);
}

Button::~Button()
{
}

void Button::RepositionWidget(const RC_RECT& newRect)
{
	Inheried::RepositionWidget(newRect);

	ButtonSettings.RepositionBrushes(newRect);
}

void Button::DrawWidget(RCTexture* RenderTargetTexture)
{
	BrushPrimitive* brush = GetCurrentBrush();
	if (!brush)
	{
		DebugEngineTrap();
		return;
	}

	brush->Draw(RenderTargetTexture);
}

bool Button::PassInput(const InputState& is)
{
	const BrushPrimitive* brush = GetCurrentBrush();
	if (!brush) return false;

	const RC_RECT& drawRect = brush->GetDrawRect();
	const MouseCoords& mousePos = is.LastMousePos;

	const bool bMouseIntersect = IsPointInsideRect(mousePos, drawRect);
	const bool bMouseStillPressed = (is.MouseBtnPressedMask != 0);

	if (is.bMouseMoved && 
		!bMouseStillPressed)
	{
		if (bMouseIntersect)
			State = ButtonState::Hovered;
		else if (State != ButtonState::Default)
			State = ButtonState::Default;

		return true;
	}

	if (State == ButtonState::Pressed &&
		!bMouseStillPressed)
	{
		State = ButtonState::Default;
	}

	if (bMouseIntersect)
	{
		if (is.MouseBtnClicked != InputMouseButton::None)
		{
			// TODO: Handle click event. Produce some event.
		}

		if (bMouseStillPressed)
		{
			if (State != ButtonState::Pressed)
			{
				if (IsPointInsideRect(is.InitialMousePressPos, drawRect))
					State = ButtonState::Pressed;
			}
		}
		else
			State = ButtonState::Hovered;

		return true;
	}

	return false;
}

RC_SIZE Button::CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc)
{
	RC_SIZE dirtySize = { 0, 0 };
	_bSizeXNeedsToRecalc = _bSizeYNeedToRecalc = false;

	if (Layout.DoesOverrideWidth()) dirtySize.cx = Layout.DimensionsOverride.cx;
	else _bSizeXNeedsToRecalc = true;
	if (Layout.DoesOverrideHeight()) dirtySize.cy = Layout.DimensionsOverride.cy;
	else _bSizeYNeedToRecalc = true;

	return dirtySize;
}

GameObject* Button::Clone() const
{
	Button* newWidget = CreateNewObject<Button>(GetOwner());
	newWidget->ButtonSettings.DefaultStyle.Get()->SetBrushStyle(ButtonSettings.DefaultStyle.Get()->GetBrushStyle());
	newWidget->ButtonSettings.HoveredStyle.Get()->SetBrushStyle(ButtonSettings.HoveredStyle.Get()->GetBrushStyle());
	newWidget->ButtonSettings.PressedStyle.Get()->SetBrushStyle(ButtonSettings.PressedStyle.Get()->GetBrushStyle());
	newWidget->ButtonSettings.DisabledStyle.Get()->SetBrushStyle(ButtonSettings.DisabledStyle.Get()->GetBrushStyle());

	newWidget->Alignment = this->Alignment;
	newWidget->Layout = this->Layout;

	return newWidget;
}

BrushPrimitive* Button::GetCurrentBrush()
{
	BrushPrimitive* brush = nullptr;
	switch (State)
	{
		case ButtonState::Default:
			return ButtonSettings.DefaultStyle.Get();
		case ButtonState::Hovered:
			return ButtonSettings.HoveredStyle.Get();
		case ButtonState::Pressed:
			return ButtonSettings.PressedStyle.Get();
		case ButtonState::Disabled:
			return ButtonSettings.DisabledStyle.Get();
		default:
			break;
	}

	return nullptr;
}
