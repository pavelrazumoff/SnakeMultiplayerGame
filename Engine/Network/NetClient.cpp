#include "NetClient.h"

void NetClient::StartClientLoop()
{
	auto clientSocket = GetSocket();
	if (bIsClientConnected ||
		!clientSocket) return;

	// Set the socket to non-blocking mode.
	// It means that the socket won't wait for data to be received while calling the Receive() function.
	if (NO_ERROR != clientSocket->SetNonBlockingMode(true))
	{
		// TODO: Logging.
		return;
	}

	bIsClientConnected = true;
	handleResponseThread = std::thread(&NetClient::ProcessServerResponseThread, this);
}

void NetClient::StopClientLoop()
{
	if (!bIsClientConnected) return;

	bIsClientConnected = false;

	// TODO: Do a callback.

	if (handleResponseThread.joinable())
		handleResponseThread.join();

	if (auto clientSocket = GetSocket())
		clientSocket->Shutdown();
}

/*
	Threads.
*/

void NetClient::ProcessServerResponseThread()
{
	while (bIsClientConnected)
	{
		std::unique_lock<std::shared_mutex> lock(loopMutex);

		// TODO.
	}
}
