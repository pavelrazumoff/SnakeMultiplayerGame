#pragma once

#include "ObjectComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/GameObject/GameObjectPtr.h"

class GamePawn;

class MovementComponent : public ObjectComponent
{
	typedef ObjectComponent Inherited;

public:
	MovementComponent();
	virtual ~MovementComponent();

	void SetOwnerPawn(GamePawn* owner);
	virtual void DoMovement(const LV_VECTOR& ScaledDirection);

	/** Movement Properties. */

	void SetMovementSpeed(LV_VECTOR speed) { MovementSpeed = speed; }
	LV_VECTOR GetMovementSpeed() const { return MovementSpeed; }

protected:
	LV_VECTOR MovementSpeed = { 10.0f, 10.0f };

private:
	TObjectPtr<GamePawn> OwnerPawn;
};
