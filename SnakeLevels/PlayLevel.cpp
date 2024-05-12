#include "PlayLevel.h"

#include "SnakeObjects/SnakePawn.h"
#include "SnakeObjects/Food.h"

#include "SnakeWidgets/HUD/PlayerHUDWidget.h"
#include "SnakeWidgets/Menu/GameMenuWidget.h"
#include "SnakeWidgets/Menu/PlayerLostMenuWidget.h"
#include "SnakePlayer/SnakePlayerState.h"

#include "Engine/Level/LevelManager.h"
#include "Engine/Player/PlayerManager.h"
#include "Engine/GameWidget/GameWidgetManager.h"

#include "Core/RenderConsoleLibrary.h"

#include "Engine/Math/MathLibrary.h"
#include "Engine/Other/TimeManager.h"
#include "Engine/Other/ProfilerManager.h"

#include "Engine/Network/NetworkEngineUtility.h"

PlayLevel::PlayLevel()
{
	PlayerManager::GetInstance().SetPlayerControllerClass("PlayerController"); // TODO.
	PlayerManager::GetInstance().SetPlayerStateClass("SnakePlayerState");
	PlayerManager::GetInstance().SetPlayerPawnClass("SnakePawn");
}

PlayLevel::~PlayLevel()
{
}

void PlayLevel::OpenLevel()
{
	Inherited::OpenLevel();

	FPSCounterTimer = CreateNewObject<TimerHandler>(this);
	if (FPSCounterTimer.IsValid())
	{
		FPSCounterTimer->SetSingleFire(false);
		FPSCounterTimer->SetTimeLimit(0.25f);

		TimeManager::GetInstance().BindTimerHandler(FPSCounterTimer, this, &PlayLevel::UpdateFPSCounter);
		ProfilerManager::GetInstance().OnEngineFeatureChangedEvent().Subscribe(this, &PlayLevel::HandleProfilerEngineFeatureChanged);
	}

	if (NetworkUtility::IsServer())
	{
		RC_SIZE screenDim = RenderConsoleLibrary::GetConsoleDimensions();

		const uint32_t numPlayers = PlayerManager::GetInstance().GetPlayerCount();
		for (uint32_t i = 0; i < numPlayers; ++i)
		{
			PlayerController* pPlayerController = PlayerManager::GetInstance().GetPlayerController(i);
			if (!pPlayerController) { DebugEngineTrap(); continue; }
			
			if (SnakePlayerState* pPlayerState = dynamic_cast<SnakePlayerState*>(pPlayerController->GetPlayerState()))
				pPlayerState->ClearScore();

			// TODO: This depends on the server screen. Change to global.
			LV_COORD startPlayerCoord((float)MathLibrary::GetRandomInRange(0, screenDim.cx),
				(float)MathLibrary::GetRandomInRange(0, screenDim.cy));

			// TODO: Move the pawn creation to game mode class.
			auto pSnakePawn = dynamic_cast<SnakePawn*>(
				PlayerManager::GetInstance().MakePawnForPlayer(pPlayerController, startPlayerCoord));
		}
	}
	else
	{
		if (!PlayerHUD.IsValid())
		{
			PlayerHUD = CreateNewObject<PlayerHUDWidget>(this);

			PlayerHUD->OnMenuOpenClickEvent().Subscribe(this, &PlayLevel::HandleGameMenuOpenClicked);

			GameWidgetManager::GetInstance().PlaceUserWidgetOnScreen(PlayerHUD.Get());
		}

		if (SnakePlayerState* pPlayerState = dynamic_cast<SnakePlayerState*>(PlayerManager::GetInstance().GetPlayerState()))
		{
			pPlayerState->OnScoreUpdatedEvent().Subscribe(this, &PlayLevel::HandleScoreChanged);
			pPlayerState->OnPlayerLostEvent().Subscribe(this, &PlayLevel::HandlePlayerLost);
		}
	}
	
	//LV_COORD startPlayerCoord((float)screenDim.cx / 2.0f - 2.0f, (float)screenDim.cy / 2.0f - 2.0f);
	//pSnakePawn = LevelManager::GetInstance().SpawnSceneObject<SnakePawn>(startPlayerCoord);

	ReconstructLevel();
}

