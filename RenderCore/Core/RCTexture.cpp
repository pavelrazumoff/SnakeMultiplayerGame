#include "pch.h"
#include "RCTexture.h"

#include <algorithm>

RCTexture::RCTexture(RC_UINT _width, RC_UINT _height)
	: width(_width), height(_height)
{
	data = new TEX_PIXEL[width * height];
	memset(data, ' ', width * height * sizeof(TEX_PIXEL));

	colorData = new TEX_COLOR_RGB[width * height];
	std::fill_n(colorData, width * height, RenderConstants::WhitePixelColorRGB);
}

RCTexture::~RCTexture()
{
	delete[] data;
	delete[] colorData;
}

RCTexture* RCTexture::CopyTexture() const
{
	RCTexture* copy = new RCTexture(width, height);

	memcpy(copy->data, data, width * height * sizeof(TEX_PIXEL));
	memcpy(copy->colorData, colorData, width * height * sizeof(TEX_COLOR_RGB));

	return copy;
}

void RCTexture::SetPixel(TEX_PIXEL pixel, TEX_COORD texCoord)
{
	if (texCoord.x < width && texCoord.y < height)
		data[texCoord.y * width + texCoord.x] = pixel;
}

void RCTexture::FillLineWithPixel(TEX_PIXEL pixel, TEX_COORD texCoord, RC_UINT lnWidth)
{
	if ((texCoord.x < width) && (texCoord.y < height) && (texCoord.x + lnWidth - 1 < width))
		memset(data + texCoord.y * width + texCoord.x, pixel, lnWidth * sizeof(TEX_PIXEL));
}

void RCTexture::SetColor(TEX_COLOR_RGB color, TEX_COORD texCoord)
{
	if (texCoord.x < width && texCoord.y < height)
		colorData[texCoord.y * width + texCoord.x] = color;
}

void RCTexture::FillLineWithColor(TEX_COLOR_RGB color, TEX_COORD texCoord, RC_UINT lnWidth)
{
	if ((texCoord.x < width) && (texCoord.y < height) && (texCoord.x + lnWidth - 1 < width))
		std::fill_n(colorData + texCoord.y * width + texCoord.x, lnWidth, color);
}
