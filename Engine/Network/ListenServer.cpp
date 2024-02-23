#include "ListenServer.h"

#include "Socket/SocketUtil.h"
#include "Engine/EngineUtility.h"
#include "Engine/Log/Logger.h"

ListenServer::ListenServer(TCPSocketPtr _listenSocket)
	: Inherited(_listenSocket)
	, inputBitStream(new InputMemoryBitStream(NetworkUtility::MAX_PACKET_SIZE))
{
}

ListenServer::~ListenServer()
{
	ClearMessages();
}

void ListenServer::StartListen()
{
	if (bIsServerRunning ||
		!GetSocket()) return;

	connectedClients.clear();

	bIsServerRunning = true;
	handleListenThread = std::thread(&ListenServer::ListenThread, this);
}

void ListenServer::StopListen()
{
	if (!bIsServerRunning) return;

	bIsServerRunning = false;

	// TODO: Notify all clients that server is shutting down.
	// Do a callback to print some info that server is shutting down while we're waiting.

	if (handleListenThread.joinable())
		handleListenThread.join();

	if (auto listenSocket = GetSocket())
		listenSocket->Shutdown();
}

/*
	Threads.
*/

void ListenServer::ListenThread()
{
	using namespace std;

	TCPSocketPtr listenSocket = GetSocket();

	vector<TCPSocketPtr> readBlockSockets; /* Sockets to listen to. */
	readBlockSockets.push_back(listenSocket);

	vector<TCPSocketPtr> readableSockets; /* Sockets that are ready to read. */

	vector<TCPSocketPtr> writeBlockSockets; /* Sockets to which we want to write. */
	vector<TCPSocketPtr> writableSockets; /* Sockets that are ready to write. */

	auto REMOVE_SOCKET_FROM_SELECT = [](vector<TCPSocketPtr>& socketVec, const TCPSocketPtr& socket) -> void {
		
		socketVec.erase(find(socketVec.begin(), socketVec.end(), socket));
	};

	/** Waiting for some data no more than 5 seconds in a blocking mode. */
	timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	while (bIsServerRunning)
	{
		if (SocketUtil::Select<TCPSocketPtr>(&readBlockSockets, &readableSockets,
			&writeBlockSockets, &writableSockets,
			nullptr, nullptr,
			&timeout))
		{
			std::unique_lock<std::shared_mutex> lock(loopMutex);

			// Go through all readable sockets.
			// If there is at least one socket, it means that we have a new message from a client.
			for (const auto& socket : readableSockets)
			{
				if (socket == listenSocket) // Listen socket got a new request for connection.
				{
					SocketAddress newClientAddress;
					auto newSocket = socket->Accept(newClientAddress);
					readBlockSockets.push_back(newSocket);

					ProcessNewClient(newSocket, newClientAddress);
					UpdateWritableSocketsFromConnectedClients(writeBlockSockets);
				}
				else // Client socket sent a message.
				{
					inputBitStream->Reset();

					int32_t bytesReceived = socket->Receive(inputBitStream->GetBufferPtr(), inputBitStream->GetCapacity());
					if (bytesReceived > 0)
					{
						ProcessClientDataReceived(socket, bytesReceived);
					}
					else if (bytesReceived == 0)
					{
						SocketAddressPtr clientAddress;
						GetSocketAddress(socket.get(), clientAddress);

						std::string logMsg = "Received 0 bytes from client (" + clientAddress->ToString() + "). Disconnecting...";
						Logger::GetInstance().Write(logMsg.c_str());

						ProcessClientDisconnected(socket);
						UpdateWritableSocketsFromConnectedClients(writeBlockSockets);

						REMOVE_SOCKET_FROM_SELECT(readBlockSockets, socket);
						REMOVE_SOCKET_FROM_SELECT(writableSockets, socket);
					}
					else
					{
						int errorCode = -bytesReceived;
						if (!ProcessClientError(errorCode, socket))
						{
							UpdateWritableSocketsFromConnectedClients(writeBlockSockets);

							REMOVE_SOCKET_FROM_SELECT(readBlockSockets, socket);
							REMOVE_SOCKET_FROM_SELECT(writableSockets, socket);
						}
					}
				}
			}

			for (const auto& socket : writableSockets)
			{
				ProcessSendPackageToClient(socket);
			}
		}
	}
}

/*
	Handle clients.
*/

