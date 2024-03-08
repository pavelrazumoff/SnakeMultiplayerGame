#include "NetClient.h"

#include "Engine/Log/Logger.h"
#include "Engine/EngineUtility.h"

NetClient::NetClient(TCPSocketPtr _clientSocket)
	: Inherited(_clientSocket)
	, inputBitStream(new InputMemoryBitStream(NetworkUtility::MAX_PACKET_SIZE))
{
}

NetClient::~NetClient()
{
	ClearMessages();
}

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
	//if (!bIsClientConnected) return;

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
	TCPSocketPtr clientSocket = GetSocket();

	while (bIsClientConnected)
	{
		std::unique_lock<std::shared_mutex> lock(loopMutex);

		short packageSize = 0;
		int bytesReceived = clientSocket->Receive(&packageSize, sizeof(packageSize));
		if (bytesReceived > 0)
		{
			inputBitStream->Reset(packageSize);

			// TODO: If there will be an issue with the Receive func returns less read bytes count than a packageSize, then we need to read it in a loop.
			bytesReceived = clientSocket->Receive(inputBitStream->GetBufferPtr(), inputBitStream->GetCapacity());
			if (bytesReceived > 0) // Received some data.
			{
				ProcessServerDataReceived(bytesReceived);
				continue;
			}
		}
		
		if (bytesReceived == 0) // Server sent a FIN flag. So the connection is closed.
		{
			Logger::WriteThreadSafe("Server sent a FIN flag. The connection is closed.");

			// TODO: Do a callback to close the socket outside.
			bIsClientConnected = false;
			break;
		}
		else
		{
			// Most of the time we will come here getting this error. It's normal, just ignore it.
			// The reason of it is that we set the socket to non-blocking mode and after getting no data in the incoming buffer we just stopped waiting for it.
			if (-bytesReceived == WSAEWOULDBLOCK) continue;

			// Okay, there was some error for sure.
			int errorCode = -bytesReceived;
			if (!ProcessServerError(errorCode))
			{
				// TODO: Do a callback to close the socket outside.
				bIsClientConnected = false;
				break;
			}
		}
	}
}

/*
	Handle Server response.
*/

bool NetClient::ProcessServerError(int errorCode)
{
	std::string message = "Received an error from the server. Error code: " + std::to_string(errorCode) + ".";
	Logger::WriteThreadSafe(message.c_str());

	if (errorCode == WSAECONNRESET)
	{
		// TODO: Logging.
		return false;
	}
	return true;
}

void NetClient::ProcessServerDataReceived(int32_t bytesReceived)
{
	std::string message = "Received " + std::to_string(bytesReceived) + " bytes from the server.";
	Logger::WriteThreadSafe(message.c_str());

	std::unique_lock<std::shared_mutex> lock(dataAccessMutex);

	using namespace NetworkState;

	RawServerPackageStateInfo* serverPackageInfo = new RawServerPackageStateInfo(inputBitStream->GetBufferPtr(), bytesReceived);
	waitingReadMessages.push(serverPackageInfo);
}

NetworkState::RawServerPackageStateInfo* NetClient::PopWaitingServerMessage()
{
	std::unique_lock<std::shared_mutex> lock(dataAccessMutex);

	using namespace NetworkState;

	if (waitingReadMessages.empty()) return nullptr;

	RawServerPackageStateInfo* nextInfo = waitingReadMessages.front();
	waitingReadMessages.pop();

	return nextInfo;
}

void NetClient::SendMessageToServer(const char* msgData, uint32_t msgByteCount)
{
	std::unique_lock<std::shared_mutex> lock(loopMutex);

	TCPSocketPtr clientSocket = GetSocket();

	short packageSize = msgByteCount;
	if (packageSize <= 0) { DebugEngineTrap(); return; }

	int retResult = clientSocket->Send(&packageSize, sizeof(packageSize));
	if (retResult < 0)
	{
		std::string logMsg = "Failed to send a size of a message to a server. Error code: " + std::to_string(-retResult) + ".";
		Logger::WriteThreadSafe(logMsg.c_str());
		return;
	}

	int bytesSent = clientSocket->Send(msgData, packageSize);
	if (bytesSent < 0)
	{
		int errorCode = -bytesSent;
		std::string message = "Failed to send a message to the server. Error code: " + std::to_string(errorCode) + ".";
		Logger::WriteThreadSafe(message.c_str());
	}
}

/*
	Other.
*/

void NetClient::ClearMessages()
{
	std::unique_lock<std::shared_mutex> lock(dataAccessMutex);

	while (!waitingReadMessages.empty())
	{
		delete waitingReadMessages.front();
		waitingReadMessages.pop();
	}
}
