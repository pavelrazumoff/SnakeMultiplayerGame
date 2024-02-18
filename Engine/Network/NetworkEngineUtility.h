#pragma once

#include "Socket/TCPSocket.h"

namespace NetworkUtility {

inline constexpr uint32_t MAX_PACKET_SIZE = 1470;

} // namespace NetworkUtility

class InputMemoryBitStream;

namespace NetworkState {

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

	void SetData(const char* inBuffer, uint32_t inBitCount);

private:
	InputMemoryBitStream* inputBitStream = nullptr;
};

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

// ------------------------------------------------------------------

} // namespace NetworkState
