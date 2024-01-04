#pragma once

#include "Engine/SceneObjects/SceneObject.h"

class GamePawn : public SceneObject
{
	typedef SceneObject Inherited;

public:
	GamePawn();
	virtual ~GamePawn();

	virtual void Update(float DeltaTime) override;

	/** Extending the IInputHandler interface. */

	virtual bool PassInput(const InputState& is) override;
};
