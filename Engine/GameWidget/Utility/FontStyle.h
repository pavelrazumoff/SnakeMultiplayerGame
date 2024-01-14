#pragma once

#include "RenderCore/Core/RCTexture.h"

enum class FontPrintType
{
	Simple,
	Letter,
	LetterBig,
};

struct FontStyle
{
public:
	FontStyle& operator=(const FontStyle& other)
	{
		if (this == &other) return *this;

		FontSize = other.FontSize;
		FontType = other.FontType;
		FontColor = other.FontColor;
		FontFillerPixel = other.FontFillerPixel;

		return *this;
	}

public:
	uint8_t FontSize = 0;
	FontPrintType FontType = FontPrintType::Simple;
	TEX_COLOR_RGB FontColor = RenderConstants::WhitePixelColorRGB;
	TEX_PIXEL FontFillerPixel = 0;
};
