#pragma once

#include "SocketHandler.h"
#include "NetworkEngineUtility.h"

#include <shared_mutex>

class NetClient : public SocketHandler
{
	typedef SocketHandler Inherited;

public:
	NetClient(TCPSocketPtr _clientSocket) : Inherited(_clientSocket)
	{}

	void StartClientLoop();
	void StopClientLoop();

protected:
	/** Threads. */

	void ProcessServerResponseThread();

private:
	std::atomic<bool> bIsClientConnected = false;

	std::thread handleResponseThread;
	std::shared_mutex loopMutex;
};
