#include "pch.h"
#include "RenderConsoleLibrary.h"

#include "Windows.h"

#if USE_VIRTUAL_TERMINAL_PROCESSING()
#include <iostream>
#endif // USE_VIRTUAL_TERMINAL_PROCESSING()

void RenderConsoleLibrary::SetCursorPosition(RC_INT x, RC_INT y)
{
	COORD coordScreen = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordScreen);
}

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

	x += 1; y += 1;
	for (RC_UINT i = 0; i < height; ++i)
	{
		std::cout << "\x1B[" << static_cast<int>(y + i) << ";" << static_cast<int>(x) << "H";

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

	x += 1; y += 1;
	std::cout << "\x1B[" << static_cast<int>(y) << ";" << static_cast<int>(x) << "H";

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
	#if USE_VIRTUAL_TERMINAL_PROCESSING()
	SetPixelRenderColor(RenderConstants::WhitePixelColorRGB);
	SetBackgroundRenderColor(RenderConstants::BlackPixelColorRGB);

	FillConsoleRegion(' ', x, y, width, height);

	#else // USE_VIRTUAL_TERMINAL_PROCESSING()
	FillConsoleRegion(' ', x, y, width, height);
	FillConsoleRegionColor(RenderConstants::WhitePixelColor, x, y, width, height);
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

void RenderConsoleLibrary::ShowCursor(bool bShow)
{
	std::cout << "\x1B[?25" << (bShow ? "h" : "l");
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
