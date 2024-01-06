#include "LevelManager.h"

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

	LevelCloseEvent.Trigger(level);
}

void LevelManager::SetLevelDeltaSeconds(float deltaSeconds)
{
	LevelDeltaSeconds = deltaSeconds;
}

float LevelManager::GetLevelDeltaSeconds() const
{
	return LevelDeltaSeconds;
}
