#pragma once

#include "Socket/TCPSocket.h"

class SocketHandler
{
public:
	SocketHandler(TCPSocketPtr _socket) : socket(_socket)
	{}
	virtual ~SocketHandler() {}

	int GetSocketAddress(const TCPSocket* Socket, SocketAddressPtr& outAddress);

protected:
	TCPSocketPtr GetSocket() const { return socket; }

protected:
	TCPSocketPtr socket;
};
