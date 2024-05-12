#include "LevelManager.h"

#include "Engine/GameWidget/GameWidgetManager.h"

#include "Engine/Player/PlayerManager.h"
#include "Engine/Network/NetworkManager.h"

#include "Engine/Other/TimeManager.h"
#include "Engine/Other/ProfilerManager.h"

LevelManager& LevelManager::GetInstance()
{
	static LevelManager instance;
	return instance;
}

void LevelManager::Release()
{
	CurrentLevel = nullptr;
}

void LevelManager::OpenLevel(GameLevel* level)
{
	if (CurrentLevel.Get())
	{
		CurrentLevel->CloseLevel();
		CurrentLevel->Destroy();
	}

	CurrentLevel = level;
	if (CurrentLevel.Get())
	{
		CurrentLevel->OpenLevel();
	}

	// Move all connected clients from one level to another.
	PlayerManager::GetInstance().TransferPlayersBetweenLevels();

	LevelOpenEvent.Trigger(level);
}

void LevelManager::CloseLevel(GameLevel* level)
{
	/*
		Call this method only when you has no other level to open.
		It means that we want to close the game actually.
	*/

	if (GetCurrentLevel() != level) { DebugEngineTrap(); return; }

	level->CloseLevel();
	LevelCloseEvent.Trigger(level); // TODO: Not the best idea to pass the closing level with some event.

	// For just a case use our safe pointer to destroy the level after sending the event.
	if (CurrentLevel.IsValid())
		CurrentLevel->Destroy();
}

void LevelManager::Update()
{
	const float deltaTime = TimeManager::GetInstance().GetFrameDeltaSeconds();
	if (CurrentLevel.IsValid())
	{
		if (ProfilerManager::GetInstance().IsEngineFeatureEnabled(ProfilerEngineFeature::UpdateScene) &&
			!IsGamePaused())
			CurrentLevel->Update(deltaTime);

		if (ProfilerManager::GetInstance().IsEngineFeatureEnabled(ProfilerEngineFeature::RenderScene))
			CurrentLevel->Render();

		NetworkManager::GetInstance().PutInReplicationQueue(CurrentLevel.Get());

		CurrentLevel->DoForEachObject([](GameObject* obj) {

			if (obj->IsReplicationEnabled())
				NetworkManager::GetInstance().PutInReplicationQueue(obj);
		});
	}

	// Always update widgets no matter game is paused or not.
	if (ProfilerManager::GetInstance().IsEngineFeatureEnabled(ProfilerEngineFeature::RenderWidgets))
		GameWidgetManager::GetInstance().Update(deltaTime);
}

void LevelManager::PassInput(const InputState& is)
{
	if (!IsGamePaused() && 
		CurrentLevel.IsValid())
	{
		if (CurrentLevel->PassInput(is)) return;
	}

	GameWidgetManager::GetInstance().PassInput(is);
}

void LevelManager::PauseGame()
{
	bGameIsPaused = true;
}

void LevelManager::ResumeGame()
{
	bGameIsPaused = false;
}

void LevelManager::PlaceObjectOnLevel(SceneObject* sceneObject, const LV_COORD& Location)
{
	if (!sceneObject || !GetCurrentLevel()) { DebugEngineTrap(); return; }

	CurrentLevel->PlaceObjectOnLevel(sceneObject);
	sceneObject->SetLocation(Location);
}

void LevelManager::FinishSpawnObjectOnLevel(SceneObject* sceneObject, const LV_COORD& Location)
{
	NetworkManager::GetInstance().PutInReplicationQueue(sceneObject, RQA_Create);
	PlaceObjectOnLevel(sceneObject, Location);
}
