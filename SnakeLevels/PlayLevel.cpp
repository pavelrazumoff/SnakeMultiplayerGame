#include "PlayLevel.h"

#include "SnakeObjects/SnakePawn.h"
#include "Engine/Level/LevelManager.h"
#include "Core/RenderConsoleLibrary.h"

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

	LV_COORD startPlayerCoord = { screenDim.cx / 2 - 2, screenDim.cy / 2 - 2 };
	pSnakePawn = LevelManager::GetInstance().SpawnSceneObject<SnakePawn>(startPlayerCoord);

	ReconstructLevel();
}

void PlayLevel::Update(float DeltaTime)
{
	Inherited::Update(DeltaTime);

}

void PlayLevel::ReconstructLevel()
{
}
