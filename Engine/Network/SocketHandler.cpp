#include "SocketHandler.h"

#include "Socket/SocketUtil.h"

int SocketHandler::GetSocketAddress(const TCPSocket* Socket, SocketAddressPtr& outAddress)
{
	// TODO: Logging.
	return SocketUtil::GetSocketAddress<TCPSocket>(Socket, outAddress);
}
