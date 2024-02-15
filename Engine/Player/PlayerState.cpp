#include "PlayerState.h"

#include "Engine/GameObject/GameObjectUtility.h"

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
