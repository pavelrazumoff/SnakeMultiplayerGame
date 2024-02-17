#include "PlayerState.h"

#include "Engine/GameObject/GameObjectUtility.h"
#include "Engine/Network/NetworkEngineUtility.h"

PlayerState::~PlayerState()
{
	if (netPlayerInfo) delete netPlayerInfo;
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

void PlayerState::SetNetPlayerInfo(ClientInfo* info)
{
	if (netPlayerInfo) delete netPlayerInfo;

	netPlayerInfo = info;
}
