#include "pch.h"
#include "RenderTextureLibrary.h"

#include <cmath>

void RenderTextureLibrary::FillTexture(RCTexture* texture, TEX_PIXEL c)
{
	if (!texture) return;

	TEX_COORD texCoords = { 0, 0 };
	TEX_SIZE texSize = { texture->GetWidth(), texture->GetHeight() };
	FillTexture(texture, c, texCoords, texSize);
}

void RenderTextureLibrary::FillTexture(RCTexture* texture, TEX_PIXEL c, const TEX_COORD& dstCoords, const TEX_SIZE& dstSize)
{
	if (!texture) return;

	const RC_UINT bottomY = dstCoords.y + dstSize.cy;
	for (RC_UINT posY = dstCoords.y; posY < bottomY; posY++)
	{
		TEX_COORD lineCoords = { dstCoords.x, posY };
		texture->FillLineWithPixel(c, lineCoords, dstSize.cx);
	}
}

void RenderTextureLibrary::FillTextureColor(RCTexture* texture, const TEX_COLOR_RGB& c)
{
	if (!texture) return;

	TEX_COORD texCoords = { 0, 0 };
	TEX_SIZE texSize = { texture->GetWidth(), texture->GetHeight() };
	FillTextureColor(texture, c, texCoords, texSize);
}

void RenderTextureLibrary::FillTextureColor(RCTexture* texture, const TEX_COLOR_RGB& c, const TEX_COORD& dstCoords, const TEX_SIZE& dstSize)
{
	if (!texture) return;

	const RC_UINT bottomY = dstCoords.y + dstSize.cy;
	for (RC_UINT posY = dstCoords.y; posY < bottomY; posY++)
	{
		TEX_COORD lineCoords = { dstCoords.x, posY };
		texture->FillLineWithColor(c, lineCoords, dstSize.cx);
	}
}

void RenderTextureLibrary::FillTexture(RCTexture* texture, const RCTexture* srcTexture, const TEX_COORD& dstCoords)
{
	if (!texture || !srcTexture) return;

	TEX_RECT srcRect = { 0, 0, srcTexture->GetWidth(), srcTexture->GetHeight() };
	FillTexture(texture, srcTexture, srcRect, dstCoords);
}

void RenderTextureLibrary::FillTexture(RCTexture* texture, const RCTexture* srcTexture, const TEX_RECT& srcRect, const TEX_COORD& dstCoords)
{
	if (!texture || !srcTexture) return;

	FillTexture(texture, srcTexture->GetData(), srcRect, srcTexture->GetWidth(), dstCoords);
	FillTextureColor(texture, srcTexture->GetColorData(), srcRect, srcTexture->GetWidth(), dstCoords);
}

void RenderTextureLibrary::FillTexture(RCTexture* texture, const TEX_PIXEL* srcData, const TEX_RECT& srcRect, RC_UINT srcStride, const TEX_COORD& dstCoords)
{
	if (!texture || !srcData) return;

	const RC_UINT srcWidth = GetTexWidth(srcRect);
	const RC_UINT srcHeight = GetTexHeight(srcRect);

	for (RC_UINT srcPosY = 0; srcPosY < srcHeight; ++srcPosY)
	{
		for (RC_UINT srcPosX = 0; srcPosX < srcWidth; ++srcPosX)
		{
			TEX_COORD srcTexCoords = { static_cast<RC_UINT>(srcRect.left + srcPosX), static_cast<RC_UINT>(srcRect.top + srcPosY) };
			TEX_COORD dstTexCoords = { static_cast<RC_UINT>(dstCoords.x + srcPosX), static_cast<RC_UINT>(dstCoords.y + srcPosY) };

			TEX_PIXEL srcPixel = srcData[srcTexCoords.y * srcStride + srcTexCoords.x];
			texture->SetPixel(srcPixel, dstTexCoords);
		}
	}
}

void RenderTextureLibrary::FillTextureColor(RCTexture* texture, const TEX_COLOR_RGB* srcColorData, const TEX_RECT& srcRect, RC_UINT srcStride, const TEX_COORD& dstCoords)
{
	if (!texture || !srcColorData) return;

	const RC_UINT srcWidth = GetTexWidth(srcRect);
	const RC_UINT srcHeight = GetTexHeight(srcRect);

	for (RC_UINT srcPosY = 0; srcPosY < srcHeight; ++srcPosY)
	{
		for (RC_UINT srcPosX = 0; srcPosX < srcWidth; ++srcPosX)
		{
			TEX_COORD srcTexCoords = { static_cast<RC_UINT>(srcRect.left + srcPosX), static_cast<RC_UINT>(srcRect.top + srcPosY) };
			TEX_COORD dstTexCoords = { static_cast<RC_UINT>(dstCoords.x + srcPosX), static_cast<RC_UINT>(dstCoords.y + srcPosY) };

			TEX_COLOR_RGB srcColor = srcColorData[srcTexCoords.y * srcStride + srcTexCoords.x];
			texture->SetColor(srcColor, dstTexCoords);
		}
	}
}

RCTexture* RenderTextureLibrary::StretchTexture(const RCTexture* srcTexture, const TEX_SIZE& newSize)
{
	RCTexture* newTexture = new RCTexture(newSize.cx, newSize.cy);

	const float widthStep = (float)newSize.cx / (float)srcTexture->GetWidth();
	const float heightStep = (float)newSize.cy / (float)srcTexture->GetHeight();

	for (RC_UINT dstY = 0; dstY < newSize.cy; ++dstY)
	{
		for (RC_UINT dstX = 0; dstX < newSize.cx; ++dstX)
		{
			RC_UINT srcPixel = (RC_UINT)(round((float)dstY / heightStep)) * srcTexture->GetWidth() +
				(RC_UINT)(round((float)dstX / widthStep));
			newTexture->SetPixel(srcTexture->GetData()[srcPixel], { dstX, dstY });
			newTexture->SetColor(srcTexture->GetColorData()[srcPixel], { dstX, dstY });
		}
	}

	return newTexture;
}
