#include <iostream>
#include <vld.h>
#include <memory>
#include <Windows.h>

#include "Core/RenderConsoleLibrary.h"
#include "Core/RenderTextureLibrary.h"
#include "Draw/DrawConsoleLibrary.h"

#include "Engine/GameEngine.h"
#include "Engine/GameObject/GameObjectUtility.h"

#include "SnakeLevels/StartupLevel.h"

int main()
{
	#if 0
	std::shared_ptr<RCTexture> testTex = std::make_shared<RCTexture>(8, 8);
	#if 0
	RenderTextureLibrary::FillTexture(testTex.get(), '0');
	#elif 1
	{
		const TEX_PIXEL texData[] =
			"* *"
			" * "
			"* *";

		TEX_RECT rect = { 0, 0, 3, 3 };
		TEX_COORD coord = { 2, 2 };
		RenderTextureLibrary::FillTexture(testTex.get(), texData, rect, GetTexWidth(rect), coord);
	}

	{
		const TEX_PIXEL texData[] = "--------";

		TEX_RECT rect = { 0, 0, 8, 1 };
		TEX_COORD coord = { 0, 0 };
		RenderTextureLibrary::FillTexture(testTex.get(), texData, rect, GetTexWidth(rect), coord);

		coord = { 0, 7 };
		RenderTextureLibrary::FillTexture(testTex.get(), texData, rect, GetTexWidth(rect), coord);
	}
	#endif

	DrawConsoleLibrary::ClearScreen();

	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
	RenderConsoleLibrary::FillConsoleRegion('*', 0, 1, consoleDim.cx, 1);

	DrawConsoleLibrary::DrawTexture(testTex.get(), 3, 3);

	DrawConsoleLibrary::SetCursorToBottom();
	#endif

	GameEngine gameEngine;

	StartupLevel* openLevel = CreateNewObject<StartupLevel>();
	gameEngine.Initialization(openLevel);
	gameEngine.Run();

	return 0;
}
