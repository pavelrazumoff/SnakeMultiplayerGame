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

	for (RC_UINT i = 0; i < height; ++i)
	{
		RenderConsoleLibrary::SetCursorPosition(x, y + i);
		for (RC_UINT j = 0; j < width; ++j)
		{
			std::cout << cCharacter;
		}
		//std::cout << "\x1B[E";
	}

	std::cout.flush();
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

	RenderConsoleLibrary::SetCursorPosition(x, y);
	for (RC_UINT i = 0; i < blockLen; ++i)
	{
		std::cout << cBlock[i];
	}

	std::cout.flush();

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
	std::cout << "\x1B]0;" << caption << "\x07";
	std::cout.flush();
}

void RenderConsoleLibrary::SetCursorPosition(RC_INT x, RC_INT y)
{
	std::cout << "\x1B[" << y + 1 << ";" << x + 1 << "H";
	std::cout.flush();
}

void RenderConsoleLibrary::ShowCursor(bool bShow)
{
	std::cout << "\x1B[?25" << (bShow ? "h" : "l");
	std::cout.flush();
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

	std::cout << "\x1B[" << top << ";" << bottom << "r";
	std::cout.flush();
}

void RenderConsoleLibrary::SetPixelRenderColor(TEX_COLOR_RGB rgb)
{
	std::cout << "\x1B[38;2;" << static_cast<int>(rgb.r) << ";" << static_cast<int>(rgb.g) << ";" << static_cast<int>(rgb.b) << "m";
	std::cout.flush();
}

void RenderConsoleLibrary::SetBackgroundRenderColor(TEX_COLOR_RGB rgb)
{
	std::cout << "\x1B[48;2;" << static_cast<int>(rgb.r) << ";" << static_cast<int>(rgb.g) << ";" << static_cast<int>(rgb.b) << "m";
	std::cout.flush();
}
#endif // USE_VIRTUAL_TERMINAL_PROCESSING()
