#include "LevelManager.h"

LevelManager::LevelManager()
{
}

LevelManager& LevelManager::GetInstance()
{
	static LevelManager instance;
	return instance;
}

void LevelManager::OpenLevel(GameLevel* level)
{
	LevelOpenEvent.Trigger(level);
}

void LevelManager::CloseLevel(GameLevel* level)
{
	LevelCloseEvent.Trigger(level);
}
