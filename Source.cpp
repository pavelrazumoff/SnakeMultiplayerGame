#include <iostream>
#include <vld.h>
#include <memory>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <Windows.h>

#include "Core/RenderConsoleLibrary.h"
#include "Core/RenderTextureLibrary.h"
#include "Draw/DrawConsoleLibrary.h"

#include "Engine/GameEngine.h"
#include "Engine/GameObject/GameObjectUtility.h"

#include "Engine/EngineFactory.h"
#include "Engine/Player/PlayerManager.h"
#include "Engine/Other/ProfilerManager.h"

#include "SnakePlayer/LobbyPlayerController.h"
#include "SnakePlayer/LobbyPlayerState.h"
#include "SnakePlayer/SnakePlayerState.h"

#include "SnakeObjects/SnakePawn.h"

#include "SnakeLevels/StartupLevel.h"

int main()
{
	GamePropertiesInfo gameProps;
	gameProps.GameName = "Snake Game";

	// TODO: Move out to the GameMode analogue class.
	EngineFactory::GetInstance().RegisterType("LobbyPlayerController", new LobbyPlayerController());
	EngineFactory::GetInstance().RegisterType("LobbyPlayerState", new LobbyPlayerState());
	EngineFactory::GetInstance().RegisterType("SnakePlayerState", new SnakePlayerState());
	EngineFactory::GetInstance().RegisterType("SnakePawn", new SnakePawn());

	ProfilerManager::GetInstance().EnableTrackCategory(ProfilerCategory::FPS, true);

	GameEngine gameEngine(gameProps);

	StartupLevel* openLevel = CreateNewObject<StartupLevel>();
	gameEngine.Initialize(openLevel);
	gameEngine.Run();

	return 0;
}
