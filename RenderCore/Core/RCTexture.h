#pragma once

#include "RenderCore.h"
#include "RCDataTypes.h"

class RENDERCORE_API RCTexture
{
public:
	RCTexture(RC_UINT _width, RC_UINT _height);
	~RCTexture();

	void SetPixel(TEX_PIXEL pixel, TEX_COORD texCoord);
	void FillLineWithPixel(TEX_PIXEL pixel, TEX_COORD texCoord, RC_UINT lnWidth);

	RC_UINT GetWidth() const { return width; }
	RC_UINT GetHeight() const { return height; }

	const TEX_PIXEL* GetData() const { return data; }

private:
	TEX_PIXEL* data = nullptr;

	RC_UINT width = 0;
	RC_UINT height = 0;
};
