#pragma once

#include "Engine/GameObject/GameObject.h"

struct ClientInfo;

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

	void SetNetPlayerInfo(ClientInfo* info);
	const ClientInfo* GetNetPlayerInfo() const { return netPlayerInfo; }

protected:
	std::string playerName;

private:
	ClientInfo* netPlayerInfo = nullptr;
};
