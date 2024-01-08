#include "PlayLevel.h"

#include "SnakeObjects/SnakePawn.h"
#include "SnakeObjects/Food.h"

#include "Engine/Level/LevelManager.h"
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
}

void PlayLevel::SpawnNewFood()
{
	RC_SIZE screenDim = RenderConsoleLibrary::GetConsoleDimensions();

	LV_COORD spawnFoodCoord((float)MathLibrary::GetRandomInRange(0, screenDim.cx), (float)MathLibrary::GetRandomInRange(0, screenDim.cy));
	TObjectPtr<FoodObject> newFood = LevelManager::GetInstance().SpawnSceneObject<FoodObject>(spawnFoodCoord);
}

void PlayLevel::CheckForBoundaries()
{
	if (!pSnakePawn.IsValid()) return;

	RC_SIZE screenDim = RenderConsoleLibrary::GetConsoleDimensions();
	LV_SIZE boundaries = { (float)screenDim.cx, (float)screenDim.cy };

	LV_COORD snakeLocation = pSnakePawn->GetLocation();
	if (snakeLocation.x <= 0.0f)
		snakeLocation.x = boundaries.x - 1.0f;
	else if (snakeLocation.x > boundaries.x - 1.0f)
		snakeLocation.x = 1.0f;
	else if (snakeLocation.y <= 0.0f)
		snakeLocation.y = boundaries.y - 1.0f;
	else if (snakeLocation.y > boundaries.y - 1.0f)
		snakeLocation.y = 0.0f;

	pSnakePawn->SetLocation(snakeLocation);
}
