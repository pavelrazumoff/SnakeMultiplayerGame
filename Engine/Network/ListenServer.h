#pragma once

#include "SocketHandler.h"
#include "NetworkEngineUtility.h"
#include "Serialization/MemoryBitStream.h"

#include <vector>
#include <shared_mutex>

#include "Engine/Events/EventDelegate.h"

enum class ClientState
{
	Connected,
	Disconnected,
};

DECLARE_EVENT_DELEGATE(ClientConnectionStateChangedDelegate, const ClientInfo*, ClientState);
DECLARE_EVENT_DELEGATE(ClientErrorDelegate, const ClientInfo*, int);

class ListenServer : public SocketHandler
{
	typedef SocketHandler Inherited;

public:
	ListenServer(TCPSocketPtr _listenSocket);

	void StartListen();
	void StopListen();

protected:
	/** Threads. */

	void ListenThread();

	/** Handle clients. */

	void ProcessNewClient(TCPSocketPtr newClientSocket, const SocketAddress& newClientAddress);
	void ProcessClientDisconnected(TCPSocketPtr clientSocket);
	bool ProcessClientError(int errorCode, TCPSocketPtr clientSocket);

	void SyncWithClient(TCPSocketPtr ClientSocket);

	void UpdateWritableSocketsFromConnectedClients(std::vector<TCPSocketPtr>& outClientSockets);

public:
	/** Events. */

	ClientConnectionStateChangedDelegate& OnClientStateChangedEvent() { return ClientStateChangedEvent; }
	ClientErrorDelegate& OnClientErrorEvent() { return ClientErrorEvent; }

protected:
	std::vector<std::shared_ptr<ClientInfo>>::iterator FindClientInfoBySocket(TCPSocketPtr socket);
	const std::vector<std::shared_ptr<ClientInfo>>::const_iterator FindClientInfoBySocket(TCPSocketPtr socket) const;

protected:
	ClientConnectionStateChangedDelegate ClientStateChangedEvent;
	ClientErrorDelegate ClientErrorEvent;

	std::vector<std::shared_ptr<ClientInfo>> connectedClients;

private:
	std::atomic<bool> bIsServerRunning = false;

	std::thread handleListenThread;
	std::shared_mutex loopMutex;

	InputMemoryBitStream* inputBitStream = nullptr;
};
