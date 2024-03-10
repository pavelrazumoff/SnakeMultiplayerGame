#include "NetworkManager.h"

#include "Socket/SocketUtil.h"
#include "Socket/SocketFactory.h"

#include "Engine/Log/Logger.h"
#include "Engine/EngineUtility.h"

#include "Engine/Player/PlayerManager.h"
#include "Engine/Level/LevelManager.h"

#include "Replication/ReplicationManager.h"
#include "Replication/RPCManager.h"
#include "Replication/ReplicationHeader.h"

#include "NetworkValidation.h"

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
		Logger::WriteThreadSafe(errMsg.c_str());
		return;
	}

	RegisterRPCs();
}

void NetworkManager::Update()
{
	ReadServerMessages();
	ReadClientMessages();
}

void NetworkManager::Cleanup()
{
	StopListenServer();
	StopClientLoop();

	WSACleanup();
}


/*
	RPC.
*/

/*
static void RPC_OpenHostLevel(OutputMemoryBitStream& outStream, GameLevel* level)
{
	ReplicationHeader rh(ReplicationAction::RA_RPC);
	rh.Write(outStream);

	uint32_t rpcId = GET_RPC_FUNC_ID(Unwrap_OpenHostLevel);
	uint32_t levelId = ReplicationManager::GetInstance().GetNetworkIdForObject(level);

	outStream.Serialize(rpcId);
	outStream.Serialize(levelId);
}

static void Unwrap_OpenHostLevel(InputMemoryBitStream& inStream)
{
	uint32_t levelId = 0;
	inStream.Serialize(levelId);
	
	GameLevel* level = dynamic_cast<GameLevel*>(
		ReplicationManager::GetInstance().GetObjectFromNetworkId(levelId));

	if (level != nullptr)
		LevelManager::GetInstance().OpenLevel(level);
	else
	{
		Logger::WriteThreadSafe("Failed to open the level. Level not found.");
		DebugEngineTrap();
	}
}
*/

void NetworkManager::RegisterRPCs()
{
	//REGISTER_RPC_FUNC(Unwrap_OpenHostLevel);
}

/*
	Listen Server.
*/

bool NetworkManager::MakeListenServer()
{
	if (IsClient() || IsServer())
	{
		DebugEngineTrap();
		Logger::WriteThreadSafe("Socket already exists.");
		return false;
	}

	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);
	SocketAddress listenAddress = SocketAddress(INADDR_ANY, 48000);

	int errCode = listenSocket->Bind(listenAddress);
	if (NO_ERROR != errCode)
	{
		std::string errMsg = "Bind failed with error code " + std::to_string(errCode);
		Logger::WriteThreadSafe(errMsg.c_str());
		return false;
	}

	errCode = listenSocket->Listen();
	if (NO_ERROR != errCode)
	{
		std::string errMsg = "Listen failed with error code " + std::to_string(errCode);
		Logger::WriteThreadSafe(errMsg.c_str());
		return false;
	}

	listenServerObj = std::make_shared<ListenServer>(listenSocket);

	serverValidation = std::make_unique<ServerReplicationValidation>();

	StartListenServer();
	return true;
}

void NetworkManager::SendPackageToObjOwnerClient(uint32_t objNetworkId, const OutputMemoryBitStream& outStream)
{
	if (!IsServer()) return;

	TCPSocketPtr clientSocket = serverValidation->FindClientForOwnedObject(objNetworkId);
	if (!clientSocket) return;

	MakeAndPushServerPackage(clientSocket, outStream);
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

bool NetworkManager::JoinServer(std::string server_addr)
{
	if (IsClient() || IsServer())
	{
		DebugEngineTrap();
		Logger::WriteThreadSafe("Socket already exists.");
		return false;
	}

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(server_addr);
	TCPSocketPtr clientSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);

	int errCode = clientSocket->Connect(*serverAddress);
	if (NO_ERROR != errCode)
	{
		std::string errMsg = "Connect failed with error code " + std::to_string(errCode);
		Logger::WriteThreadSafe(errMsg.c_str());
		return false;
	}

	netClientObj = std::make_shared<NetClient>(clientSocket);

	StartClientLoop();
	return true;
}

void NetworkManager::SendPackageToServer(const OutputMemoryBitStream& outStream)
{
	if (!IsClient()) return;

	netClientObj->SendMessageToServer(outStream.GetBufferPtr(), outStream.GetByteLength());
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
					Logger::WriteThreadSafe(logMsg.c_str());

					ProcessNewClient(*clientInfo);
				}
				break;
			case ERawClientState::Disconnected:
				{
					std::string logMsg = "Client disconnected: " + clientInfo->GetAddress()->ToString();
					if (clientInfo->GetErrorCode() > 0)
						logMsg += " with error code " + std::to_string(clientInfo->GetErrorCode());

					Logger::WriteThreadSafe(logMsg.c_str());

					ProcessClientDisconnected(*clientInfo);
				}
				break;
			case ERawClientState::DataReceived:
				{
					if (auto packegeInfo = dynamic_cast<RawClientPackageStateInfo*>(clientInfo.get()))
						ProcessClientPackage(*packegeInfo);
				}
				break;
			default:
				break;
		}
	}
}

