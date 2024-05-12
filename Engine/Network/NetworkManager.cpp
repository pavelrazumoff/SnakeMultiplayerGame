#include "NetworkManager.h"

#include "Socket/SocketUtil.h"
#include "Socket/SocketFactory.h"
#include "Replication/ObjectCreationRegistry.h"

#include "Engine/Log/Logger.h"
#include "Engine/EngineUtility.h"

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

void NetworkManager::Replicate()
{
	ReplicateObjectsChanges();
}

void NetworkManager::Cleanup()
{
	StopListenServer();
	StopClientLoop();

	WSACleanup();
}


/*
	RPC.

	Left it here for reference.
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

void NetworkManager::ReplicateObjectsChanges()
{
	for (auto clientNetState : clientNetStates)
	{
		OutputMemoryBitStream outStream;
		PacketType type = PacketType::PT_ReplicationData;
		outStream.Serialize(type, NetworkUtilityLibrary::GetRequiredBits<PacketType::PT_MAX>());

		bool bReplicatedAny = false;
		auto newClientIt = std::find(newClientNetStates.begin(), newClientNetStates.end(), clientNetState);
		bool bNewClient = (newClientIt != newClientNetStates.end());
		if (bNewClient)
		{
			newClientNetStates.erase(newClientIt);
		}

		for (auto item : replicationQueue)
		{
			IReplicationObject* obj = item.obj;
			if (!obj ||
				(!bNewClient && obj->GetReplicationPropertyMask() == 0)) continue;

			const bool bOwner = serverValidation->ValidateObjectOwnership(
				ReplicationManager::GetInstance().GetNetworkIdForObject(obj), clientNetState->GetSocket());

			const ReplicationType replicationType = bOwner ? ReplicationType::Owner : ReplicationType::NotOwner;

			OutputMemoryBitStream objOutStream;

			ReplicationQueueAction replAction = item.action;
			if (bNewClient) replAction = RQA_Create;

			switch (replAction)
			{
				case RQA_Create:
					{
						ReplicationManager::GetInstance().ReplicateCreate(objOutStream, obj, replicationType);
						outStream << objOutStream;
						bReplicatedAny = true;
					}
					break;
				case RQA_Update:
					if (ReplicationManager::GetInstance().ReplicateUpdate(objOutStream, obj, replicationType))
					{
						outStream << objOutStream;
						bReplicatedAny = true;
					}
					break;
				case RQA_Destroy:
					{
						ReplicationManager::GetInstance().ReplicateDestroy(objOutStream, obj);
						outStream << objOutStream;
						bReplicatedAny = true;
					}
					break;
				default:
					break;
			}
		}

		if (bReplicatedAny)
		{
			ReplicationManager::GetInstance().CloseReplicationPackage(outStream);
			MakeAndPushServerPackage(clientNetState->GetSocket(), outStream);
		}
	}

	replicationQueue.clear();
}

void NetworkManager::PutInReplicationQueue(IReplicationObject* obj, ReplicationQueueAction action)
{
	if (!obj) return;

	// TODO: Potentially expensive operation. But we still need to do the search because we could put some objects earlier (e.g. when created them - the order matters here).
	//if (std::find(replicationQueue.begin(), replicationQueue.end(), obj) == replicationQueue.end())
	//	replicationQueue.push_back(obj);

	auto it = std::find_if(replicationQueue.begin(), replicationQueue.end(),
		[obj](const ReplicationQueueItem& item)
		{
			return item.obj == obj;
		});

	if (it != replicationQueue.end()) return;

	replicationQueue.push_back({ obj, action });
}

void NetworkManager::ReplicateCreateObjectListPacket(PacketType type,
	std::vector<IReplicationObject*> objList, const NetworkState::ClientNetStateWrapper* netOwner, bool bRegisterOwnership)
{
	if (!IsServer() || !netOwner) return;

	// TODO: Rewrite.
	OutputMemoryBitStream outStream;
	outStream.Serialize(type, NetworkUtilityLibrary::GetRequiredBits<PacketType::PT_MAX>());

	ReplicationType replicationType = ReplicationType::Owner;

	for (auto obj : objList)
	{
		ReplicationManager::GetInstance().ReplicateCreate(outStream, obj, replicationType);

		if (bRegisterOwnership)
		{
			serverValidation->RegisterObjectOwnershipForClient(
				ReplicationManager::GetInstance().GetNetworkIdForObject(obj), netOwner->GetSocket());
		}
	}

	ReplicationManager::GetInstance().CloseReplicationPackage(outStream);
	MakeAndPushServerPackage(netOwner->GetSocket(), outStream);
}

void NetworkManager::MakeAndPushServerPackage(TCPSocketPtr clientSocket, const OutputMemoryBitStream& outStream)
{
	using namespace NetworkState;
	Server2ClientPackage* package = new Server2ClientPackage(clientSocket, outStream.GetBufferPtr(), outStream.GetByteLength());

	listenServerObj->PushSendMessageToClient(package);
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

NewClientDelegate& NetworkManager::OnNewClientConnected()
{
	return NewClientConnectedEvent;
}

ClientDisconnectedDelegate& NetworkManager::OnClientDisconnected()
{
	return ClientDisconnectedEvent;
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
	auto clientNetState = std::make_shared<ClientNetStateWrapper>(clientInfo.GetSocket(), clientInfo.GetAddress());
	clientNetStates.push_back(clientNetState);
	newClientNetStates.push_back(clientNetState);

	NewClientConnectedEvent.Trigger(clientNetState);
}

void NetworkManager::ProcessClientDisconnected(const NetworkState::RawClientStateInfo& clientInfo)
{
	ClientDisconnectedEvent.Trigger(clientInfo);

	auto it = std::find_if(clientNetStates.begin(), clientNetStates.end(),
		[&clientInfo](const std::shared_ptr<NetworkState::ClientNetStateWrapper>& clientNetState)
		{
			return clientNetState->GetSocket() == clientInfo.GetSocket();
		});

	if (it != clientNetStates.end())
		clientNetStates.erase(it);
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
			// TODO.
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
