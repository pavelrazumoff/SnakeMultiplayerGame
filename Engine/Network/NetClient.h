#pragma once

#include "SocketHandler.h"
#include "NetworkEngineUtility.h"
#include "Serialization/MemoryBitStream.h"

#include <queue>
#include <shared_mutex>

class NetClient : public SocketHandler
{
	typedef SocketHandler Inherited;

public:
	NetClient(TCPSocketPtr _clientSocket);
	~NetClient();

	void StartClientLoop();
	void StopClientLoop();

	NetworkState::RawServerPackageStateInfo* PopWaitingServerMessage();
	void SendMessageToServer(const char* msgData, uint32_t msgByteCount);

protected:
	/** Threads. */

	void ProcessServerResponseThread();

	/** Handle Server response. */

	bool ProcessServerError(int errorCode);

	void ProcessServerDataReceived(int32_t bytesReceived);

	/** Other. */

	void ClearMessages();

protected:
	std::queue<NetworkState::RawServerPackageStateInfo*> waitingReadMessages;

private:
	std::atomic<bool> bIsClientConnected = false;

	std::thread handleResponseThread;
	std::shared_mutex loopMutex;
	std::shared_mutex dataAccessMutex;

	std::unique_ptr<InputMemoryBitStream> inputBitStream;
};
