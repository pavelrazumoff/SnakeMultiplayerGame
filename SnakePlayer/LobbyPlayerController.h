#pragma once

#include "Engine/Player/PlayerController.h"
#include "Engine/Events/EventDelegate.h"

DECLARE_EVENT_DELEGATE(PlayerWaitGameDelegate);

REGISTER_CLASS(LobbyPlayerController)
class LobbyPlayerController : public PlayerController
{
	GAMEOBJECT_BODY(LobbyPlayerController, PlayerController)
	ENABLE_REPLICATION(LobbyPlayerController)

public:
	LobbyPlayerController() {}
	virtual ~LobbyPlayerController() {}

	virtual [[nodiscard]] EngineGenericType* CloneGeneric() const override;
	virtual std::string GetGenericTypeName() const override;

public:
	/** Networking. */

	void SetPlayerReady();

	/** Events. */

	PlayerWaitGameDelegate& OnPlayerWaitGameEvent() { return PlayerWaitGameEvent; }

protected:
	/** Replication. */

	void Server_SetPlayerReady();
	void Client_SetPlayerWaitForGameStart();
	void Client_PlayerEnterWrongName();

protected:
	PlayerWaitGameDelegate PlayerWaitGameEvent;
};
