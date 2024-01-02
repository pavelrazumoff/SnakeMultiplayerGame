#include "TextPrimitive.h"

#include "Core/RenderTextureLibrary.h"
#include "Core/RenderTextLibrary.h"

#include "Engine/GameObject/GameObjectUtility.h"

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

	// TODO: Consider text alignment.
	if (Font.FontSize == 0)
	{
		TEX_PIXEL* texData = new TEX_PIXEL[fillWidth * fillHeight];
		memset(texData, RenderConstants::AlphaZeroPixel, fillWidth * fillHeight * sizeof(TEX_PIXEL));

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

		TEX_RECT rect = { 0, 0, fillWidth - 1, fillHeight - 1 };
		TEX_COORD coord = { 0, 0 };
		RenderTextureLibrary::FillTexture(style.Image.get(), texData, rect, GetTexWidth(rect), coord);
		RenderTextureLibrary::FillTextureColor(style.Image.get(), Font.FontColor);

		delete[] texData;
		texData = nullptr;
	}
	else
	{
		TEX_COORD coord = { 0, 0 };
		RenderTextLibrary::FillTextureWithScaledText(style.Image.get(), Text, Font.FontColor, Font.FontSize, coord);
		//RenderTextLibrary::Test_FillTextureWithScaledLetters(style.Image.get(), 'A', Font.FontColor, Font.FontSize, coord);
	}

	SetBrushStyle(style);

	Inherited::Construct();
}

RC_SIZE TextPrimitive::GetTextDrawSize() const
{
	if (Font.FontSize == 0)
	{
		RC_SIZE textSize = { (RC_INT)Text.length(), 1 };
		return textSize;
	}

	RC_SIZE textSize = RenderTextLibrary::GetScaledTextSize(Text, Font.FontSize);
	return textSize;
}

GameObject* TextPrimitive::Clone(GameObject* _owner) const
{
	TextPrimitive* newText = CreateNewObject<TextPrimitive>(_owner);
	newText->SetFontStyle(GetFontStyle());
	newText->SetText(GetText());

	return newText;
}
