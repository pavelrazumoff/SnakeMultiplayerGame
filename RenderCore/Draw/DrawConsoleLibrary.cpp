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

void DrawConsoleLibrary::SetCursorToBottom()
{
	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
	RenderConsoleLibrary::SetCursorPosition(0, consoleDim.cy - 1);
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

	bool bWasDifference = false;

	const RC_UINT texWidth = texture->GetWidth();
	const RC_UINT texHeight = texture->GetHeight();
	for (RC_UINT i = 0; i < texHeight; ++i)
	{
		const TEX_PIXEL* pTextureData = texture->GetData() + i * texWidth;
		const TEX_PIXEL* pPrevTextureData = prevTexture->GetData() + i * texWidth;

		int startPosX = -1;
		for (RC_UINT j = 0; j < texWidth; ++j)
		{
			if (pTextureData[j] == pPrevTextureData[j])
			{
				if (startPosX != -1)
				{
					RenderConsoleLibrary::FillConsoleLineWithBlock(pTextureData + startPosX, x + startPosX, y + i, j - startPosX);
					startPosX = -1;

					bWasDifference = true;
				}
			}
			else if (startPosX == -1) startPosX = j;
		}

		if (startPosX != -1)
		{
			RenderConsoleLibrary::FillConsoleLineWithBlock(pTextureData + startPosX, x + startPosX, y + i, texWidth - startPosX);
			bWasDifference = true;
		}
	}

	return bWasDifference;
}
