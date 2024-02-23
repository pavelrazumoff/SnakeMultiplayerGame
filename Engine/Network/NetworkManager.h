#pragma once

#include "SocketHandler.h"

#include "ListenServer.h"
#include "NetClient.h"

enum PacketType
{
	PT_Hello,
	PT_ReplicationData,
	PT_Disconnect,

	PT_MAX,
};

class NetworkManager
{
private:
	NetworkManager();

public:
	~NetworkManager();

	static NetworkManager& GetInstance();

	void Initialize();
	void Update();

	void Cleanup();

	/** Listen Server. */

	bool MakeListenServer();
	void StartListenServer();
	void StopListenServer();

	bool IsServer() const;

	/** Client. */

	bool ConnectClient(std::string server_addr);
	void StartClientLoop();
	void StopClientLoop();

	bool IsClient() const;

protected:

	/** Listen Server. */

	void ReadServerMessages();

	void DoSayHello(const NetworkState::ClientNetStateWrapper* client);
	void DoSayGoodbye(const NetworkState::ClientNetStateWrapper* client);
	void DoReplication();

	/** Client. */

	void ReadClientMessages();

protected:

	/** Listen Server. */

	void ProcessNewClient(const NetworkState::RawClientStateInfo& clientInfo);
	void ProcessClientDisconnected(const NetworkState::RawClientStateInfo& clientInfo);

	/** Client. */

	void ProcessServerPackage(NetworkState::RawServerPackageStateInfo& serverPackageInfo);

private:
	std::shared_ptr<ListenServer> listenServerObj;
	std::shared_ptr<NetClient> netClientObj;
};
