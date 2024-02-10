#pragma once

#include "Engine/SceneObjects/SceneObject.h"
#include "../Components/InputComponent.h"
#include "../Components/MovementComponent.h"

class GamePawn : public SceneObject
{
	typedef SceneObject Inherited;

	GAMEOBJECT_BODY(GamePawn)

public:
	GamePawn();
	virtual ~GamePawn();

	virtual void Update(float DeltaTime) override;

	void AddMovement(const LV_VECTOR& Direction, float Scale);

protected:
	void _MovePawn(const LV_VECTOR& ScaledDirection);

protected:
	friend class MovementComponent;

	TObjectPtr<InputComponent> InputPawnComponent;
	TObjectPtr<MovementComponent> MovementPawnComponent;
};
