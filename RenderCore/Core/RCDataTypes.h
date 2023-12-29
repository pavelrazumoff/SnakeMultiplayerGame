#pragma once

#include "stdint.h"

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

namespace RenderConstants {

inline constexpr TEX_PIXEL ClearPixel = ' ';
inline constexpr TEX_PIXEL AlphaZeroPixel = 0x00;

} // namespace RenderConstants