void ListenServer::ProcessNewClient(TCPSocketPtr newClientSocket, const SocketAddress& newClientAddress)
{
	std::unique_lock<std::shared_mutex> lock(dataAccessMutex);

	using namespace NetworkState;

	connectedClients.push_back(newClientSocket);

	SocketAddressPtr clientAddress = std::make_shared<SocketAddress>(newClientAddress.GetAsSockAddr());
	waitingReadMessages.push(new RawClientStateInfo(newClientSocket, clientAddress, ERawClientState::Connected));
}

void ListenServer::ProcessClientDisconnected(TCPSocketPtr clientSocket, int errCode)
{
	std::unique_lock<std::shared_mutex> lock(dataAccessMutex);

	using namespace NetworkState;

	connectedClients.erase(
		std::remove(connectedClients.begin(), connectedClients.end(), clientSocket), connectedClients.end());

	SocketAddressPtr clientAddress;
	GetSocketAddress(clientSocket.get(), clientAddress);

	waitingReadMessages.push(new RawClientStateInfo(nullptr, clientAddress, ERawClientState::Disconnected));
	if (errCode > 0)
		waitingReadMessages.back()->SetErrorCode(errCode);

	clientSocket->Shutdown();
}

bool ListenServer::ProcessClientError(int errorCode, TCPSocketPtr clientSocket)
{
	switch (errorCode)
	{
		case WSAECONNRESET:
		case WSAENETRESET:
		case WSAETIMEDOUT:
		{
			ProcessClientDisconnected(clientSocket, errorCode);
			return false;
		}
		default:
			break;
	}

	return true;
}

void ListenServer::ProcessClientDataReceived(TCPSocketPtr clientSocket, int32_t bytesReceived)
{
	std::string logMsg = "Received a message from a client. Bytes received: " + std::to_string(bytesReceived) + ".";
	Logger::GetInstance().Write(logMsg.c_str());

	std::unique_lock<std::shared_mutex> lock(dataAccessMutex);

	using namespace NetworkState;

	RawClientPackageStateInfo* clientDataInfo = new RawClientPackageStateInfo(clientSocket, nullptr, ERawClientState::DataReceived);
	clientDataInfo->SetData(inputBitStream->GetBufferPtr(), bytesReceived);

	waitingReadMessages.push(clientDataInfo);
}

void ListenServer::ProcessSendPackageToClient(TCPSocketPtr clientSocket)
{
	std::unique_lock<std::shared_mutex> lock(dataAccessMutex);

	using namespace NetworkState;

	if (waitingSendMessages.empty()) return;

	auto range = waitingSendMessages.equal_range(clientSocket);
	if (range.first == waitingSendMessages.end()) return; // Nothing to send.

	for (auto it = range.first; it != range.second; ++it)
	{
		Server2ClientPackage* nextPackage = it->second;
		if (!nextPackage) { DebugEngineTrap(); continue; }

		std::string logMsg;
		int retResult = clientSocket->Send(
			reinterpret_cast<const void*>(nextPackage->GetPackageData()), nextPackage->GetPackageByteCount());
		if (retResult < 0)
			logMsg = "Failed to send a message to a client. Error code: " + std::to_string(-retResult) + ".";
		else
			logMsg = "Sent a message to a client. Bytes sent: " + std::to_string(retResult) + ".";

		Logger::GetInstance().Write(logMsg.c_str());
		delete it->second;
	}

	waitingSendMessages.erase(range.first, range.second);
}

NetworkState::RawClientStateInfo* ListenServer::PopWaitingClientMessage()
{
	std::unique_lock<std::shared_mutex> lock(dataAccessMutex);

	using namespace NetworkState;

	if (waitingReadMessages.empty()) return nullptr;

	RawClientStateInfo* nextInfo = waitingReadMessages.front();
	waitingReadMessages.pop();

	return nextInfo;
}

void ListenServer::PushSendMessageToClient(NetworkState::Server2ClientPackage* package)
{
	std::unique_lock<std::shared_mutex> lock(dataAccessMutex);

	waitingSendMessages.insert(std::make_pair(package->GetClientSocket(), package));
}

void ListenServer::UpdateWritableSocketsFromConnectedClients(std::vector<TCPSocketPtr>& outClientSockets)
{
	outClientSockets.clear();

	outClientSockets.reserve(connectedClients.size());
	for (const auto& client : connectedClients)
		if (client) outClientSockets.push_back(client);
}

/*
	Other.
*/

void ListenServer::ClearMessages()
{
	std::unique_lock<std::shared_mutex> lock(dataAccessMutex);

	while (!waitingReadMessages.empty())
	{
		delete waitingReadMessages.front();
		waitingReadMessages.pop();
	}

	for (auto& pair : waitingSendMessages)
		delete pair.second;

	waitingSendMessages.clear();
}
