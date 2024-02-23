#pragma once

#include "SocketHandler.h"
#include "NetworkEngineUtility.h"
#include "Serialization/MemoryBitStream.h"

#include <vector>
#include <queue>
#include <map>
#include <shared_mutex>

#include "Engine/Events/EventDelegate.h"

class ListenServer : public SocketHandler
{
	typedef SocketHandler Inherited;

public:
	ListenServer(TCPSocketPtr _listenSocket);
	virtual ~ListenServer();

	void StartListen();
	void StopListen();

	NetworkState::RawClientStateInfo* PopWaitingClientMessage();
	void PushSendMessageToClient(NetworkState::Server2ClientPackage* package);

protected:
	/** Threads. */

	void ListenThread();

	/** Handle clients. */

	void ProcessNewClient(TCPSocketPtr newClientSocket, const SocketAddress& newClientAddress);
	void ProcessClientDisconnected(TCPSocketPtr clientSocket, int errCode = 0);
	bool ProcessClientError(int errorCode, TCPSocketPtr clientSocket);

	void ProcessClientDataReceived(TCPSocketPtr clientSocket, int32_t bytesReceived);
	void ProcessSendPackageToClient(TCPSocketPtr clientSocket);

	void UpdateWritableSocketsFromConnectedClients(std::vector<TCPSocketPtr>& outClientSockets);

	/** Other. */

	void ClearMessages();

protected:
	std::vector<TCPSocketPtr> connectedClients;
	std::queue<NetworkState::RawClientStateInfo*> waitingReadMessages;
	std::multimap<TCPSocketPtr, NetworkState::Server2ClientPackage*> waitingSendMessages;

private:
	std::atomic<bool> bIsServerRunning = false;

	std::thread handleListenThread;
	std::shared_mutex loopMutex;
	std::shared_mutex dataAccessMutex;

	std::unique_ptr<InputMemoryBitStream> inputBitStream;
};
