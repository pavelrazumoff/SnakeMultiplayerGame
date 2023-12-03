#include "pch.h"
#include "RenderTextureLibrary.h"

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
