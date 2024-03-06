#pragma once

#include "Engine/GameObject/GameObject.h"
#include "Engine/GameObject/GameObjectPtr.h"

#include "Engine/SceneObjects/SceneObject.h"

#include "Engine/Input/InputHandler.h"
#include "Engine/Events/EventDelegate.h"

#include <vector>
#include <queue>

REGISTER_CLASS(GameLevel)
class GameLevel : public GameObject, public IInputHandler
{
	GAMEOBJECT_BODY(GameLevel, GameObject)

public:
	GameLevel();
	virtual ~GameLevel();

	virtual void OpenLevel();
	virtual void CloseLevel();

	virtual void Update(float DeltaTime);
	virtual void Render();

	virtual bool PassInput(const InputState& is) override;

	void PlaceObjectOnLevel(GameObject* obj);

	virtual void ReconstructLevel() {}

	virtual void NotifyChildDestroy(GameObject* Child) override;

private:
	std::vector<TObjectPtr<GameObject>> ObjectsOnLevel;
	std::queue<TObjectPtr<SceneObject>> ObjectBeginPlayQueue;
};
