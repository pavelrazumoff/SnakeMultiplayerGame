#include "NetworkEngineUtility.h"

ClientInfo::ClientInfo(TCPSocketPtr socket, SocketAddressPtr socketAddress, ClientState state)
{
	clientSocket = socket;
	clientAddress = socketAddress;
	clientState = state;
}
