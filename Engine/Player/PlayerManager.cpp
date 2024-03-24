#include "PlayerManager.h"

#include "Engine/EngineFactory.h"
#include "Engine/Network/NetworkManager.h"
#include "Engine/Network/NetworkEngineUtility.h"
#include "Engine/Level/LevelManager.h"

PlayerManager::~PlayerManager()
{
}

void PlayerManager::Initialize()
{
	if (PlayerControllerClassName.empty() ||
		PlayerStateClassName.empty()) { DebugEngineTrap(); return; }

	// Server events.
	NetworkManager::GetInstance().OnNewClientConnected().Subscribe(this, &PlayerManager::ProcessNewClient);
	NetworkManager::GetInstance().OnClientDisconnected().Subscribe(this, &PlayerManager::ProcessClientDisconnect);

	// Client events.
	NetworkManager::GetInstance().OnJoinServerSuccess().Subscribe(this, &PlayerManager::ProcessJoinServerSuccess);
	NetworkManager::GetInstance().OnJoinServerFailure().Subscribe(this, &PlayerManager::ProcessJoinServerFailure);
}

PlayerManager& PlayerManager::GetInstance()
{
	static PlayerManager instance;
	return instance;
}

void PlayerManager::SetPlayerControllerClass(const std::string& className)
{
	if (PlayerControllerClassName == className)
		return;

	PlayerControllerClassName = className;
}

void PlayerManager::SetPlayerStateClass(const std::string& className)
{
	if (PlayerStateClassName == className)
		return;

	PlayerStateClassName = className;
}

PlayerController* PlayerManager::MakeNewPlayer()
{
	if (auto playerController =
		dynamic_cast<PlayerController*>(EngineFactory::GetInstance().Create(PlayerControllerClassName)))
	{
		playerControllers.push_back(playerController);
		if (auto playerState = dynamic_cast<PlayerState*>(EngineFactory::GetInstance().Create(PlayerStateClassName)))
		{
			playerController->SetPlayerState(playerState);
		}
		else DebugEngineTrap();

		return playerController;
	}

	DebugEngineTrap(); 
	return nullptr;
}

PlayerController* PlayerManager::MakeNewPlayer(NetworkState::ClientNetStateWrapper*& netState)
{
	PlayerController* _newPlayerController = MakeNewPlayer();
	if (!_newPlayerController)
	{
		delete netState; netState = nullptr;
		return nullptr;
	}

	if (netState)
	{
		auto _playerState = _newPlayerController->GetPlayerState();
		_playerState->SetPlayerId(GetNextPlayerId());

		_playerState->SetNetPlayerState(netState);
		netState = nullptr;
	}

	return _newPlayerController;
}

void PlayerManager::RegisterRemotePlayerController(PlayerController* playerController)
{
	playerControllers.push_back(playerController);
}

void PlayerManager::RegisterRemotePlayerState(PlayerState* playerState)
{
	// TODO: Add checks for existing player id.
	if (auto _playerController = GetPlayerController(GetPlayerCount() - 1))
	{
		_playerController->SetPlayerState(playerState);
	}
	else DebugEngineTrap();
}

void PlayerManager::RemoveRemotePlayerController(PlayerController* playerController)
{
	if (!playerController) { DebugEngineTrap(); return; }

	auto it = std::find_if(playerControllers.begin(), playerControllers.end(),
		[playerController](TObjectPtr<PlayerController>& _playerController) -> bool {
		return playerController == _playerController.Get();
	});

	if (it != playerControllers.end())
		playerControllers.erase(it);
}

void PlayerManager::DestroyPlayer(uint16_t playerIndex)
{
	if (auto _playerController = GetPlayerController(playerIndex))
	{
		playerControllers.erase(playerControllers.begin() + playerIndex);
		_playerController->Destroy();
	}
}

void PlayerManager::NotifyAboutPlayerListChange()
{
	playerListChangeEvent.Trigger();
}

PlayerController* PlayerManager::GetPlayerController(uint16_t playerIndex)
{
	return (playerIndex >= 0 && playerIndex < playerControllers.size()) ? playerControllers[playerIndex].Get() : nullptr;
}

PlayerState* PlayerManager::GetPlayerState(uint16_t playerIndex)
{
	if (auto _playerController = GetPlayerController(playerIndex))
	{
		return _playerController->GetPlayerState();
	}

	return nullptr;
}

