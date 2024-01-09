#pragma once

#include "PlayerState.h"

/*
	TODO: The player manager class should represent some kind of game mode.
	Later we will store here all info and objects related to every player in a multiplayer game.
*/

class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();

	void Initialize();

	static PlayerManager& GetInstance();

	void SetPlayerStateClass(const std::string& className);
	PlayerState* GetPlayerState(uint16_t playerIndex = 0) const { return pPlayerState; }

protected:
	// TODO: When the mulitplayer is implemented, this should be a vector of player states.
	PlayerState* pPlayerState = nullptr;

private:
	std::string PlayerStateClassName = "";
};
