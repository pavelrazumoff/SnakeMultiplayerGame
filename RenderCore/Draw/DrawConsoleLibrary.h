#pragma once

#include "RenderCore.h"
#include "Core/RCTexture.h"

class RENDERCORE_API DrawConsoleLibrary
{
public:
	static void ClearScreen();

	static void SetCursorToTopLeft();
	static void SetCursorToBottomLeft();
	static void SetCursorToBottomRight();

	static void DrawTexture(const RCTexture* texture, RC_INT x, RC_INT y);
	static bool DrawTextureDifference(const RCTexture* texture, const RCTexture* prevTexture, RC_INT x, RC_INT y);
};
