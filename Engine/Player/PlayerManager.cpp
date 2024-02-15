#include "PlayerManager.h"

#include "Engine/EngineFactory.h"

PlayerManager::~PlayerManager()
{
}

void PlayerManager::Initialize()
{
	if (PlayerStateClassName.empty()) { DebugEngineTrap(); return; }


}

PlayerManager& PlayerManager::GetInstance()
{
	static PlayerManager instance;
	return instance;
}

void PlayerManager::SetPlayerStateClass(const std::string& className)
{
	if (PlayerStateClassName == className)
		return;

	PlayerStateClassName = className;
}

PlayerState* PlayerManager::MakeNewPlayer()
{
	if (auto playerState = dynamic_cast<PlayerState*>(EngineFactory::GetInstance().Create(PlayerStateClassName)))
	{
		playerStates.push_back(playerState);
		return playerState;
	}

	return nullptr;
}

void PlayerManager::DestroyPlayer(uint16_t playerIndex)
{
	if (auto playerState = GetPlayerState(playerIndex))
	{
		playerStates.erase(playerStates.begin() + playerIndex);
		playerState->Destroy();
	}
}

void PlayerManager::NotifyAboutPlayerListChange()
{
	playerListChangeEvent.Trigger();
}

PlayerState* PlayerManager::GetPlayerState(uint16_t playerIndex)
{
	return (playerIndex >= 0 && playerIndex < playerStates.size()) ? playerStates[playerIndex].Get() : nullptr;
}

uint32_t PlayerManager::GetPlayerCount() const
{
	return (uint32_t)playerStates.size();
}
