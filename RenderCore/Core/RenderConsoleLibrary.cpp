#include "pch.h"
#include "RenderConsoleLibrary.h"

#include "Windows.h"

#if USE_VIRTUAL_TERMINAL_PROCESSING()
/*
	Useful links:
	1. https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
	2. https://invisible-island.net/xterm/ctlseqs/ctlseqs.html
*/

#include <iostream>
#endif // USE_VIRTUAL_TERMINAL_PROCESSING()

RC_SIZE RenderConsoleLibrary::GetConsoleDimensions()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

	RC_SIZE consoleSize;
	consoleSize.cx = static_cast<RC_UINT>(csbi.srWindow.Right - csbi.srWindow.Left + 1);
	consoleSize.cy = static_cast<RC_UINT>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);

	return consoleSize;
}

void RenderConsoleLibrary::FillConsoleRegion(TEX_PIXEL cCharacter, RC_INT x, RC_INT y, RC_UINT width, RC_UINT height)
{
	#if USE_VIRTUAL_TERMINAL_PROCESSING()

	std::string buff;
	for (RC_UINT i = 0; i < height; ++i)
	{
		buff += GetCursorPosition(x, y + i);
		buff += std::string(width, cCharacter);
	}

	printf("%s", buff.c_str());

	#else
	COORD coordScreen = { x, y };
	DWORD cCharsWritten;

	for (RC_UINT i = 0; i < height; ++i)
	{
		FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), cCharacter, width, coordScreen, &cCharsWritten);
		coordScreen.Y++;
	}
	#endif // USE_VIRTUAL_TERMINAL_PROCESSING()
}

void RenderConsoleLibrary::FillConsoleLineWithBlock(const TEX_PIXEL* cBlock, RC_INT x, RC_INT y, RC_UINT blockLen)
{
	#if USE_VIRTUAL_TERMINAL_PROCESSING()

	std::string buff = GetCursorPosition(x, y);

	buff += std::string(cBlock, cBlock + blockLen);
	printf("%s", buff.c_str());

	#else // USE_VIRTUAL_TERMINAL_PROCESSING()
	COORD coordScreen = { x, y };
	for (RC_UINT i = 0; i < blockLen; ++i)
	{
		DWORD cCharsWritten;
		FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), cBlock[i], 1, coordScreen, &cCharsWritten);
		coordScreen.X++;
	}
	#endif // USE_VIRTUAL_TERMINAL_PROCESSING()
}

#if USE_VIRTUAL_TERMINAL_PROCESSING()
#else // USE_VIRTUAL_TERMINAL_PROCESSING()
void RenderConsoleLibrary::SetCursorPosition(RC_INT x, RC_INT y)
{
	COORD coordScreen = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordScreen);
}

void RenderConsoleLibrary::FillConsoleRegionColor(TEX_COLOR cColor, RC_INT x, RC_INT y, RC_UINT width, RC_UINT height)
{
	COORD coordScreen = { x, y };
	DWORD cAttrsWritten;

	for (RC_UINT i = 0; i < height; ++i)
	{
		FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cColor, width, coordScreen, &cAttrsWritten);
		coordScreen.Y++;
	}
}

void RenderConsoleLibrary::FillConsoleLineWithColorBlock(const TEX_COLOR* cColor, RC_INT x, RC_INT y, RC_UINT blockLen)
{
	COORD coordScreen = { x, y };
	for (RC_UINT i = 0; i < blockLen; ++i)
	{
		DWORD cAttrsWritten;
		FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cColor[i], 1, coordScreen, &cAttrsWritten);
		coordScreen.X++;
	}
}
#endif // USE_VIRTUAL_TERMINAL_PROCESSING()

void RenderConsoleLibrary::ClearConsoleRegion(RC_INT x, RC_INT y, RC_UINT width, RC_UINT height)
{
	RestoreConsoleRenderMode();
	#if USE_VIRTUAL_TERMINAL_PROCESSING()
	FillConsoleRegion(' ', x, y, width, height);

	#else // USE_VIRTUAL_TERMINAL_PROCESSING()
	FillConsoleRegion(' ', x, y, width, height);
	FillConsoleRegionColor(RenderConstants::WhitePixelColor, x, y, width, height);
	#endif // USE_VIRTUAL_TERMINAL_PROCESSING()
}

void RenderConsoleLibrary::RestoreConsoleRenderMode()
{
	#if USE_VIRTUAL_TERMINAL_PROCESSING()
	SetPixelRenderColor(RenderConstants::WhitePixelColorRGB);
	SetBackgroundRenderColor(RenderConstants::BlackPixelColorRGB);
	#endif // USE_VIRTUAL_TERMINAL_PROCESSING()
}

#if USE_VIRTUAL_TERMINAL_PROCESSING()

void RenderConsoleLibrary::EnableVirtualTerminalProcessing()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) return;
	
	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) return;
	
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) return;
}

void RenderConsoleLibrary::SetConsoleCaption(const char* caption)
{
	printf("\x1B]0;%s\x07", caption);
}

void RenderConsoleLibrary::SetCursorPosition(RC_INT x, RC_INT y)
{
	printf("%s", GetCursorPosition(x, y).c_str());
}

std::string RenderConsoleLibrary::GetCursorPosition(RC_INT x, RC_INT y)
{
	return std::string("\x1B[" + std::to_string(y + 1) + ";" + std::to_string(x + 1) + "H");
}

void RenderConsoleLibrary::ShowCursor(bool bShow)
{
	printf("\x1B[?25%c", bShow ? 'h' : 'l');
}

void RenderConsoleLibrary::SetTextScrollingMargins(RC_INT top, RC_INT bottom)
{
	/*
		In the context of the Virtual Terminal Processing feature in Windows Console, "scroll" refers to the movement of 
		text lines up or down the console window.
		When new text is written to the console and it reaches the bottom of the window, the existing lines of text move up
		to make room for the new text. This movement of text is referred to as "scrolling".

		So, when new text is written to the console, and it reaches the bottom margin of the set scrolling region,
		the existing lines of text within this region move up to make room for the new text. 
		The text outside this region (above the top margin and below the bottom margin) remains static.
	*/

	printf("\x1B[%d;%dr", top, bottom);
}

void RenderConsoleLibrary::SetPixelRenderColor(TEX_COLOR_RGB rgb)
{
	printf("\x1B[38;2;%d;%d;%dm", static_cast<int>(rgb.r), static_cast<int>(rgb.g), static_cast<int>(rgb.b));
}

void RenderConsoleLibrary::SetBackgroundRenderColor(TEX_COLOR_RGB rgb)
{
	printf("\x1B[48;2;%d;%d;%dm", static_cast<int>(rgb.r), static_cast<int>(rgb.g), static_cast<int>(rgb.b));
}
#endif // USE_VIRTUAL_TERMINAL_PROCESSING()
