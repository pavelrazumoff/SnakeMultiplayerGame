#include "LevelManager.h"

#include "Engine/GameWidget/GameWidgetManager.h"

#include "Engine/Other/TimeManager.h"

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
		if (!IsGamePaused())
			CurrentLevel->Update(deltaTime);

		CurrentLevel->Render();
	}

	// Always update widgets no matter gam is paused or not.
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
