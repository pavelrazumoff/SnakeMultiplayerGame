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
	GamePropertiesInfo gameProps;
	gameProps.GameName = "Snake Game";

	GameEngine gameEngine(gameProps);

	StartupLevel* openLevel = CreateNewObject<StartupLevel>();
	gameEngine.Initialization(openLevel);
	gameEngine.Run();

	return 0;
}
