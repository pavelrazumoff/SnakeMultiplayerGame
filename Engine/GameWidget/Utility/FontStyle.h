#pragma once

#include "RenderCore/Core/RCTexture.h"

struct FontStyle
{
public:
	FontStyle& operator=(const FontStyle& other)
	{
		if (this == &other) return *this;

		FontSize = other.FontSize;
		FontColor = other.FontColor;

		return *this;
	}

public:
	uint8_t FontSize = 0;
	TEX_COLOR_RGB FontColor = RenderConstants::WhitePixelColorRGB;
};
