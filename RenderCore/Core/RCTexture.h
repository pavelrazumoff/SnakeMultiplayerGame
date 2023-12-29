#pragma once

#include "RenderCore.h"
#include "RCDataTypes.h"

class RENDERCORE_API RCTexture
{
public:
	RCTexture(RC_UINT _width, RC_UINT _height);
	~RCTexture();

	[[nodiscard]] RCTexture* CopyTexture() const;

	void SetPixel(TEX_PIXEL pixel, TEX_COORD texCoord);
	void FillLineWithPixel(TEX_PIXEL pixel, TEX_COORD texCoord, RC_UINT lnWidth);

	void SetColor(TEX_COLOR_RGB color, TEX_COORD texCoord);
	void FillLineWithColor(TEX_COLOR_RGB color, TEX_COORD texCoord, RC_UINT lnWidth);

	RC_UINT GetWidth() const { return width; }
	RC_UINT GetHeight() const { return height; }

	const TEX_PIXEL* GetData() const { return data; }
	const TEX_COLOR_RGB* GetColorData() const { return colorData; }

private:
	TEX_PIXEL* data = nullptr;
	TEX_COLOR_RGB* colorData = nullptr;

	RC_UINT width = 0;
	RC_UINT height = 0;
};
