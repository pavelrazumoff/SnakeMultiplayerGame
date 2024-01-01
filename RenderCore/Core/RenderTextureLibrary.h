#pragma once

#include "RenderCore.h"
#include "RCTexture.h"

#include "stdint.h"

class RENDERCORE_API RenderTextureLibrary
{
public:
	static void FillTexture(RCTexture* texture, TEX_PIXEL c);
	static void FillTexture(RCTexture* texture, TEX_PIXEL c, const TEX_COORD& dstCoords, const TEX_SIZE& dstSize);

	static void FillTextureColor(RCTexture* texture, const TEX_COLOR_RGB& c);
	static void FillTextureColor(RCTexture* texture, const TEX_COLOR_RGB& c, const TEX_COORD& dstCoords, const TEX_SIZE& dstSize);

	static void FillTexture(RCTexture* texture, const RCTexture* srcTexture, const TEX_COORD& dstCoords);
	static void FillTexture(RCTexture* texture, const RCTexture* srcTexture, const TEX_RECT& srcRect, const TEX_COORD& dstCoords);

	static void FillTexture(RCTexture* texture, const TEX_PIXEL* srcData, const TEX_RECT& srcRect, RC_UINT srcStride, const TEX_COORD& dstCoords);
	static void FillTextureColor(RCTexture* texture, const TEX_PIXEL* srcPixelData, const TEX_COLOR_RGB* srcColorData, const TEX_RECT& srcRect, RC_UINT srcStride, const TEX_COORD& dstCoords);

	static RCTexture* StretchTexture(const RCTexture* srcTexture, const TEX_SIZE& newSize);
};
