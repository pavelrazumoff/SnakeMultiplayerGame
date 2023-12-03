#pragma once

#include "RenderCore.h"
#include "Core/RCTexture.h"

class RENDERCORE_API DrawConsoleLibrary
{
public:
	static void ClearScreen();
	static void SetCursorToBottom();

	static void DrawTexture(const RCTexture* texture, RC_INT x, RC_INT y);
};
