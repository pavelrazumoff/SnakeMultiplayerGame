#pragma once

#include "GameLevel.h"

#include "Engine/Events/EventDelegate.h"

DECLARE_EVENT_DELEGATE(LevelActionDelegate, GameLevel*);

class LevelManager
{
public:
	LevelManager();
	~LevelManager() {}

	static LevelManager& GetInstance();

	void OpenLevel(GameLevel* level);
	void CloseLevel(GameLevel* level);

	LevelActionDelegate& OnLevelOpenEvent() { return LevelOpenEvent; }
	LevelActionDelegate& OnLevelCloseEvent() { return LevelCloseEvent; }

protected:
	LevelActionDelegate LevelOpenEvent;
	LevelActionDelegate LevelCloseEvent;
};
