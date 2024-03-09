#include "LobbyPlayerState.h"

#include "Engine/GameObject/GameObjectUtility.h"
#include "Engine/Network/ReplicationUtility.h"

LobbyPlayerState::LobbyPlayerState()
{
}

void LobbyPlayerState::RegisterReplicationMembers()
{
	Inherited::RegisterReplicationMembers();

	ADD_REMOTE_INVOCATION_ON_SERVER(LobbyPlayerState::Server_SetPlayerReady);
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
	}


}
