#pragma once

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

class PlayerManager
{
private:
	PlayerManager() {}

public:
	~PlayerManager();

	void Initialize();

	static PlayerManager& GetInstance();

	void SetPlayerStateClass(const std::string& className);

	PlayerState* MakeNewPlayer();
	PlayerState* MakeNewPlayer(NetworkState::ClientNetStateWrapper*& netState);
	void RegisterServerPlayerState(PlayerState* playerState);

	void DestroyPlayer(uint16_t playerIndex = 0);

	void NotifyAboutPlayerListChange();

	PlayerState* GetPlayerState(uint16_t playerIndex = 0);
	uint32_t GetPlayerCount() const;

	/** Networking. */

	uint32_t GetLocalPlayerId() const { return localPlayerId; }

	/** Events. */

	PlayerListChangeDelegate& OnPlayerListChangeEvent() { return playerListChangeEvent; }

protected:
	/** Networking. */
	friend class NetworkManager;

	void SetLocalPlayerId(uint32_t id);
	uint32_t GetNextPlayerId();

protected:
	PlayerListChangeDelegate playerListChangeEvent;

protected:
	std::vector<TObjectPtr<PlayerState>> playerStates;

private:
	std::string PlayerStateClassName = "";

	uint32_t localPlayerId = 0;
};
