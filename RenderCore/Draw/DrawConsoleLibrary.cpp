#include "pch.h"
#include "DrawConsoleLibrary.h"

#include "Core/RenderConsoleLibrary.h"

void DrawConsoleLibrary::ClearScreen()
{
	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
	RenderConsoleLibrary::ClearConsoleRegion(0, 0, consoleDim.cx, consoleDim.cy);
}

void DrawConsoleLibrary::SetCursorToTopLeft()
{
	RenderConsoleLibrary::SetCursorPosition(0, 0);
}

void DrawConsoleLibrary::SetCursorToBottomLeft()
{
	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
	RenderConsoleLibrary::SetCursorPosition(0, consoleDim.cy);
}

void DrawConsoleLibrary::SetCursorToBottomRight()
{
	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
	RenderConsoleLibrary::SetCursorPosition(consoleDim.cx, consoleDim.cy);
}

void DrawConsoleLibrary::DrawTexture(const RCTexture* texture, RC_INT x, RC_INT y)
{
	if (!texture) return;

	RenderConsoleLibrary::ClearConsoleRegion(x, y, texture->GetWidth(), texture->GetHeight());

	const RC_UINT texWidth = texture->GetWidth();
	for (RC_UINT i = 0; i < texture->GetHeight(); ++i)
	{
		const TEX_PIXEL* pTextureData = texture->GetData() + i * texWidth;
		RenderConsoleLibrary::FillConsoleLineWithBlock(pTextureData, x, y + i, texWidth);
	}
}

bool DrawConsoleLibrary::DrawTextureDifference(const RCTexture* texture, const RCTexture* prevTexture, RC_INT x, RC_INT y)
{
	if (!texture || !prevTexture ||
		(texture->GetWidth() != prevTexture->GetWidth()) ||
		(texture->GetHeight() != prevTexture->GetHeight())) return false;

	bool bFoundAnyDifference = false;

	const RC_UINT texWidth = texture->GetWidth();
	const RC_UINT texHeight = texture->GetHeight();
	for (RC_UINT i = 0; i < texHeight; ++i)
	{
		const TEX_PIXEL* pTextureData = texture->GetData() + i * texWidth;
		const TEX_COLOR_RGB* pTextureColorData = texture->GetColorData() + i * texWidth;

		const TEX_PIXEL* pPrevTextureData = prevTexture->GetData() + i * texWidth;
		const TEX_COLOR_RGB* pPrevTextureColorData = prevTexture->GetColorData() + i * texWidth;

		int startPixelPosX = -1;

		for (RC_UINT j = 0; j < texWidth; ++j)
		{
			const bool bPixelIsSameWithPrevFrame = pTextureData[j] == pPrevTextureData[j];
			const bool bColorIsSameWithPrevFrame = pTextureColorData[j] == pPrevTextureColorData[j];
			const bool bColorIsSameWithPrevPixel = (j > 0) ? (pTextureColorData[j] == pTextureColorData[j - 1]) : false;

			if (!bColorIsSameWithPrevPixel && (!bColorIsSameWithPrevFrame || startPixelPosX != -1))
			{
				if (startPixelPosX != -1)
				{
					RenderConsoleLibrary::SetPixelRenderColor(pTextureColorData[startPixelPosX]);
					RenderConsoleLibrary::FillConsoleLineWithBlock(pTextureData + startPixelPosX, x + startPixelPosX, y + i, j - startPixelPosX);
					startPixelPosX = -1;

					bFoundAnyDifference = true;
				}

				if (startPixelPosX == -1) startPixelPosX = j;
				continue;
			}

			if (bPixelIsSameWithPrevFrame && bColorIsSameWithPrevFrame)
			{
				if (startPixelPosX != -1)
				{
					RenderConsoleLibrary::SetPixelRenderColor(pTextureColorData[startPixelPosX]);
					RenderConsoleLibrary::FillConsoleLineWithBlock(pTextureData + startPixelPosX, x + startPixelPosX, y + i, j - startPixelPosX);
					startPixelPosX = -1;

					bFoundAnyDifference = true;
				}
			}
			else if (startPixelPosX == -1) startPixelPosX = j;
		}

		if (startPixelPosX != -1)
		{
			RenderConsoleLibrary::SetPixelRenderColor(pTextureColorData[startPixelPosX]);
			RenderConsoleLibrary::FillConsoleLineWithBlock(pTextureData + startPixelPosX, x + startPixelPosX, y + i, texWidth - startPixelPosX);

			bFoundAnyDifference = true;
		}
	}

	return bFoundAnyDifference;
}
