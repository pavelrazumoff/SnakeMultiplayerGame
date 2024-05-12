#include "PlayerController.h"

#include "Engine/Player/PlayerManager.h"
#include "Engine/SceneObjects/Pawn/GamePawn.h"

void PlayerController::RegisterReplicationMembers()
{
	// TODO.
}

EngineGenericType* PlayerController::CloneGeneric() const
{
	return CreateNewObject<PlayerController>();
}

std::string PlayerController::GetGenericTypeName() const
{
	return "PlayerController";
}

PlayerState* PlayerController::GetPlayerState()
{
	return playerState.Get();
}

GamePawn* PlayerController::GetPlayerPawn()
{
	return playerPawn.Get();
}

void PlayerController::PostReplCreate()
{
	Inherited::PostReplCreate();

	PlayerManager::GetInstance().RegisterRemotePlayerController(this);
}

void PlayerController::ReplDestroy()
{
	PlayerManager::GetInstance().RemoveRemotePlayerController(this);

	Inherited::ReplDestroy();
}

void PlayerController::SetPlayerState(PlayerState* _playerState)
{
	playerState = _playerState;
}

void PlayerController::SetPlayerPawn(GamePawn* _playerPawn)
{
	playerPawn = _playerPawn;

	// TODO: Register ownership for the same client.

	// Create a tree class that will represent the ownership tree for each player.
}
