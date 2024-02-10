#pragma once

#include "Socket/TCPSocket.h"

namespace NetworkUtility {

inline constexpr uint32_t MAX_PACKET_SIZE = 1470;

} // namespace NetworkUtility

struct ClientInfo
{
public:
	ClientInfo(TCPSocketPtr socket) : clientSocket(socket) {}

public:
	TCPSocketPtr clientSocket;
};
