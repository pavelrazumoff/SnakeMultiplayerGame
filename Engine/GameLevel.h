#pragma once

#include "GameObject/GameObject.h"
#include "GameObject/GameObjectPtr.h"

#include <vector>

class GameLevel : public GameObject
{
public:
	GameLevel();
	virtual ~GameLevel();

	virtual void OpenLevel();
	virtual void Update(float DeltaTime);

	void PlaceObjectOnLevel(GameObject* obj);

	virtual void ReconstructLevel() {}

private:
	std::vector<TObjectPtr<GameObject>> ObjectsOnLevel;
};
