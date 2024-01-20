#include "LevelManager.h"

#include "Engine/GameWidget/GameWidgetManager.h"

#include "Engine/Other/TimeManager.h"
#include "Engine/Other/ProfilerManager.h"

LevelManager::LevelManager()
{
}

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
		// TODO: Close the current level first.
		CurrentLevel->CloseLevel();
	}

	CurrentLevel = level;
	if (CurrentLevel.Get())
	{
		CurrentLevel->OpenLevel();
	}

	LevelOpenEvent.Trigger(level);
}

void LevelManager::CloseLevel(GameLevel* level)
{
	if (GetCurrentLevel() != level) { DebugEngineTrap(); return; }

	level->CloseLevel();
	LevelCloseEvent.Trigger(level);
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
