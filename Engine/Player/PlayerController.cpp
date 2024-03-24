#include "PlayerController.h"

#include "Engine/Player/PlayerManager.h"

void PlayerController::RegisterReplicationMembers()
{
	//Inherited::RegisterReplicationMembers();

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
