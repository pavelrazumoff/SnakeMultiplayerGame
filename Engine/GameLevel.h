#pragma once

#include "GameObject/GameObject.h"
#include "GameObject/GameObjectPtr.h"

#include "Engine/Input/InputHandler.h"
#include "Engine/Events/EventDelegate.h"

#include <vector>

class GameLevel;

DECLARE_EVENT_DELEGATE(LevelCloseDelegate, GameLevel*);

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

	/** Events. */
	LevelCloseDelegate& OnLevelCloseEvent() { return LevelCloseEvent; }

protected:
	LevelCloseDelegate LevelCloseEvent;

private:
	std::vector<TObjectPtr<GameObject>> ObjectsOnLevel;
};
