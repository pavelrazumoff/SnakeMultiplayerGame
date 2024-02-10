#pragma once

#include "SocketHandler.h"

#include "ListenServer.h"
#include "NetClient.h"

class NetworkManager
{
private:
	NetworkManager();

public:
	~NetworkManager();

	static NetworkManager& GetInstance();

	void Initialize();
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

	/** Listen Server Events. */

	void PrepareListenServerEvents();
	void HandleNewClientConnected(const ClientInfo* clientInfo, ClientState newState);
	void HandleClientError(const ClientInfo* clientInfo, int errorCode);

private:
	std::shared_ptr<ListenServer> listenServerObj;
	std::shared_ptr<NetClient> netClientObj;
};