uint32_t PlayerManager::GetPlayerCount() const
{
	return (uint32_t)playerControllers.size();
}

/*
	Networking.
*/

// Listen Server.

void PlayerManager::ProcessNewClient(NetworkState::ClientNetStateWrapper* netState)
{
	auto newPlayerController = MakeNewPlayer(netState);
	if (!newPlayerController) { DebugEngineTrap(); return; }

	DoSayHello(newPlayerController);
	DoTeleportToHostLevel(newPlayerController);

	// TODO: Replicate all player states to connected clients.
}

void PlayerManager::ProcessClientDisconnect(const NetworkState::RawClientStateInfo& clientState)
{
	const uint32_t numPlayers = GetPlayerCount();
	for (uint32_t i = 0; i < numPlayers; ++i)
	{
		auto playerController = GetPlayerController(i);
		auto playerState = playerController ? playerController->GetPlayerState() : nullptr;
		if (const auto netInfo = playerController ? playerState->GetNetPlayerInfo() : nullptr)
		{
			if (*netInfo->GetAddress() == *clientState.GetAddress())
			{
				DestroyPlayer(i);
				break;
			}
		}
	}

	NotifyAboutPlayerListChange();
}

void PlayerManager::DoSayHello(PlayerController* clientController)
{
	DoReplicatePlayerControllerToOwner(PacketType::PT_Hello, clientController);
}

void PlayerManager::DoTeleportToHostLevel(PlayerController* clientController)
{
	auto client = clientController->GetPlayerState() ?
		clientController->GetPlayerState()->GetNetPlayerInfo() :
		nullptr;
	if (!client) { DebugEngineTrap(); return; }

	// Replicate the current open level to connected client.
	GameLevel* currentLevel = LevelManager::GetInstance().GetCurrentLevel();

	std::vector<IReplicationObject*> objList;
	objList.push_back(currentLevel);

	NetworkManager::GetInstance().ReplicateCreateObjectListPacket(PacketType::PT_ReplicationData, objList, client);
}

void PlayerManager::DoSayGoodbye(PlayerController* clientController)
{
	// TODO: If we want to force the client to disconnect.
}

void PlayerManager::DoReplicatePlayerControllerToOwner(bool bNewClient, PlayerController* clientController)
{
	auto clientPlayerState = clientController->GetPlayerState();
	auto netPlayerInfo = clientPlayerState ? clientPlayerState->GetNetPlayerInfo() : nullptr;
	if (!netPlayerInfo) { DebugEngineTrap(); return; }

	std::vector<IReplicationObject*> objList;
	objList.push_back(clientController);
	objList.push_back(clientPlayerState);

	PacketType type = bNewClient ? PacketType::PT_Hello : PacketType::PT_ReplicationData;
	NetworkManager::GetInstance().ReplicateCreateObjectListPacket(type, objList, netPlayerInfo);
}

// Client.

void PlayerManager::ProcessJoinServerSuccess()
{
	if (auto firstPlayerController = GetPlayerController(0))
	{
		if (auto firstPlayerState = firstPlayerController->GetPlayerState())
			SetLocalPlayerId(firstPlayerState->GetPlayerId());
	}
}

void PlayerManager::ProcessJoinServerFailure()
{
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

void PlayerManager::TransferPlayersBetweenLevels()
{
	if (!NetworkUtility::IsServer()) return;

	size_t numOldPlayerControllers = playerControllers.size();
	for (size_t i = 0; i < numOldPlayerControllers; ++i)
	{
		MakeNewPlayer();
	}

	engine_assert(playerControllers.size() == numOldPlayerControllers * 2);

	for (size_t i = 0, j = numOldPlayerControllers; i < numOldPlayerControllers; ++i, ++j)
	{
		auto oldPlayerController = GetPlayerController(i);
		auto newPlayerController = GetPlayerController(j);

		if (oldPlayerController && newPlayerController)
		{
			oldPlayerController->GetPlayerState()->CopyProperties(newPlayerController->GetPlayerState());
		}
	}

	while (playerControllers.size() > numOldPlayerControllers)
	{
		playerControllers[0]->Destroy();
		playerControllers.erase(playerControllers.begin());
	}

	for (size_t i = 0; i < numOldPlayerControllers; ++i)
	{
		auto clientController = playerControllers[i].Get();
		DoReplicatePlayerControllerToOwner(false, clientController);

		DoTeleportToHostLevel(clientController);
	}
}
