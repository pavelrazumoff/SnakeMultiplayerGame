#include "LobbyPlayerController.h"

#include "Engine/Network/ReplicationUtility.h"

#include "Engine/Player/PlayerManager.h"

void LobbyPlayerController::RegisterReplicationMembers()
{
	REGISTER_REMOTE_INVOCATION(LobbyPlayerController::Server_SetPlayerReady);
	REGISTER_REMOTE_INVOCATION(LobbyPlayerController::Client_SetPlayerWaitForGameStart);
	REGISTER_REMOTE_INVOCATION(LobbyPlayerController::Client_PlayerEnterWrongName);
}

EngineGenericType* LobbyPlayerController::CloneGeneric() const
{
	return CreateNewObject<LobbyPlayerController>();
}

std::string LobbyPlayerController::GetGenericTypeName() const
{
	return "LobbyPlayerController";
}

/*
	Networking.
*/

void LobbyPlayerController::SetPlayerReady()
{
	CALL_REMOTE_INVOCATION_ON_SERVER(LobbyPlayerController::Server_SetPlayerReady);
}

/*
	Replication.
*/

void LobbyPlayerController::Server_SetPlayerReady()
{
	// TODO: Mark this player as ready. Block all input for this client until the game starts.
	auto playerState = GetPlayerState();

	if (playerState->GetPlayerName().empty())
	{
		// TODO: Player still needs to enter his name.
		DebugEngineTrap();
		CALL_REMOTE_INVOCATION_ON_OWNING_CLIENT(LobbyPlayerController::Client_PlayerEnterWrongName);
	}

	CALL_REMOTE_INVOCATION_ON_OWNING_CLIENT(LobbyPlayerController::Client_SetPlayerWaitForGameStart);

	PlayerManager::GetInstance().NotifyAboutPlayerListChange();
}

void LobbyPlayerController::Client_SetPlayerWaitForGameStart()
{
	PlayerWaitGameEvent.Trigger();
}

void LobbyPlayerController::Client_PlayerEnterWrongName()
{
	// TODO.
	//DebugEngineTrap();
}