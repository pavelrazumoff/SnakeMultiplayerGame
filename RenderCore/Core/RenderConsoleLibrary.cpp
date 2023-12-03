#include "pch.h"
#include "RenderConsoleLibrary.h"

#include "Windows.h"

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

void RenderConsoleLibrary::FillConsoleRegion(BYTE cCharacter, RC_INT x, RC_INT y, RC_UINT width, RC_UINT height)
{
	COORD coordScreen = { x, y };
	DWORD cCharsWritten;

	for (RC_UINT i = 0; i < height; ++i)
	{
		FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), cCharacter, width, coordScreen, &cCharsWritten);
		coordScreen.Y++;
	}
}

void RenderConsoleLibrary::FillConsoleLineWithBlock(const BYTE* cBlock, RC_INT x, RC_INT y, RC_UINT blockLen)
{
	COORD coordScreen = { x, y };
	for (RC_UINT i = 0; i < blockLen; ++i)
	{
		DWORD cCharsWritten;
		FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), cBlock[i], 1, coordScreen, &cCharsWritten);
		coordScreen.X++;
	}
}

void RenderConsoleLibrary::ClearConsoleRegion(RC_INT x, RC_INT y, RC_UINT width, RC_UINT height)
{
	FillConsoleRegion(' ', x, y, width, height);
}
