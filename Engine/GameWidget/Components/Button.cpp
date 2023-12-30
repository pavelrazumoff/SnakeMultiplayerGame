#include "Button.h"

#include "Engine/GameObject/GameObjectUtility.h"
#include "Core/RenderTextureLibrary.h"

Button::Button()
{
	DefaultStyle = CreateNewObject<BrushPrimitive>(this);
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

	AddWidgetComponent(&Alignment);
	AddWidgetComponent(&Layout);
}

Button::~Button()
{
}

void Button::RepositionWidget(const RC_RECT& newRect)
{
	Inheried::RepositionWidget(newRect);

	BrushPrimitive* buttonBrushes[] = { DefaultStyle.Get(), HoveredStyle.Get(), PressedStyle.Get(), DisabledStyle.Get() };
	for (BrushPrimitive* brush : buttonBrushes)
	{
		if (!brush)
		{
			engine_assert(false);
			continue;
		}

		brush->UpdateDrawRect(newRect);
		brush->Construct();
	}
}

void Button::DrawWidget(RCTexture* RenderTargetTexture)
{
	BrushPrimitive* brush = nullptr;
	switch (State)
	{
		case ButtonState::Default:
			brush = DefaultStyle.Get();
			break;
		case ButtonState::Hovered:
			brush = HoveredStyle.Get();
			break;
		case ButtonState::Pressed:
			brush = PressedStyle.Get();
			break;
		case ButtonState::Disabled:
			brush = DisabledStyle.Get();
			break;
		default:
			break;
	}

	if (!brush)
	{
		engine_assert(false);
		return;
	}

	brush->Draw(RenderTargetTexture);
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
	newWidget->DefaultStyle.Get()->SetBrushStyle(DefaultStyle.Get()->GetBrushStyle());
	newWidget->HoveredStyle.Get()->SetBrushStyle(HoveredStyle.Get()->GetBrushStyle());
	newWidget->PressedStyle.Get()->SetBrushStyle(PressedStyle.Get()->GetBrushStyle());
	newWidget->DisabledStyle.Get()->SetBrushStyle(DisabledStyle.Get()->GetBrushStyle());

	newWidget->Alignment = this->Alignment;
	newWidget->Layout = this->Layout;

	return newWidget;
}
