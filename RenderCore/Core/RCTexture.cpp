#include "pch.h"
#include "RCTexture.h"

RCTexture::RCTexture(RC_UINT _width, RC_UINT _height)
	: width(_width), height(_height)
{
	data = new TEX_PIXEL[width * height];
	memset(data, ' ', width * height * sizeof(TEX_PIXEL));
}

RCTexture::~RCTexture()
{
	delete[] data;
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
