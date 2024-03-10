#include "LobbyPlayerState.h"

#include "Engine/GameObject/GameObjectUtility.h"
#include "Engine/Network/ReplicationUtility.h"

LobbyPlayerState::LobbyPlayerState()
{
}

void LobbyPlayerState::RegisterReplicationMembers()
{
	Inherited::RegisterReplicationMembers();

	REGISTER_REMOTE_INVOCATION(LobbyPlayerState::Server_SetPlayerReady);
	REGISTER_REMOTE_INVOCATION(LobbyPlayerState::Client_SetPlayerWaitForGameStart);
	REGISTER_REMOTE_INVOCATION(LobbyPlayerState::Client_PlayerEnterWrongName);
}

LobbyPlayerState::~LobbyPlayerState()
{
}

EngineGenericType* LobbyPlayerState::CloneGeneric() const
{
	return CreateNewObject<LobbyPlayerState>();
}

std::string LobbyPlayerState::GetGenericTypeName() const
{
	return "LobbyPlayerState";
}

/*
	Networking.
*/

void LobbyPlayerState::SetPlayerReady()
{
	CALL_REMOTE_INVOCATION_ON_SERVER(LobbyPlayerState::Server_SetPlayerReady);
}

/*
	Replication.
*/

void LobbyPlayerState::Server_SetPlayerReady()
{
	// TODO: Mark this player as ready. Block all input for this client until the game starts.
	if (playerName.empty())
	{
		// TODO: Player still needs to enter his name.
		DebugEngineTrap();
		CALL_REMOTE_INVOCATION_ON_OWNING_CLIENT(LobbyPlayerState::Client_PlayerEnterWrongName);
	}

	CALL_REMOTE_INVOCATION_ON_OWNING_CLIENT(LobbyPlayerState::Client_SetPlayerWaitForGameStart);
}

void LobbyPlayerState::Client_SetPlayerWaitForGameStart()
{
	// TODO.
	//DebugEngineTrap();
}

void LobbyPlayerState::Client_PlayerEnterWrongName()
{
	// TODO.
	//DebugEngineTrap();
}
