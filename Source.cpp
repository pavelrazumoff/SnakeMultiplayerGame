#include <iostream>
#include <vld.h>
#include <memory>
#include <Windows.h>

#include "Core/RenderConsoleLibrary.h"
#include "Core/RenderTextureLibrary.h"
#include "Draw/DrawConsoleLibrary.h"

#include "Engine/GameEngine.h"
#include "Engine/GameObject/GameObjectUtility.h"

#include "Engine/EngineFactory.h"
#include "Engine/Player/PlayerManager.h"

#include "SnakePlayer/SnakePlayerState.h"

#include "SnakeLevels/StartupLevel.h"

int main()
{
	GamePropertiesInfo gameProps;
	gameProps.GameName = "Snake Game";

	// TODO: Move out to the GameMode analogue class.
	EngineFactory::GetInstance().RegisterType("SnakePlayerState", new SnakePlayerState());
	PlayerManager::GetInstance().SetPlayerStateClass("SnakePlayerState");

	GameEngine gameEngine(gameProps);

	StartupLevel* openLevel = CreateNewObject<StartupLevel>();
	gameEngine.Initialization(openLevel);
	gameEngine.Run();

	return 0;
}
