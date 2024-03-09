#include "PlayerManager.h"

#include "Engine/EngineFactory.h"
#include "Engine/Network/NetworkEngineUtility.h"

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

PlayerState* PlayerManager::MakeNewPlayer(NetworkState::ClientNetStateWrapper*& netState)
{
	PlayerState* _newPlayerState = MakeNewPlayer();
	if (!_newPlayerState)
	{
		delete netState; netState = nullptr;
		return nullptr;
	}

	if (netState)
	{
		_newPlayerState->SetPlayerId(GetNextPlayerId());

		_newPlayerState->SetNetPlayerState(netState);
		netState = nullptr;
	}

	return _newPlayerState;
}

void PlayerManager::RegisterRemotePlayerState(PlayerState* playerState)
{
	// TODO: Add checks for existing player id.
	playerStates.push_back(playerState);
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

void PlayerManager::SetLocalPlayerId(uint32_t id)
{
	localPlayerId = id;
}

uint32_t PlayerManager::GetNextPlayerId()
{
	static uint32_t nextPlayerId = 1;
	return nextPlayerId++;
}
