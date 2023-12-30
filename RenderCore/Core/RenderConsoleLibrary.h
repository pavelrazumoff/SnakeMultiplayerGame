#pragma once

#include "RenderCore.h"
#include "RCDataTypes.h"

#define USE_VIRTUAL_TERMINAL_PROCESSING() 1

class RENDERCORE_API RenderConsoleLibrary
{
public:
	static RC_SIZE GetConsoleDimensions();

	static void FillConsoleRegion(TEX_PIXEL cCharacter, RC_INT x, RC_INT y, RC_UINT width, RC_UINT height);
	static void FillConsoleLineWithBlock(const TEX_PIXEL* cBlock, RC_INT x, RC_INT y, RC_UINT blockLen);

	static void ClearConsoleRegion(RC_INT x, RC_INT y, RC_UINT width, RC_UINT height);
	static void RestoreConsoleRenderMode();

	#if USE_VIRTUAL_TERMINAL_PROCESSING()
	static void EnableVirtualTerminalProcessing();

	static void SetConsoleCaption(const char* caption);

	static void SetCursorPosition(RC_INT x, RC_INT y);
	static void ShowCursor(bool bShow);
	static void SetTextScrollingMargins(RC_INT top, RC_INT bottom);

	static void SetPixelRenderColor(TEX_COLOR_RGB rgb);
	static void SetBackgroundRenderColor(TEX_COLOR_RGB rgb);
	#else // USE_VIRTUAL_TERMINAL_PROCESSING()
	static void SetCursorPosition(RC_INT x, RC_INT y);

	static void FillConsoleRegionColor(TEX_COLOR cColor, RC_INT x, RC_INT y, RC_UINT width, RC_UINT height);
	static void FillConsoleLineWithColorBlock(const TEX_COLOR* cColor, RC_INT x, RC_INT y, RC_UINT blockLen);
	#endif // USE_VIRTUAL_TERMINAL_PROCESSING()
};
