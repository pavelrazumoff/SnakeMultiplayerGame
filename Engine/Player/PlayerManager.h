#pragma once

#include "PlayerController.h"
#include "PlayerState.h"
#include "Engine/GameObject/GameObjectPtr.h"
#include "Engine/Events/EventDelegate.h"

#include <vector>

/*
	TODO: The player manager class should represent some kind of game mode.
	Later we will store here all info and objects related to every player in a multiplayer game.
	
	Correction: Actually, the GameLevel inherited class should represent the game mode analogue,
	because it manages a lot of things in it and can control the appearance of gui and other stuff.

	Here we will store the player states of every player connected to the server. The player state can change for 
	players based on the actual level they are playing right now on.
*/

DECLARE_EVENT_DELEGATE(PlayerListChangeDelegate);

namespace NetworkState {
	struct ClientNetStateWrapper;
	struct RawClientStateInfo;
} // namespace NetworkState

class PlayerManager
{
private:
	PlayerManager() {}

public:
	~PlayerManager();

	void Initialize();

	static PlayerManager& GetInstance();

	void SetPlayerControllerClass(const std::string& className);
	void SetPlayerStateClass(const std::string& className);

	PlayerController* MakeNewPlayer();
	PlayerController* MakeNewPlayer(NetworkState::ClientNetStateWrapper*& netState);

	void RegisterRemotePlayerController(PlayerController* playerController);
	void RegisterRemotePlayerState(PlayerState* playerState);
	void RemoveRemotePlayerController(PlayerController* playerController);

	void DestroyPlayer(uint16_t playerIndex = 0);

	void NotifyAboutPlayerListChange();

	/**
	 *  playerIndex == 0 should mean the local player.
	 */
	PlayerController* GetPlayerController(uint16_t playerIndex = 0);
	PlayerState* GetPlayerState(uint16_t playerIndex = 0);
	uint32_t GetPlayerCount() const;

	/** Networking. */

	uint32_t GetLocalPlayerId() const { return localPlayerId; }

	/** Events. */

	PlayerListChangeDelegate& OnPlayerListChangeEvent() { return playerListChangeEvent; }

protected:

	/** Networking. */

	// Listen Server.
	void ProcessNewClient(NetworkState::ClientNetStateWrapper* netState);
	void ProcessClientDisconnect(const NetworkState::RawClientStateInfo& clientState);

	void DoSayHello(PlayerController* clientController);
	void DoTeleportToHostLevel(PlayerController* clientController);
	void DoSayGoodbye(PlayerController* clientController);

	void DoReplicatePlayerControllerToOwner(bool bNewClient, PlayerController* clientController);

	// Client.
	void ProcessJoinServerSuccess();
	void ProcessJoinServerFailure();

protected:
	// TODO: Remove NetworkManager class from here.
	friend class NetworkManager;
	friend class LevelManager;

	/** Networking. */

	void SetLocalPlayerId(uint32_t id);
	uint32_t GetNextPlayerId();

	void TransferPlayersBetweenLevels();

protected:
	PlayerListChangeDelegate playerListChangeEvent;

protected:
	std::vector<TObjectPtr<PlayerController>> playerControllers;

private:
	std::string PlayerControllerClassName = "";
	std::string PlayerStateClassName = "";

	uint32_t localPlayerId = 0;
};
