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

PlayLevel::PlayLevel()
{
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
	}

	RC_SIZE screenDim = RenderConsoleLibrary::GetConsoleDimensions();

	LV_COORD startPlayerCoord((float)screenDim.cx / 2.0f - 2.0f, (float)screenDim.cy / 2.0f - 2.0f);
	pSnakePawn = LevelManager::GetInstance().SpawnSceneObject<SnakePawn>(startPlayerCoord);

	if (!PlayerHUD.IsValid())
	{
		PlayerHUD = CreateNewObject<PlayerHUDWidget>(this);

		PlayerHUD->OnMenuOpenClickEvent().Subscribe(this, &PlayLevel::HandleGameMenuOpenClicked);

		GameWidgetManager::GetInstance().PlaceUserWidgetOnScreen(PlayerHUD.Get());
	}

	if (SnakePlayerState* pPlayerState = dynamic_cast<SnakePlayerState*>(PlayerManager::GetInstance().GetPlayerState()))
	{
		pPlayerState->ClearScore();

		pPlayerState->OnScoreUpdatedEvent().Subscribe(this, &PlayLevel::HandleScoreChanged);
		pPlayerState->OnPlayerLostEvent().Subscribe(this, &PlayLevel::HandlePlayerLost);
	}

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

	if (SpawnNewFoodTimer >= SpawnNewFoodTimeValue)
	{
		SpawnNewFood();
		SpawnNewFoodTimer = 0.0f;
	}
	else
		SpawnNewFoodTimer += DeltaTime;

	CheckForBoundaries();
}

void PlayLevel::ReconstructLevel()
{
	if (PlayerHUD.IsValid())
	{
		RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
		PlayerHUD->SetCanvasDimensions(consoleDim.cx, consoleDim.cy);

		RC_RECT screenRect = PlayerHUD->GetScreenFreeRect();
		playAreaRect = { (float)screenRect.left, (float)screenRect.top, (float)screenRect.right, (float)screenRect.bottom };
	}

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
	LV_COORD spawnFoodCoord((float)MathLibrary::GetRandomInRange((int)playAreaRect.left, (int)playAreaRect.right),
		(float)MathLibrary::GetRandomInRange((int)playAreaRect.top, (int)playAreaRect.bottom));
	TObjectPtr<FoodObject> newFood = LevelManager::GetInstance().SpawnSceneObject<FoodObject>(spawnFoodCoord);
}

void PlayLevel::CheckForBoundaries()
{
	if (!pSnakePawn.IsValid()) return;

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
	// TODO: Show average FPS instead of the last second.
	if (PlayerHUD.IsValid())
		PlayerHUD->SetFPSCounter((uint32_t)(1.0f / TimeManager::GetInstance().GetFrameDeltaSeconds()));
}
