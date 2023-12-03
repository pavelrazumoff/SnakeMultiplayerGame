#include "pch.h"
#include "DrawConsoleLibrary.h"

#include "Core/RenderConsoleLibrary.h"

void DrawConsoleLibrary::ClearScreen()
{
	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
	RenderConsoleLibrary::ClearConsoleRegion(0, 0, consoleDim.cx, consoleDim.cy);
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
