#pragma once

#include "SocketHandler.h"
#include "NetworkEngineUtility.h"
#include "Serialization/MemoryBitStream.h"

#include <vector>
#include <queue>
#include <shared_mutex>

#include "Engine/Events/EventDelegate.h"

class ListenServer : public SocketHandler
{
	typedef SocketHandler Inherited;

public:
	ListenServer(TCPSocketPtr _listenSocket);

	void StartListen();
	void StopListen();

	ClientInfo* PopWaitingHandleClient();

protected:
	/** Threads. */

	void ListenThread();

	/** Handle clients. */

	void ProcessNewClient(TCPSocketPtr newClientSocket, const SocketAddress& newClientAddress);
	void ProcessClientDisconnected(TCPSocketPtr clientSocket, int errCode = 0);
	bool ProcessClientError(int errorCode, TCPSocketPtr clientSocket);

	void SyncWithClient(TCPSocketPtr ClientSocket);

	void UpdateWritableSocketsFromConnectedClients(std::vector<TCPSocketPtr>& outClientSockets);

protected:
	std::vector<TCPSocketPtr> connectedClients;
	std::queue<ClientInfo*> waitingHandleClients;

private:
	std::atomic<bool> bIsServerRunning = false;

	std::thread handleListenThread;
	std::shared_mutex loopMutex;
	std::shared_mutex dataAccessMutex;

	InputMemoryBitStream* inputBitStream = nullptr;
};