void PlayLevel::CloseLevel()
{
	Inherited::CloseLevel();

	if (SnakePlayerState* pPlayerState = dynamic_cast<SnakePlayerState*>(PlayerManager::GetInstance().GetPlayerState()))
	{
		pPlayerState->OnScoreUpdatedEvent().Unsubscribe(this);
		pPlayerState->OnPlayerLostEvent().Unsubscribe(this);
	}

	GameWidgetManager::GetInstance().RemoveUserWidgetFromScreen(PlayerHUD.Get());
	GameWidgetManager::GetInstance().RemoveUserWidgetFromScreen(PlayerLostMenu.Get());
}

void PlayLevel::Update(float DeltaTime)
{
	Inherited::Update(DeltaTime);

	if (NetworkUtility::IsServer())
	{
		if (SpawnNewFoodTimer >= SpawnNewFoodTimeValue)
		{
			SpawnNewFood();
			SpawnNewFoodTimer = 0.0f;
		}
		else
			SpawnNewFoodTimer += DeltaTime;
	}

	CheckForBoundaries();
}

void PlayLevel::ReconstructLevel()
{
	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
	if (PlayerHUD.IsValid())
	{
		PlayerHUD->SetCanvasDimensions(consoleDim.cx, consoleDim.cy);

		RC_RECT screenRect = PlayerHUD->GetScreenFreeRect();
		playAreaRect = { (float)screenRect.left, (float)screenRect.top, (float)screenRect.right, (float)screenRect.bottom };
	}
	else
		playAreaRect = { 0.0f, 0.0f, (float)consoleDim.cx, (float)consoleDim.cy };

	ReconstructDynamicMenu();
}

void PlayLevel::ReconstructDynamicMenu()
{
	// TODO: Move it to GameWidgetManager.
	if (GameMenu.IsValid())
	{
		RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
		GameMenu->SetCanvasDimensions(consoleDim.cx, consoleDim.cy);
	}

	if (PlayerLostMenu.IsValid())
	{
		RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
		PlayerLostMenu->SetCanvasDimensions(consoleDim.cx, consoleDim.cy);
	}
}

void PlayLevel::SpawnNewFood()
{
	if (!NetworkUtility::IsServer()) return;

	LV_COORD spawnFoodCoord((float)MathLibrary::GetRandomInRange((int)playAreaRect.left, (int)playAreaRect.right),
		(float)MathLibrary::GetRandomInRange((int)playAreaRect.top, (int)playAreaRect.bottom));
	TObjectPtr<FoodObject> newFood = LevelManager::GetInstance().SpawnSceneObject<FoodObject>(spawnFoodCoord);
}

void PlayLevel::CheckForBoundaries()
{
	const uint32_t numPlayers = PlayerManager::GetInstance().GetPlayerCount();
	for (uint32_t i = 0; i < numPlayers; ++i)
	{
		PlayerController* pPlayerController = PlayerManager::GetInstance().GetPlayerController(i);
		if (!pPlayerController) { DebugEngineTrap(); continue; }

		auto pSnakePawn = pPlayerController->GetPlayerPawn();
		if (pSnakePawn)
		{
			LV_COORD snakeLocation = pSnakePawn->GetLocation();
			if (snakeLocation.x <= playAreaRect.left)
				snakeLocation.x = playAreaRect.right;
			else if (snakeLocation.x > playAreaRect.right)
				snakeLocation.x = playAreaRect.left;
			else if (snakeLocation.y <= playAreaRect.top)
				snakeLocation.y = playAreaRect.bottom;
			else if (snakeLocation.y > playAreaRect.bottom)
				snakeLocation.y = playAreaRect.top;

			pSnakePawn->SetLocation(snakeLocation);
		}
	}
}

