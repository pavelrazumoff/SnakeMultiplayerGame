#pragma once

#include "Socket/TCPSocket.h"

namespace NetworkUtility {

inline constexpr uint32_t MAX_PACKET_SIZE = 1470;

} // namespace NetworkUtility

enum class ClientState
{
	None,

	Connected,
	Disconnected,

	DataReceived,
};

struct ClientInfo
{
public:
	ClientInfo(TCPSocketPtr socket, SocketAddressPtr socketAddress, ClientState state);
	virtual ~ClientInfo() {}

	void SetErrorCode(int code) { errorCode = code; }

	const TCPSocketPtr& GetSocket() const { return clientSocket; }
	const SocketAddress& GetAddress() const { return *clientAddress; }
	ClientState GetState() const { return clientState; }

	int GetErrorCode() const { return errorCode; }

private:
	TCPSocketPtr clientSocket;
	SocketAddressPtr clientAddress;

	ClientState clientState = ClientState::None;

	int errorCode = 0;
};

class InputMemoryBitStream;

struct ClientDataInfo : public ClientInfo
{
public:
	ClientDataInfo(TCPSocketPtr socket, SocketAddressPtr socketAddress, ClientState state) :
		ClientInfo(socket, socketAddress, state) {}
	virtual ~ClientDataInfo();

	void SetData(const char* inBuffer, uint32_t inBitCount);

private:
	InputMemoryBitStream* inputBitStream = nullptr;
};
