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
}

void PlayLevel::ReconstructLevel()
{
}

void PlayLevel::SpawnNewFood()
{
	RC_SIZE screenDim = RenderConsoleLibrary::GetConsoleDimensions();

	LV_COORD spawnFoodCoord(MathLibrary::GetRandomInRange(0, screenDim.cx), MathLibrary::GetRandomInRange(0, screenDim.cy));
	TObjectPtr<FoodObject> newFood = LevelManager::GetInstance().SpawnSceneObject<FoodObject>(spawnFoodCoord);
}
