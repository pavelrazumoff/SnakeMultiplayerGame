#pragma once

#include "Engine/Player/PlayerState.h"

REGISTER_CLASS(LobbyPlayerState)
class LobbyPlayerState : public PlayerState
{
	GAMEOBJECT_BODY(LobbyPlayerState, PlayerState)
	ENABLE_REPLICATION(LobbyPlayerState)

public:
	LobbyPlayerState();
	virtual ~LobbyPlayerState();

	virtual [[nodiscard]] EngineGenericType* CloneGeneric() const override;
	virtual std::string GetGenericTypeName() const override;

public:
	/** Networking. */

	void SetPlayerReady();

protected:
	/** Replication. */

	void Server_SetPlayerReady();
};
