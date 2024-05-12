#include "GamePawn.h"
#include "../Components/MovementComponent.h"

#include "Engine/Other/TimeManager.h"
#include "Engine/Network/ReplicationUtility.h"
#include "Engine/Player/PlayerManager.h"

GamePawn::GamePawn()
{
	MovementPawnComponent = CreateChildComponent<MovementComponent>();
	MovementPawnComponent->SetOwnerPawn(this);
}

GamePawn::~GamePawn()
{
}

void GamePawn::BeginPlay()
{
	Inherited::BeginPlay();

	ApplyInputBindings();
}

void GamePawn::RegisterReplicationMembers()
{
	MAKE_REPLICATED(GamePawn, ControllerNetworkId, EPrimitiveType::EPT_Int, nullptr);
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
	LV_COORD newLocation = currentLocation + ScaledDirection * TimeManager::GetInstance().GetFrameDeltaSeconds();

	SetLocation(newLocation);
}

void GamePawn::PostReplCreate()
{
	Inherited::PostReplCreate();

	PlayerManager::GetInstance().TryRegisterRemotePlayerPawn(this);

	// TODO: Put in more appropriate place.
	// Won't work on local play.
	if (PlayerManager::GetInstance().GetPlayerPawn() == this)
		InputPawnComponent = CreateChildComponent<InputComponent>();
}

EngineGenericType* GamePawn::CloneGeneric() const
{
	return CreateNewObject<GamePawn>();
}

std::string GamePawn::GetGenericTypeName() const
{
	return "GamePawn";
}

void GamePawn::SetControllerNetworkId(uint32_t objId)
{
	ControllerNetworkId = objId;
	MARK_FOR_REPLICATION(GamePawn, ControllerNetworkId);
}
