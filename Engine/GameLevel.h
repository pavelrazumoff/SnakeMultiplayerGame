#pragma once

#include "GameObject/GameObject.h"
#include "GameObject/GameObjectPtr.h"

#include "Engine/Input/InputHandler.h"

#include <vector>

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
};
