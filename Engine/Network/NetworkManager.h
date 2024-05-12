#pragma once

#include "SocketHandler.h"

#include "ListenServer.h"
#include "NetClient.h"

#include "Engine/Events/EventDelegate.h"

namespace NetworkState {
	struct ClientNetStateWrapper;
	struct RawClientStateInfo;
} // namespace NetworkState

DECLARE_EVENT_DELEGATE(ServerResponseDelegate);

DECLARE_EVENT_DELEGATE(NewClientDelegate, std::shared_ptr<NetworkState::ClientNetStateWrapper>);
DECLARE_EVENT_DELEGATE(ClientDisconnectedDelegate, const NetworkState::RawClientStateInfo&);

enum PacketType
{
	PT_Hello,
	PT_Denied,
	PT_ReplicationData,
	PT_Disconnect,

	PT_MAX,
};

enum ReplicationQueueAction
{
	RQA_Create,
	RQA_Update,
	RQA_Destroy,
};

class PlayerController;
class ServerReplicationValidation;
class IReplicationObject;

struct ReplicationQueueItem
{
	IReplicationObject* obj;
	ReplicationQueueAction action;
};

class NetworkManager
{
private:
	NetworkManager();

public:
	~NetworkManager();

	static NetworkManager& GetInstance();

	void Initialize();
	void Update();
	void Replicate();

	void Cleanup();

	/** Listen Server. */

	bool MakeListenServer();
	void SendPackageToObjOwnerClient(uint32_t objNetworkId, const OutputMemoryBitStream& outStream);

	void PutInReplicationQueue(IReplicationObject* obj, ReplicationQueueAction action = RQA_Update);

	void ReplicateCreateObjectListPacket(PacketType type,
		std::vector<IReplicationObject*> objList, const NetworkState::ClientNetStateWrapper* netOwner, bool bRegisterOwnership = true);
	void MakeAndPushServerPackage(TCPSocketPtr clientSocket, const OutputMemoryBitStream& outStream);

	bool IsServer() const;

	NewClientDelegate& OnNewClientConnected();
	ClientDisconnectedDelegate& OnClientDisconnected();

	/** Client. */

	bool JoinServer(std::string server_addr);
	void SendPackageToServer(const OutputMemoryBitStream& outStream);

	bool IsClient() const;

	ServerResponseDelegate& OnJoinServerSuccess();
	ServerResponseDelegate& OnJoinServerFailure();

protected:

	/** RPC. */

	void RegisterRPCs();

	/** Listen Server. */

	void StartListenServer();
	void StopListenServer();

	void ReadServerMessages();

	/** Client. */

	void StartClientLoop();
	void StopClientLoop();

	void ReadClientMessages();

protected:

	/** Listen Server. */

	void ProcessNewClient(const NetworkState::RawClientStateInfo& clientInfo);
	void ProcessClientDisconnected(const NetworkState::RawClientStateInfo& clientInfo);
	void ProcessClientPackage(NetworkState::RawClientPackageStateInfo& clientPackageInfo);

	void ReplicateObjectsChanges();

	/** Client. */

	void ProcessServerPackage(NetworkState::RawServerPackageStateInfo& serverPackageInfo);
	void DoAcceptServerHelloMessage(InputMemoryBitStream* inStream);

protected:
	ServerResponseDelegate JoinServerSuccessEvent;
	ServerResponseDelegate JoinServerFailureEvent;

	NewClientDelegate NewClientConnectedEvent;
	ClientDisconnectedDelegate ClientDisconnectedEvent;

private:
	std::shared_ptr<ListenServer> listenServerObj;
	std::shared_ptr<NetClient> netClientObj;

	std::unique_ptr<ServerReplicationValidation> serverValidation;

	std::vector<ReplicationQueueItem> replicationQueue;
	std::vector<std::shared_ptr<NetworkState::ClientNetStateWrapper>> clientNetStates;
	std::vector<std::shared_ptr<NetworkState::ClientNetStateWrapper>> newClientNetStates;
};
