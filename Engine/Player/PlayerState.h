#pragma once

#include "Engine/GameObject/GameObject.h"
#include "Engine/Network/NetworkEngineUtility.h"

REGISTER_CLASS(PlayerState)
REGISTER_CLASS_FOR_REPLICATION(PlayerState)
class PlayerState : public GameObject
{
	GAMEOBJECT_BODY(PlayerState, GameObject)
	ENABLE_REPLICATION(PlayerState)

public:
	PlayerState() {}
	virtual ~PlayerState();

	void SetPlayerName(const char* player_name);
	const std::string& GetPlayerName() const { return playerName; }

protected:

	/** IReplicationObject implementation. */

	virtual void PostReplCreate() override;

	virtual void CopyProperties(PlayerState* targetPlayerState);

public:

	/** override EngineGenericType. */

	virtual [[nodiscard]] EngineGenericType* CloneGeneric() const override;
	virtual std::string GetGenericTypeName() const override;

	/** Networking. */

	uint32_t GetPlayerId() const { return playerId; }

protected:
	/** Networking. */

	friend class PlayerManager;
	friend class NetworkManager;

	void SetNetPlayerState(std::shared_ptr<NetworkState::ClientNetStateWrapper> netState);
	const NetworkState::ClientNetStateWrapper* GetNetPlayerInfo() const;

	void SetPlayerId(uint32_t id);

	/** Replication. */

	void OnReplicate_PlayerName();
	void Server_SetPlayerName(std::string player_name);

protected:
	uint32_t playerId = 0;

	std::string playerName;

private:
	std::shared_ptr<NetworkState::ClientNetStateWrapper> netPlayerState;
};
