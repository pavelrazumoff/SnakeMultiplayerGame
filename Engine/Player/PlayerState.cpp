#include "PlayerState.h"

#include "Engine/GameObject/GameObjectUtility.h"
#include "Engine/Network/NetworkEngineUtility.h"
#include "Engine/Network/ReplicationUtility.h"

#include "Engine/Player/PlayerManager.h"

#include "Engine/Log/Logger.h"

void PlayerState::RegisterReplicationMembers()
{
	Inherited::RegisterReplicationMembers();

	MAKE_REPLICATED(PlayerState, playerId, EPrimitiveType::EPT_Int, nullptr);
	MAKE_REPLICATED(PlayerState, playerName, EPrimitiveType::EPT_String, &PlayerState::OnReplicate_PlayerName);
	//ASSIGN_REPLICATION_CALLBACK(PlayerState, playerName, &PlayerState::OnReplicate_PlayerName);

	ADD_REMOTE_INVOCATION_ON_SERVER(PlayerState::Server_SetPlayerName);
}

PlayerState::~PlayerState()
{
	if (netPlayerState) delete netPlayerState;
}

/*
	IReplicationObject implementation.
*/

void PlayerState::PostReplCreate()
{
	Inherited::PostReplCreate();

	PlayerManager::GetInstance().RegisterRemotePlayerState(this);
}

/*
*/

void PlayerState::SetPlayerName(const char* player_name)
{
	if (playerName != player_name)
	{
		playerName = player_name;
		MARK_FOR_REPLICATION(PlayerState, playerName);
		CALL_REMOTE_INVOCATION_ON_SERVER(PlayerState::Server_SetPlayerName, playerName);
	}
}

EngineGenericType* PlayerState::CloneGeneric() const
{
	return CreateNewObject<PlayerState>();
}

std::string PlayerState::GetGenericTypeName() const
{
	return "PlayerState";
}

void PlayerState::SetNetPlayerState(NetworkState::ClientNetStateWrapper* netState)
{
	if (netPlayerState) delete netPlayerState;

	netPlayerState = netState;
}

void PlayerState::SetPlayerId(uint32_t id)
{
	if (playerId != id)
	{
		playerId = id;
		MARK_FOR_REPLICATION(PlayerState, playerId);
	}
}

/*
	Replication.
*/

void PlayerState::OnReplicate_PlayerName()
{
	Logger::GetInstance().Write("PlayerState::OnReplicate_PlayerName");
}

void PlayerState::Server_SetPlayerName(std::string player_name)
{
	// TODO: Add validation.
	DebugEngineTrap();
}
