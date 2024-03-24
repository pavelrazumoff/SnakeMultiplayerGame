#pragma once

#include "Engine/GameObject/GameObject.h"
#include "Engine/GameObject/GameObjectPtr.h"
#include "PlayerState.h"

REGISTER_CLASS(PlayerController)
class PlayerController : public GameObject
{
	GAMEOBJECT_BODY(PlayerController, GameObject)
	ENABLE_REPLICATION(PlayerController)

public:
	PlayerController() {}
	virtual ~PlayerController() {}

	/** override EngineGenericType. */

	virtual [[nodiscard]] EngineGenericType* CloneGeneric() const override;
	virtual std::string GetGenericTypeName() const override;

	PlayerState* GetPlayerState() { return playerState.Get(); }

protected:
	/** IReplicationObject implementation. */

	virtual void PostReplCreate() override;
	virtual void ReplDestroy() override;

	void SetPlayerState(PlayerState* _playerState);

protected:
	friend class PlayerManager;

protected:
	TObjectPtr<PlayerState> playerState;
};
