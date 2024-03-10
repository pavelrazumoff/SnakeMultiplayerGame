#pragma once

#include "SocketHandler.h"

#include "ListenServer.h"
#include "NetClient.h"

#include "Engine/Events/EventDelegate.h"

DECLARE_EVENT_DELEGATE(ServerResponseDelegate);

enum PacketType
{
	PT_Hello,
	PT_Denied,
	PT_ReplicationData,
	PT_Disconnect,

	PT_MAX,
};

class PlayerState;
class ServerReplicationValidation;

class NetworkManager
{
private:
	NetworkManager();

public:
	~NetworkManager();

	static NetworkManager& GetInstance();

	void Initialize();
	void Update();

	void Cleanup();

	/** Listen Server. */

	bool MakeListenServer();
	void SendPackageToObjOwnerClient(uint32_t objNetworkId, const OutputMemoryBitStream& outStream);

	bool IsServer() const;

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

	void DoSayHello(PlayerState* clientState);
	void DoTeleportToHostLevel(const NetworkState::ClientNetStateWrapper* client);
	void DoSayGoodbye(const NetworkState::ClientNetStateWrapper* client);

	void MakeAndPushServerPackage(TCPSocketPtr clientSocket, const OutputMemoryBitStream& outStream);

	/** Client. */

	void StartClientLoop();
	void StopClientLoop();

	void ReadClientMessages();

protected:

	/** Listen Server. */

	void ProcessNewClient(const NetworkState::RawClientStateInfo& clientInfo);
	void ProcessClientDisconnected(const NetworkState::RawClientStateInfo& clientInfo);
	void ProcessClientPackage(NetworkState::RawClientPackageStateInfo& clientPackageInfo);

	/** Client. */

	void ProcessServerPackage(NetworkState::RawServerPackageStateInfo& serverPackageInfo);
	void DoAcceptServerHelloMessage(InputMemoryBitStream* inStream);

protected:
	ServerResponseDelegate JoinServerSuccessEvent;
	ServerResponseDelegate JoinServerFailureEvent;

private:
	std::shared_ptr<ListenServer> listenServerObj;
	std::shared_ptr<NetClient> netClientObj;

	std::unique_ptr<ServerReplicationValidation> serverValidation;
};
