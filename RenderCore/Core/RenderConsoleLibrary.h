#pragma once

#include "RenderCore.h"

#include "RCDataTypes.h"

class RENDERCORE_API RenderConsoleLibrary
{
public:
	static void SetCursorPosition(RC_INT x, RC_INT y);
	static RC_SIZE GetConsoleDimensions();

	static void FillConsoleRegion(BYTE cCharacter, RC_INT x, RC_INT y, RC_UINT width, RC_UINT height);
	static void FillConsoleLineWithBlock(const BYTE* cBlock, RC_INT x, RC_INT y, RC_UINT blockLen);

	static void ClearConsoleRegion(RC_INT x, RC_INT y, RC_UINT width, RC_UINT height);
};
