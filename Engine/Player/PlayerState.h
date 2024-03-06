#pragma once

#include "Engine/GameObject/GameObject.h"

namespace NetworkState {

struct ClientNetStateWrapper;

} // namespace NetworkState

REGISTER_CLASS(PlayerState)
class PlayerState : public GameObject
{
	GAMEOBJECT_BODY(PlayerState, GameObject)
	ENABLE_REPLICATION(PlayerState)

public:
	PlayerState() {}
	virtual ~PlayerState();

	void SetPlayerName(const char* player_name);
	const std::string& GetPlayerName() const { return playerName; }

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

	void SetNetPlayerState(NetworkState::ClientNetStateWrapper* netState);
	const NetworkState::ClientNetStateWrapper* GetNetPlayerInfo() const { return netPlayerState; }

	void SetPlayerId(uint32_t id);

	/** Replication. */

	void OnReplicate_PlayerName();
	void Server_SetPlayerName(std::string player_name);

protected:
	uint32_t playerId = 0;

	std::string playerName;

private:
	NetworkState::ClientNetStateWrapper* netPlayerState = nullptr;
};
