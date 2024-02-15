#include "NetworkManager.h"

#include "Socket/SocketUtil.h"
#include "Socket/SocketFactory.h"

#include "Engine/Log/Logger.h"
#include "Engine/EngineUtility.h"

#include "Engine/Player/PlayerManager.h"

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

void NetworkManager::Update()
{
	UpdateListenServerEvents();
	UpdateClientEvents();
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

void NetworkManager::UpdateListenServerEvents()
{
	if (!IsServer()) return;

	while (auto clientInfo = listenServerObj->PopWaitingHandleClient())
	{
		switch (clientInfo->GetState())
		{
			case ClientState::Connected:
				{
					std::string logMsg = "New client connected: " + clientInfo->GetAddress().ToString();
					Logger::GetInstance().Write(logMsg.c_str());

					ProcessNewClient(clientInfo);
				}
				break;
			case ClientState::Disconnected:
				{
					std::string logMsg = "Client disconnected: " + clientInfo->GetAddress().ToString();
					if (clientInfo->GetErrorCode() > 0)
						logMsg += " with error code " + std::to_string(clientInfo->GetErrorCode());

					Logger::GetInstance().Write(logMsg.c_str());

					ProcessClientDisconnected(clientInfo);
				}
				break;
			default:
				break;
		}
	}
}

/*
	Client.
*/

void NetworkManager::UpdateClientEvents()
{
}

/*

*/

void NetworkManager::ProcessNewClient(const ClientInfo* clientInfo)
{
	auto newPlayerState = PlayerManager::GetInstance().MakeNewPlayer();
	if (!newPlayerState) { DebugEngineTrap(); return; }

	newPlayerState->SetNetPlayerInfo(clientInfo);
	newPlayerState->SetPlayerName(clientInfo->GetAddress().ToString().c_str());

	PlayerManager::GetInstance().NotifyAboutPlayerListChange();
}

void NetworkManager::ProcessClientDisconnected(const ClientInfo* clientInfo)
{
	uint32_t numPlayers = PlayerManager::GetInstance().GetPlayerCount();
	for (uint32_t i = 0; i < numPlayers; ++i)
	{
		auto playerState = PlayerManager::GetInstance().GetPlayerState(i);
		if (playerState->GetNetPlayerInfo()->GetAddress() == clientInfo->GetAddress())
		{
			PlayerManager::GetInstance().DestroyPlayer(i);
			break;
		}
	}

	PlayerManager::GetInstance().NotifyAboutPlayerListChange();
}
