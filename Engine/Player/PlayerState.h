#pragma once

#include "Engine/GameObject/GameObject.h"

namespace NetworkState {

struct ClientNetStateWrapper;

} // namespace NetworkState

class PlayerState : public GameObject
{
public:
	PlayerState() {}
	virtual ~PlayerState();

	void SetPlayerName(const char* player_name);
	const std::string& GetPlayerName() const { return playerName; }

public:

	/** override EngineGenericType. */

	virtual [[nodiscard]] EngineGenericType* CloneGeneric() const override;
	virtual std::string GetGenericTypeName() const override;

protected:
	/** Networking. */

	friend class NetworkManager;

	void SetNetPlayerState(NetworkState::ClientNetStateWrapper* netState);
	const NetworkState::ClientNetStateWrapper* GetNetPlayerInfo() const { return netPlayerState; }

protected:
	std::string playerName;

private:
	NetworkState::ClientNetStateWrapper* netPlayerState = nullptr;
};
