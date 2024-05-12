#include "LobbyPlayerState.h"

#include "Engine/GameObject/GameObjectUtility.h"
#include "Engine/Network/ReplicationUtility.h"

LobbyPlayerState::LobbyPlayerState()
{
}

void LobbyPlayerState::RegisterReplicationMembers()
{
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
