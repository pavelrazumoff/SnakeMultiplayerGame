#pragma once

#include "stdint.h"
#include "RenderCore.h"

typedef uint8_t TEX_PIXEL;

typedef uint16_t RC_UINT;
typedef int16_t RC_INT;

struct TEX_COORD
{
	RC_UINT x;
	RC_UINT y;
};

struct TEX_SIZE
{
	RC_UINT cx;
	RC_UINT cy;
};

typedef TEX_SIZE RC_SIZE;

struct TEX_RECT
{
	RC_UINT left;
	RC_UINT top;
	RC_UINT right;
	RC_UINT bottom;
};

typedef TEX_RECT RC_RECT;

inline uint16_t GetTexWidth(const TEX_RECT& texRect) { return texRect.right - texRect.left; }
inline uint16_t GetTexHeight(const TEX_RECT& texRect) { return texRect.bottom - texRect.top; }

typedef unsigned short TEX_COLOR;

struct TEX_COLOR_RGB
{
public:
	bool operator==(const TEX_COLOR_RGB& other) const
	{
		return (r == other.r) && (g == other.g) && (b == other.b);
	}

public:
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

namespace RenderConstants {

/** Pixel Data. */

inline constexpr TEX_PIXEL ClearPixel = ' ';
inline constexpr TEX_PIXEL AlphaZeroPixel = 0x00;

/** Color Data Legacy. */

//RENDERCORE_API extern const TEX_COLOR RedPixelColor;
//RENDERCORE_API extern const TEX_COLOR GreenPixelColor;
//RENDERCORE_API extern const TEX_COLOR BluePixelColor;
//RENDERCORE_API extern const TEX_COLOR WhitePixelColor;

/** Color Data VTP. */

inline constexpr TEX_COLOR_RGB BlackPixelColorRGB  = { 0, 0, 0 };
inline constexpr TEX_COLOR_RGB RedPixelColorRGB    = { 255, 0, 0 };
inline constexpr TEX_COLOR_RGB GreenPixelColorRGB  = { 0, 255, 0 };
inline constexpr TEX_COLOR_RGB BluePixelColorRGB   = { 0, 0, 255 };
inline constexpr TEX_COLOR_RGB WhitePixelColorRGB  = { 255, 255, 255 };

} // namespace RenderConstants
