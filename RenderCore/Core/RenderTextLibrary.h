#pragma once

#include "RenderCore.h"
#include "RCTexture.h"

#include "stdint.h"

#include <string>

class RENDERCORE_API RenderTextLibrary
{
public:
	static void FillTextureWithScaledText(RCTexture* texture, std::string text, TEX_COLOR_RGB textColor, uint8_t textScale, TEX_COORD startCoord);
	static void Test_FillTextureWithScaledLetters(RCTexture* texture, char startLetter, TEX_COLOR_RGB textColor, uint8_t textScale, TEX_COORD startCoord);
};
