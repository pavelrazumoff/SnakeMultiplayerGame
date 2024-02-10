#include "ListenServer.h"

#include "Socket/SocketUtil.h"
#include "Engine/EngineUtility.h"

ListenServer::ListenServer(TCPSocketPtr _listenSocket)
	: Inherited(_listenSocket)
	, inputBitStream(new InputMemoryBitStream(NetworkUtility::MAX_PACKET_SIZE))
{
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

					int32_t bytesRead = socket->Receive(inputBitStream->GetBufferPtr(), inputBitStream->GetCapacity());
					if (bytesRead > 0)
					{
						// TODO.
					}
					else if (bytesRead == 0)
					{
						ProcessClientDisconnected(socket);
						UpdateWritableSocketsFromConnectedClients(writeBlockSockets);

						REMOVE_SOCKET_FROM_SELECT(readBlockSockets, socket);
						REMOVE_SOCKET_FROM_SELECT(writableSockets, socket);
					}
					else
					{
						int errorCode = -bytesRead;
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
				SyncWithClient(socket);
			}
		}
	}
}

/*
	Handle clients.
*/

void ListenServer::ProcessNewClient(TCPSocketPtr newClientSocket, const SocketAddress& newClientAddress)
{
	connectedClients.push_back(std::make_shared<ClientInfo>(newClientSocket));

	ClientStateChangedEvent.Trigger(connectedClients.back().get(), ClientState::Connected);
}

void ListenServer::ProcessClientDisconnected(TCPSocketPtr clientSocket)
{
	auto foundClient = FindClientInfoBySocket(clientSocket);
	auto clientInfo = *foundClient;

	if (foundClient != connectedClients.end())
	{
		connectedClients.erase(foundClient);
	}
	else
		DebugEngineTrap();
	
	ClientStateChangedEvent.Trigger(clientInfo.get(), ClientState::Disconnected);

	clientSocket->Shutdown();
}

bool ListenServer::ProcessClientError(int errorCode, TCPSocketPtr clientSocket)
{
	ClientErrorEvent.Trigger(FindClientInfoBySocket(clientSocket)->get(), errorCode);

	switch (errorCode)
	{
		case WSAECONNRESET:
		case WSAENETRESET:
		case WSAETIMEDOUT:
		{
			ProcessClientDisconnected(clientSocket);
			return false;
		}
		default:
			break;
	}

	return true;
}

void ListenServer::SyncWithClient(TCPSocketPtr ClientSocket)
{
	// Every client has its own data info on the server.
	auto foundClient = FindClientInfoBySocket(ClientSocket);
	if (foundClient == connectedClients.end())
	{
		DebugEngineTrap();
		return;
	}

	// TODO: Sync player state first.
}

void ListenServer::UpdateWritableSocketsFromConnectedClients(std::vector<TCPSocketPtr>& outClientSockets)
{
	outClientSockets.clear();

	outClientSockets.reserve(connectedClients.size());
	for (const auto& client : connectedClients)
		if (client) outClientSockets.push_back(client->clientSocket);
}

std::vector<std::shared_ptr<ClientInfo>>::iterator ListenServer::FindClientInfoBySocket(TCPSocketPtr socket)
{
	return std::find_if(connectedClients.begin(), connectedClients.end(),
		[&socket](const auto& client)
		{
			return client->clientSocket == socket;
		});
}

const std::vector<std::shared_ptr<ClientInfo>>::const_iterator ListenServer::FindClientInfoBySocket(TCPSocketPtr socket) const
{
	return std::find_if(connectedClients.cbegin(), connectedClients.cend(),
		[&socket](const auto& client)
		{
			return client->clientSocket == socket;
		});
}
