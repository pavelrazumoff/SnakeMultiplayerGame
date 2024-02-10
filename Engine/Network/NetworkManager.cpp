#include "NetworkManager.h"

#include "Socket/SocketUtil.h"
#include "Socket/SocketFactory.h"

#include "Engine/Log/Logger.h"
#include "Engine/EngineUtility.h"

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
}

NetworkManager& NetworkManager::GetInstance()
{
	static NetworkManager instance;
	return instance;
}

void NetworkManager::Initialize()
{
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err)
	{
		std::string errMsg = "WSAStartup failed with error code " + std::to_string(err);
		Logger::GetInstance().Write(errMsg.c_str());
		return;
	}
}

void NetworkManager::Cleanup()
{
	StopListenServer();
	StopClientLoop();

	WSACleanup();
}

/*
	Listen Server.
*/

bool NetworkManager::MakeListenServer()
{
	if (IsClient() || IsServer())
	{
		DebugEngineTrap();
		Logger::GetInstance().Write("Socket already exists.");
		return false;
	}

	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);
	SocketAddress listenAddress = SocketAddress(INADDR_ANY, 48000);

	int errCode = listenSocket->Bind(listenAddress);
	if (NO_ERROR != errCode)
	{
		std::string errMsg = "Bind failed with error code " + std::to_string(errCode);
		Logger::GetInstance().Write(errMsg.c_str());
		return false;
	}

	errCode = listenSocket->Listen();
	if (NO_ERROR != errCode)
	{
		std::string errMsg = "Listen failed with error code " + std::to_string(errCode);
		Logger::GetInstance().Write(errMsg.c_str());
		return false;
	}

	listenServerObj = std::make_shared<ListenServer>(listenSocket);
	PrepareListenServerEvents();

	return true;
}

void NetworkManager::StartListenServer()
{
	if (!IsServer()) return;

	listenServerObj->StartListen();
}

void NetworkManager::StopListenServer()
{
	if (!IsServer()) return;

	listenServerObj->StopListen();
}

bool NetworkManager::IsServer() const
{
	return listenServerObj.get() != nullptr;
}

/*
	Client.
*/

bool NetworkManager::ConnectClient(std::string server_addr)
{
	if (IsClient() || IsServer())
	{
		DebugEngineTrap();
		Logger::GetInstance().Write("Socket already exists.");
		return false;
	}

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(server_addr);
	TCPSocketPtr clientSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);

	int errCode = clientSocket->Connect(*serverAddress);
	if (NO_ERROR != errCode)
	{
		std::string errMsg = "Connect failed with error code " + std::to_string(errCode);
		Logger::GetInstance().Write(errMsg.c_str());
		return false;
	}

	netClientObj = std::make_shared<NetClient>(clientSocket);
	return true;
}

void NetworkManager::StartClientLoop()
{
	if (!IsClient()) return;

	netClientObj->StartClientLoop();
}

void NetworkManager::StopClientLoop()
{
	if (!IsClient()) return;

	netClientObj->StopClientLoop();
}

bool NetworkManager::IsClient() const
{
	return netClientObj.get() != nullptr;
}

/*
	Listen Server Events.
*/

void NetworkManager::PrepareListenServerEvents()
{
	if (!IsServer()) return;

	listenServerObj->OnClientStateChangedEvent().Subscribe(this, &NetworkManager::HandleNewClientConnected);
	listenServerObj->OnClientErrorEvent().Subscribe(this, &NetworkManager::HandleClientError);
}

void NetworkManager::HandleNewClientConnected(const ClientInfo* clientInfo, ClientState newState)
{
	if (!clientInfo) return;

	SocketAddressPtr clientAddress;
	if (0 != listenServerObj->GetSocketAddress(clientInfo->clientSocket.get(), clientAddress))
	{
		Logger::GetInstance().Write("Failed to get client address.");
		return;
	}

	switch (newState)
	{
		case ClientState::Connected:
			{
				std::string logMsg = "New client connected: " + clientAddress->ToString();
				Logger::GetInstance().Write(logMsg.c_str());
			}
			break;
		case ClientState::Disconnected:
			{
				std::string logMsg = "Client disconnected: " + clientAddress->ToString();
				Logger::GetInstance().Write(logMsg.c_str());
			}
			break;
		default:
			break;
	}
}

void NetworkManager::HandleClientError(const ClientInfo* clientInfo, int errorCode)
{
	if (!clientInfo) return;

	SocketAddressPtr clientAddress;
	if (0 != listenServerObj->GetSocketAddress(clientInfo->clientSocket.get(), clientAddress))
	{
		Logger::GetInstance().Write("Failed to get client address.");
		return;
	}

	std::string logMsg = "Client error: " + clientAddress->ToString() + " with error code " + std::to_string(errorCode);
	Logger::GetInstance().Write(logMsg.c_str());
}
