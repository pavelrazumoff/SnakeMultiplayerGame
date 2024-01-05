#include "MovementComponent.h"
#include "../Pawn/GamePawn.h"

MovementComponent::MovementComponent()
{
}

MovementComponent::~MovementComponent()
{
}

void MovementComponent::SetOwnerPawn(GamePawn* owner)
{
	OwnerPawn = owner;
}

void MovementComponent::DoMovement(const LV_VECTOR& ScaledDirection)
{
	if (OwnerPawn.Get())
		OwnerPawn->_MovePawn(ScaledDirection * MovementSpeed);
}
