#pragma once

#include "PlayerState.h"

/*
	TODO: The player manager class should represent some kind of game mode.
	Later we will store here all info and objects related to every player in a multiplayer game.
	
	Correction: Actually, the GameLevel inherited class should represent the game mode analogue,
	because it manages a lot of things in it and can control the appearance of gui and other stuff.

	Here we will store the player states of every player connected to the server. The player state can change for 
	players based on the actual level they are playing right now on.
*/

class PlayerManager
{
private:
	PlayerManager() {}

public:
	~PlayerManager();

	void Initialize();

	static PlayerManager& GetInstance();

	void SetPlayerStateClass(const std::string& className);
	PlayerState* GetPlayerState(uint16_t playerIndex = 0) const { return pPlayerState; }

protected:
	// TODO: When the multiplayer is implemented, this should be a vector of player states.
	PlayerState* pPlayerState = nullptr;

private:
	std::string PlayerStateClassName = "";
};
