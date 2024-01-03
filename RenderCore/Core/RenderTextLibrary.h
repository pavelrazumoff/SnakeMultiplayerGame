#pragma once

#include "RenderCore.h"
#include "RCTexture.h"

#include "stdint.h"

#include <string>

#define USE_SCALED_TEXT() 0

class RENDERCORE_API RenderTextLibrary
{
public:
	/** Scaled text. */

	#if USE_SCALED_TEXT()
	static void FillTextureWithScaledText(RCTexture* texture, std::string text, TEX_COLOR_RGB textColor, uint8_t textScale, TEX_COORD startCoord);
	static void Test_FillTextureWithScaledLetters(RCTexture* texture, char startLetter, TEX_COLOR_RGB textColor, uint8_t textScale, TEX_COORD startCoord);

	static RC_SIZE GetScaledTextSize(std::string text, uint8_t textScale);
	#endif // USE_SCALED_TEXT()

	/** Non-scaled text. */

	static void FillTextureWithText(RCTexture* texture, std::string text, TEX_COLOR_RGB textColor, TEX_COORD startCoord);
	static void FillTextureWithTextBig(RCTexture* texture, std::string text, TEX_COLOR_RGB textColor, TEX_COORD startCoord);

	static RC_SIZE GetTextSize(std::string text, uint8_t fontSize = 0);
	static RC_SIZE GetTextBigSize(std::string text, uint8_t fontSize = 0);
};
