#pragma once

#include "Engine/GameObject/GameObject.h"
#include "Engine/GameObject/GameObjectPtr.h"

#include "Engine/SceneObjects/SceneObject.h"

#include "Engine/Input/InputHandler.h"
#include "Engine/Events/EventDelegate.h"

#include <vector>
#include <queue>

class GameLevel;

class GameLevel : public GameObject, public IInputHandler
{
public:
	GameLevel();
	virtual ~GameLevel();

	virtual void OpenLevel();
	virtual void Update(float DeltaTime);

	virtual bool PassInput(const InputState& is) override;

	void PlaceObjectOnLevel(GameObject* obj);

	virtual void ReconstructLevel() {}

private:
	std::vector<TObjectPtr<GameObject>> ObjectsOnLevel;
	std::queue<TObjectPtr<SceneObject>> ObjectBeginPlayQueue;
};
