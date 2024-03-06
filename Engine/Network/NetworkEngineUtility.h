#pragma once

#include "Socket/TCPSocket.h"

namespace NetworkUtility {

inline constexpr uint32_t MAX_PACKET_SIZE = 1470;

bool IsServer();
bool IsClient();

} // namespace NetworkUtility

class InputMemoryBitStream;

namespace NetworkState {

/*
	Receive data from client.
*/

enum class ERawClientState
{
	None,

	Connected,
	Disconnected,

	DataReceived,
};

struct RawClientStateInfo
{
public:
	RawClientStateInfo(TCPSocketPtr socket, SocketAddressPtr socketAddress, ERawClientState state);
	virtual ~RawClientStateInfo() {}

	void SetErrorCode(int code) { errorCode = code; }

	const TCPSocketPtr& GetSocket() const { return clientSocket; }
	const SocketAddressPtr& GetAddress() const { return clientAddress; }
	ERawClientState GetState() const { return clientState; }

	int GetErrorCode() const { return errorCode; }

private:
	TCPSocketPtr clientSocket;
	SocketAddressPtr clientAddress;

	ERawClientState clientState = ERawClientState::None;

	int errorCode = 0;
};

struct RawClientPackageStateInfo : public RawClientStateInfo
{
public:
	RawClientPackageStateInfo(TCPSocketPtr socket, SocketAddressPtr socketAddress, ERawClientState state) :
		RawClientStateInfo(socket, socketAddress, state) {}
	virtual ~RawClientPackageStateInfo();

	void SetData(const char* inBuffer, uint32_t inByteCount);

	InputMemoryBitStream* GetStream();

private:
	InputMemoryBitStream* inputBitStream = nullptr;
};

/*
	Send data to client.
*/

struct Server2ClientPackage
{
public:
	Server2ClientPackage(TCPSocketPtr _clientSocket, const char* _packageData, uint32_t _packageByteCount);
	~Server2ClientPackage();

	TCPSocketPtr GetClientSocket() const { return clientSocket; }
	const char* GetPackageData() const { return packageData; }
	uint32_t GetPackageByteCount() const { return packageByteCount; }

private:
	TCPSocketPtr clientSocket;

	char* packageData = nullptr;
	uint32_t packageByteCount = 0;
};

/*
	Wrapper for connected client's socket and address.
*/

struct ClientNetStateWrapper
{
	ClientNetStateWrapper(TCPSocketPtr socket, SocketAddressPtr socketAddress) :
		clientSocket(socket),
		clientAddress(socketAddress) {}

	const TCPSocketPtr& GetSocket() const { return clientSocket; }
	const SocketAddressPtr& GetAddress() const { return clientAddress; }

private:
	TCPSocketPtr clientSocket;
	SocketAddressPtr clientAddress;
};

/*
	Receive data from server.
*/

struct RawServerPackageStateInfo
{
public:
	RawServerPackageStateInfo(const char* inBuffer, uint32_t inByteCount);
	~RawServerPackageStateInfo();

	InputMemoryBitStream* GetStream();

private:
	InputMemoryBitStream* inputBitStream = nullptr;
};

// ------------------------------------------------------------------

} // namespace NetworkState