void PlayLevel::HandleScoreChanged(SnakePlayerState* /*Instigator*/, uint32_t newScore)
{
	if (PlayerHUD.IsValid())
		PlayerHUD->SetScore(newScore);
}

void PlayLevel::HandlePlayerLost(SnakePlayerState* Instigator)
{
	// If the player who has lost is us.
	if (PlayerManager::GetInstance().GetPlayerState() == Instigator)
	{
		LevelManager::GetInstance().PauseGame();

		if (!PlayerLostMenu.IsValid())
		{
			PlayerLostMenu = CreateNewObject<PlayerLostMenuWidget>(this);
			if (PlayerLostMenu.IsValid())
			{
				PlayerLostMenu->OnPlayAgainClickEvent().Subscribe(this, &PlayLevel::HandlePlayAgainClicked);
				PlayerLostMenu->OnExitGameClickEvent().Subscribe(this, &PlayLevel::HandleExitGameClicked);
			}

			GameWidgetManager::GetInstance().PlaceUserWidgetOnScreen(PlayerLostMenu.Get());
			ReconstructDynamicMenu();
		}
	}
}

void PlayLevel::HandleGameMenuOpenClicked()
{
	LevelManager::GetInstance().PauseGame();

	// TODO: Block the underlying HUD widget from getting input.
	if (!GameMenu.IsValid())
	{
		GameMenu = CreateNewObject<GameMenuWidget>(this);
		if (GameMenu.IsValid())
		{
			GameMenu->OnResumeClickEvent().Subscribe(this, &PlayLevel::HandleResumeGameClicked);
			GameMenu->OnExitGameClickEvent().Subscribe(this, &PlayLevel::HandleExitGameClicked);
		}

		GameWidgetManager::GetInstance().PlaceUserWidgetOnScreen(GameMenu.Get());
		ReconstructDynamicMenu();
	}
}

void PlayLevel::HandleResumeGameClicked()
{
	LevelManager::GetInstance().ResumeGame();

	GameWidgetManager::GetInstance().RemoveUserWidgetFromScreen(GameMenu.Get());
	GameMenu = nullptr;
}

void PlayLevel::HandlePlayAgainClicked()
{
	LevelManager::GetInstance().ResumeGame();

	PlayLevel* playLevel = CreateNewObject<PlayLevel>();
	LevelManager::GetInstance().OpenLevel(playLevel);
}

void PlayLevel::HandleExitGameClicked()
{
	LevelManager::GetInstance().CloseLevel(this);
}

void PlayLevel::UpdateFPSCounter()
{
	if (PlayerHUD.IsValid())
		PlayerHUD->SetFPSCounter((uint32_t)ProfilerManager::GetInstance().GetAverageFPS());
}

void PlayLevel::HandleProfilerEngineFeatureChanged(ProfilerEngineFeature /*feature*/, bool /*bEnabled*/)
{
	if (!PlayerHUD.IsValid()) return;

	uint64_t disabledFeaturesMask = ProfilerManager::GetInstance().GetEngineDisabledFeatureMask();
	std::string profilerInfoText;

	auto ADD_PROFILER_STR = [&profilerInfoText](const char* str) -> void {
		if (profilerInfoText.empty())
			profilerInfoText += str;
		else
			profilerInfoText += ", " + std::string(str);
	};

	if (disabledFeaturesMask & ENUM2BIT(ProfilerEngineFeature::CollisionDetection))
		ADD_PROFILER_STR("Collision: Disabled");
	if (disabledFeaturesMask & ENUM2BIT(ProfilerEngineFeature::RenderScene))
		ADD_PROFILER_STR("Render Scene: Disabled");
	if (disabledFeaturesMask & ENUM2BIT(ProfilerEngineFeature::RenderWidgets))
		ADD_PROFILER_STR(/*"Widgets: Disabled"*/""); // Anyway we won't see anything.
	if (disabledFeaturesMask & ENUM2BIT(ProfilerEngineFeature::UpdateScene))
		ADD_PROFILER_STR("Update Scene: Disabled");

	PlayerHUD->SetProfilerInfoText(profilerInfoText.c_str());
}
