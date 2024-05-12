#pragma once

#include "GameLevel.h"

#include "Engine/Events/EventDelegate.h"
#include "Engine/EngineTypes/EngineBasicTypes.h"
#include "Engine/EngineUtility.h"

#include "Engine/SceneObjects/SceneObject.h"
#include "Engine/GameObject/GameObjectUtility.h"

DECLARE_EVENT_DELEGATE(LevelActionDelegate, GameLevel*);

class LevelManager
{
private:
	LevelManager() {}

public:
	~LevelManager() {}

	static LevelManager& GetInstance();

	void Release();

	void OpenLevel(GameLevel* level);
	void CloseLevel(GameLevel* level);

	void Update();
	void PassInput(const InputState& is);

	void PauseGame();
	void ResumeGame();

	bool IsGamePaused() const { return bGameIsPaused; }

	GameLevel* GetCurrentLevel() { return CurrentLevel.Get(); }

	LevelActionDelegate& OnLevelOpenEvent() { return LevelOpenEvent; }
	LevelActionDelegate& OnLevelCloseEvent() { return LevelCloseEvent; }

	/** Spawn Scene Objects on the current Level. */

	template<typename T>
	T* SpawnSceneObject(const LV_COORD& Location)
	{
		if (!GetCurrentLevel()) { DebugEngineTrap(); return nullptr; }

		static_assert(std::is_base_of<SceneObject, T>::value, "T must be derived from SceneObject.");

		T* SpawnedObject = CreateNewObject<T>(CurrentLevel.Get());
		if (SceneObject* sceneObject = dynamic_cast<SceneObject*>(SpawnedObject))
		{
			FinishSpawnObjectOnLevel(sceneObject, Location);
		}
		else
			DebugEngineTrap();

		return SpawnedObject;
	}

	void PlaceObjectOnLevel(SceneObject* sceneObject, const LV_COORD& Location);

protected:
	void FinishSpawnObjectOnLevel(SceneObject* sceneObject, const LV_COORD& Location);

protected:
	LevelActionDelegate LevelOpenEvent;
	LevelActionDelegate LevelCloseEvent;

private:
	TObjectPtr<GameLevel> CurrentLevel;

	bool bGameIsPaused = false;
};
