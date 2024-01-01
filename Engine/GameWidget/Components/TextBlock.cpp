#include "TextBlock.h"

#include "Engine/GameObject/GameObjectUtility.h"

TextBlock::TextBlock()
{
	Text = CreateNewObject<TextPrimitive>(this);
	{
		FontStyle font;
		font.FontSize = 1;
		font.FontColor = RenderConstants::WhitePixelColorRGB;
		Text->SetFontStyle(font);
	}

	AddWidgetComponent(&Alignment);
	AddWidgetComponent(&Layout);
}

TextBlock::~TextBlock()
{
}

void TextBlock::RepositionWidget(const RC_RECT& newRect)
{
	Inheried::RepositionWidget(newRect);

	Text->UpdateDrawRect(newRect);
	Text->Construct();
}

void TextBlock::DrawWidget(RCTexture* RenderTargetTexture)
{
	Text->Draw(RenderTargetTexture);
}

RC_SIZE TextBlock::CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc)
{
	RC_SIZE dirtySize = { 0, 0 };
	_bSizeXNeedsToRecalc = _bSizeYNeedToRecalc = false;

	if (Layout.DoesOverrideWidth()) dirtySize.cx = Layout.DimensionsOverride.cx;
	else _bSizeXNeedsToRecalc = true;
	if (Layout.DoesOverrideHeight()) dirtySize.cy = Layout.DimensionsOverride.cy;
	else _bSizeYNeedToRecalc = true;

	return dirtySize;
}

GameObject* TextBlock::Clone() const
{
	return nullptr;
}
