#include "NetworkManager.h"

#include "Socket/SocketUtil.h"
#include "Socket/SocketFactory.h"

#include "Engine/Log/Logger.h"
#include "Engine/EngineUtility.h"

#include "Engine/Player/PlayerManager.h"
#include "Replication/ReplicationManager.h"

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
	ReadServerMessages();
	ReadClientMessages();

	DoReplication();
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

void NetworkManager::ReadServerMessages()
{
	if (!IsServer()) return;

	using namespace NetworkState;

	while (true)
	{
		std::unique_ptr<RawClientStateInfo> clientInfo(listenServerObj->PopWaitingClientMessage());
		if (!clientInfo) break;

		switch (clientInfo->GetState())
		{
			case ERawClientState::Connected:
				{
					std::string logMsg = "New client connected: " + clientInfo->GetAddress()->ToString();
					Logger::GetInstance().Write(logMsg.c_str());

					ProcessNewClient(*clientInfo);
				}
				break;
			case ERawClientState::Disconnected:
				{
					std::string logMsg = "Client disconnected: " + clientInfo->GetAddress()->ToString();
					if (clientInfo->GetErrorCode() > 0)
						logMsg += " with error code " + std::to_string(clientInfo->GetErrorCode());

					Logger::GetInstance().Write(logMsg.c_str());

					ProcessClientDisconnected(*clientInfo);
				}
				break;
			case ERawClientState::DataReceived:
				{
					// TODO.
				}
				break;
			default:
				break;
		}
	}
}

void NetworkManager::ProcessNewClient(const NetworkState::RawClientStateInfo& clientInfo)
{
	auto newPlayerState = PlayerManager::GetInstance().MakeNewPlayer();
	if (!newPlayerState) { DebugEngineTrap(); return; }

	using namespace NetworkState;
	ClientNetStateWrapper* clientNetState = new ClientNetStateWrapper(clientInfo.GetSocket(), clientInfo.GetAddress());

	newPlayerState->SetNetPlayerState(clientNetState);
	if (auto addr = clientNetState->GetAddress())
		newPlayerState->SetPlayerName(addr->ToString().c_str());

	DoSayHello(newPlayerState->GetNetPlayerInfo());

	PlayerManager::GetInstance().NotifyAboutPlayerListChange();
}

void NetworkManager::ProcessClientDisconnected(const NetworkState::RawClientStateInfo& clientInfo)
{
	uint32_t numPlayers = PlayerManager::GetInstance().GetPlayerCount();
	for (uint32_t i = 0; i < numPlayers; ++i)
	{
		auto playerState = PlayerManager::GetInstance().GetPlayerState(i);
		if (const auto netInfo = playerState ? playerState->GetNetPlayerInfo() : nullptr)
		{
			if (*netInfo->GetAddress() == *clientInfo.GetAddress())
			{
				PlayerManager::GetInstance().DestroyPlayer(i);
				break;
			}
		}
	}

	PlayerManager::GetInstance().NotifyAboutPlayerListChange();
}

void NetworkManager::DoSayHello(const NetworkState::ClientNetStateWrapper* client)
{
	OutputMemoryBitStream outStream;

	PacketType type = PacketType::PT_Hello;
	outStream.Serialize(type, NetworkUtilityLibrary::GetRequiredBits<PacketType::PT_MAX>());

	using namespace NetworkState;
	Server2ClientPackage* package = new Server2ClientPackage(client->GetSocket(), outStream.GetBufferPtr(), outStream.GetByteLength());

	listenServerObj->PushSendMessageToClient(package);
}

void NetworkManager::DoSayGoodbye(const NetworkState::ClientNetStateWrapper* client)
{
	// TODO: If we want to force the client to disconnect.
}

void NetworkManager::DoReplication()
{
	// Here we should sync the game state with the clients.
	// TODO: Check if there is something to send to the clients.

	//OutputMemoryBitStream outStream;

}

/*
	Client.
*/

void NetworkManager::ReadClientMessages()
{
	if (!IsClient()) return;

	using namespace NetworkState;

	while (true)
	{
		std::unique_ptr<RawServerPackageStateInfo> serverPackageInfo(netClientObj->PopWaitingServerMessage());
		if (!serverPackageInfo) break;

		ProcessServerPackage(*serverPackageInfo);
	}
}

void NetworkManager::ProcessServerPackage(NetworkState::RawServerPackageStateInfo& serverPackageInfo)
{
	InputMemoryBitStream* inStream = serverPackageInfo.GetStream();
	if (!inStream) { DebugEngineTrap(); return; }

	PacketType type = PacketType::PT_MAX;
	inStream->Serialize(type, NetworkUtilityLibrary::GetRequiredBits<PacketType::PT_MAX>());

	switch (type)
	{
		case PT_Hello:
			// TODO: Do some initialization.
			DebugEngineTrap();
			break;
		case PT_ReplicationData:
			break;
		case PT_Disconnect:
			break;
		default:
			DebugEngineTrap();
			break;
	}
}
