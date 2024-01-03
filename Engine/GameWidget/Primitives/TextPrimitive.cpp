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
	/*
		TODO: For different font sizes.
		Calculate the whole image with text dimensions based on text content (including width/height of each letter).
		This will be constant no matter what primitive draw rect is. We just draw the text on the image with the same size.
		Then in parent's Construct() we stretch the image with printed text to the draw rect.
	*/

	BrushStyle style;

	// TODO: Consider text alignment.
	if (Font.FontType == FontPrintType::Simple)
	{
		TEX_RECT textFillRect = GetDrawRect();
		uint16_t fillWidth = GetTexWidth(textFillRect);
		uint16_t fillHeight = GetTexHeight(textFillRect);

		style.Image = std::make_shared<RCTexture>(fillWidth, fillHeight);

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
		const bool bUseBigLetters = (Font.FontType == FontPrintType::LetterBig);

		// Always draw with the default font size.
		RC_SIZE textRenderSize = bUseBigLetters ? RenderTextLibrary::GetTextBigSize(Text) : RenderTextLibrary::GetTextSize(Text);
		style.Image = std::make_shared<RCTexture>(textRenderSize.cx, textRenderSize.cy);

		if (bUseBigLetters)
			RenderTextLibrary::FillTextureWithTextBig(style.Image.get(), Text, Font.FontColor, coord);
		else
			RenderTextLibrary::FillTextureWithText(style.Image.get(), Text, Font.FontColor, coord);
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

	// Scale the overall image with the specified font size (Font.FontSize == 1 is default).
	const bool bUseBigLetters = (Font.FontType == FontPrintType::LetterBig);

	RC_SIZE textSize = bUseBigLetters ? RenderTextLibrary::GetTextBigSize(Text, Font.FontSize - 1) :
		RenderTextLibrary::GetTextSize(Text, Font.FontSize - 1);
	return textSize;
}

GameObject* TextPrimitive::Clone(GameObject* _owner) const
{
	TextPrimitive* newText = CreateNewObject<TextPrimitive>(_owner);
	newText->SetFontStyle(GetFontStyle());
	newText->SetText(GetText());

	return newText;
}
