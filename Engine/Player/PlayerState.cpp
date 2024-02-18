#include "PlayerState.h"

#include "Engine/GameObject/GameObjectUtility.h"
#include "Engine/Network/NetworkEngineUtility.h"

PlayerState::~PlayerState()
{
	if (netPlayerState) delete netPlayerState;
}

void PlayerState::SetPlayerName(const char* player_name)
{
	playerName = player_name;
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
