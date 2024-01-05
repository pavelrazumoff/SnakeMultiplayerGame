#pragma once

#include "GameLevel.h"

#include "Engine/Events/EventDelegate.h"
#include "Engine/EngineTypes.h"
#include "Engine/EngineUtility.h"

#include "Engine/SceneObjects/SceneObject.h"
#include "Engine/GameObject/GameObjectUtility.h"

DECLARE_EVENT_DELEGATE(LevelActionDelegate, GameLevel*);

class LevelManager
{
public:
	LevelManager();
	~LevelManager() {}

	static LevelManager& GetInstance();

	void Release();

	void OpenLevel(GameLevel* level);
	void CloseLevel(GameLevel* level);

	void SetLevelDeltaSeconds(float deltaSeconds) { LevelDeltaSeconds = deltaSeconds; }
	float GetLevelDeltaSeconds() const { return LevelDeltaSeconds; }

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
			CurrentLevel->PlaceObjectOnLevel(sceneObject);
			sceneObject->SetLocation(Location);
		}
		else
			DebugEngineTrap();

		return SpawnedObject;
	}

protected:
	LevelActionDelegate LevelOpenEvent;
	LevelActionDelegate LevelCloseEvent;

private:
	TObjectPtr<GameLevel> CurrentLevel;
	float LevelDeltaSeconds = 0.0f;
};
