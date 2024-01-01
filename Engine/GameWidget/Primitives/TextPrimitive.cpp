#include "TextPrimitive.h"

#include "Core/RenderTextureLibrary.h"

TextPrimitive::TextPrimitive()
{
}

TextPrimitive::~TextPrimitive()
{
}

void TextPrimitive::Construct()
{
	TEX_RECT textFillRect = GetDrawRect();
	uint16_t fillWidth = GetTexWidth(textFillRect);
	uint16_t fillHeight = GetTexHeight(textFillRect);

	BrushStyle style;
	style.Image = std::make_shared<RCTexture>(fillWidth, fillHeight);

	TEX_PIXEL* texData = new TEX_PIXEL[fillWidth * fillHeight];
	memset(texData, RenderConstants::AlphaZeroPixel, fillWidth * fillHeight * sizeof(TEX_PIXEL));

	// TODO: Consider text alignment.
	{
		// Center the text.
		const size_t textLen = Text.length();
		const int textDisplacementLeft = (fillWidth - (uint16_t)textLen) / 2;

		for (size_t ti = 0; ti < textLen; ++ti)
		{
			int drawX = textDisplacementLeft + (uint16_t)ti;
			if (drawX < 0) continue;
			if (drawX >= fillWidth) break;

			uint16_t drawY = fillHeight / 2;

			texData[drawY * fillWidth + drawX] = Text[ti];
		}
	}

	TEX_RECT rect = { 0, 0, fillWidth, fillHeight };
	TEX_COORD coord = { 0, 0 };
	RenderTextureLibrary::FillTexture(style.Image.get(), texData, rect, GetTexWidth(rect), coord);
	RenderTextureLibrary::FillTextureColor(style.Image.get(), Font.FontColor);

	delete[] texData;
	texData = nullptr;

	SetBrushStyle(style);

	Inherited::Construct();
}
