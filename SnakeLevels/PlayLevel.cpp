#include "PlayLevel.h"

#include "SnakeObjects/SnakePawn.h"
#include "SnakeObjects/Food.h"

#include "SnakeWidgets/HUD/PlayerHUDWidget.h"
#include "SnakePlayer/SnakePlayerState.h"

#include "Engine/Level/LevelManager.h"
#include "Engine/Player/PlayerManager.h"
#include "Engine/GameWidget/GameWidgetManager.h"

#include "Core/RenderConsoleLibrary.h"

#include "Engine/Math/MathLibrary.h"

PlayLevel::PlayLevel()
{
}

PlayLevel::~PlayLevel()
{
}

void PlayLevel::OpenLevel()
{
	Inherited::OpenLevel();

	RC_SIZE screenDim = RenderConsoleLibrary::GetConsoleDimensions();

	LV_COORD startPlayerCoord((float)screenDim.cx / 2.0f - 2.0f, (float)screenDim.cy / 2.0f - 2.0f);
	pSnakePawn = LevelManager::GetInstance().SpawnSceneObject<SnakePawn>(startPlayerCoord);

	if (!PlayerHUD.IsValid())
	{
		PlayerHUD = CreateNewObject<PlayerHUDWidget>(this);

		GameWidgetManager::GetInstance().PlaceUserWidgetOnScreen(PlayerHUD.Get());
	}

	if (SnakePlayerState* pPlayerState = dynamic_cast<SnakePlayerState*>(PlayerManager::GetInstance().GetPlayerState()))
	{
		pPlayerState->OnScoreUpdatedEvent().Subscribe(this, &PlayLevel::HandleScoreChanged);
	}

	ReconstructLevel();
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
	if (!PlayerHUD.IsValid()) return;

	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
	PlayerHUD->SetCanvasDimensions(consoleDim.cx, consoleDim.cy);

	RC_RECT screenRect = PlayerHUD->GetScreenFreeRect();
	playAreaRect = { (float)screenRect.left, (float)screenRect.top, (float)screenRect.right, (float)screenRect.bottom };
	//playAreaRect = { 0.0f, 0.0f, (float)consoleDim.cx, (float)consoleDim.cy };
}

void PlayLevel::SpawnNewFood()
{
	LV_COORD spawnFoodCoord((float)MathLibrary::GetRandomInRange(playAreaRect.left, playAreaRect.right),
		(float)MathLibrary::GetRandomInRange(playAreaRect.top, playAreaRect.bottom));
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
	if (!PlayerHUD.IsValid()) return;

	PlayerHUD->SetScore(newScore);
}