void NetworkManager::ProcessNewClient(const NetworkState::RawClientStateInfo& clientInfo)
{
	using namespace NetworkState;
	ClientNetStateWrapper* clientNetState = new ClientNetStateWrapper(clientInfo.GetSocket(), clientInfo.GetAddress());

	auto newPlayerState = PlayerManager::GetInstance().MakeNewPlayer(clientNetState);
	if (!newPlayerState) { DebugEngineTrap(); return; }

	DoSayHello(newPlayerState);
	DoTeleportToHostLevel(newPlayerState->GetNetPlayerInfo());

	// TODO: Replicate all player states to connected clients.

	// TODO: Replace with auto update.
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

void NetworkManager::ProcessClientPackage(NetworkState::RawClientPackageStateInfo& clientPackageInfo)
{
	InputMemoryBitStream* inStream = clientPackageInfo.GetStream();
	if (!inStream) { DebugEngineTrap(); return; }

	PacketType type = PacketType::PT_MAX;
	inStream->Serialize(type, NetworkUtilityLibrary::GetRequiredBits<PacketType::PT_MAX>());

	serverValidation->StartValidationForClient(clientPackageInfo.GetSocket());
	switch (type)
	{
		case PT_ReplicationData:
			while (inStream->GetBitCapacityLeft() > 0)
			{
				ReplicationManager::GetInstance().ProcessReplicationAction(&ObjectCreationRegistry::GetInstance(), *inStream,
					serverValidation.get());
			}
			break;
		default:
			break;
	}

	serverValidation->FinishValidation();
}

void NetworkManager::DoSayHello(PlayerState* clientState)
{
	auto netPlayerInfo = clientState ? clientState->GetNetPlayerInfo() : nullptr;
	if (!netPlayerInfo)
	{
		DebugEngineTrap(); return;
	}

	OutputMemoryBitStream outStream;

	PacketType type = PacketType::PT_Hello;
	outStream.Serialize(type, NetworkUtilityLibrary::GetRequiredBits<PacketType::PT_MAX>());

	ReplicationManager::GetInstance().ReplicateCreate(outStream, clientState);

	serverValidation->RegisterObjectOwnershipForClient(
		ReplicationManager::GetInstance().GetNetworkIdForObject(clientState), netPlayerInfo->GetSocket());

	ReplicationManager::GetInstance().CloseReplicationPackage(outStream);

	MakeAndPushServerPackage(netPlayerInfo->GetSocket(), outStream);
}

void NetworkManager::DoTeleportToHostLevel(const NetworkState::ClientNetStateWrapper* client)
{
	OutputMemoryBitStream outStream;

	PacketType type = PacketType::PT_ReplicationData;
	outStream.Serialize(type, NetworkUtilityLibrary::GetRequiredBits<PacketType::PT_MAX>());

	// Replicate the current open level to connected client.
	GameLevel* currentLevel = LevelManager::GetInstance().GetCurrentLevel();

	ReplicationManager::GetInstance().ReplicateCreate(outStream, currentLevel);
	//RPC_OpenHostLevel(outStream, currentLevel);

	ReplicationManager::GetInstance().CloseReplicationPackage(outStream);

	MakeAndPushServerPackage(client->GetSocket(), outStream);
}

void NetworkManager::DoSayGoodbye(const NetworkState::ClientNetStateWrapper* client)
{
	// TODO: If we want to force the client to disconnect.
}

void NetworkManager::MakeAndPushServerPackage(TCPSocketPtr clientSocket, const OutputMemoryBitStream& outStream)
{
	using namespace NetworkState;
	Server2ClientPackage* package = new Server2ClientPackage(clientSocket, outStream.GetBufferPtr(), outStream.GetByteLength());

	listenServerObj->PushSendMessageToClient(package);
}

/*
	Client Events.
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
		case PacketType::PT_Hello:
			DoAcceptServerHelloMessage(inStream);
			break;
		case PacketType::PT_Denied:
			JoinServerFailureEvent.Trigger();
			break;
		case PacketType::PT_ReplicationData:
			while (inStream->GetBitCapacityLeft() > 0)
			{
				ReplicationManager::GetInstance().ProcessReplicationAction(&ObjectCreationRegistry::GetInstance(), *inStream);
			}
			break;
		case PacketType::PT_Disconnect:
			break;
		default:
			JoinServerFailureEvent.Trigger();
			break;
	}
}

void NetworkManager::DoAcceptServerHelloMessage(InputMemoryBitStream* inStream)
{
	while (inStream->GetBitCapacityLeft() > 0)
	{
		ReplicationManager::GetInstance().ProcessReplicationAction(&ObjectCreationRegistry::GetInstance(), *inStream);
	}

	if (auto firstPlayerState = PlayerManager::GetInstance().GetPlayerState(0))
	{
		PlayerManager::GetInstance().SetLocalPlayerId(firstPlayerState->GetPlayerId());
	}

	JoinServerSuccessEvent.Trigger();
}

ServerResponseDelegate& NetworkManager::OnJoinServerSuccess()
{
	return JoinServerSuccessEvent;
}

ServerResponseDelegate& NetworkManager::OnJoinServerFailure()
{
	return JoinServerFailureEvent;
}
