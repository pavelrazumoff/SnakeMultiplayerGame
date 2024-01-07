#include "GamePawn.h"
#include "../Components/MovementComponent.h"

#include "Engine/Level/LevelManager.h"

GamePawn::GamePawn()
{
	InputPawnComponent = CreateChildComponent<InputComponent>();

	MovementPawnComponent = CreateChildComponent<MovementComponent>();
	MovementPawnComponent->SetOwnerPawn(this);
}

GamePawn::~GamePawn()
{
}

void GamePawn::Update(float DeltaTime)
{
	Inherited::Update(DeltaTime);
}

void GamePawn::AddMovement(const LV_VECTOR& Direction, float Scale)
{
	if (auto movement = FindComponent<MovementComponent>(this))
	{
		movement->DoMovement(Direction * Scale);
	}
}

void GamePawn::_MovePawn(const LV_VECTOR& ScaledDirection)
{
	LV_COORD currentLocation = GetLocation();
	LV_COORD newLocation = currentLocation + ScaledDirection * LevelManager::GetInstance().GetLevelDeltaSeconds();

	SetLocation(newLocation);
}
