#include "NetworkEngineUtility.h"

#include "Serialization/MemoryBitStream.h"

#include "Engine/EngineUtility.h"

ClientInfo::ClientInfo(TCPSocketPtr socket, SocketAddressPtr socketAddress, ClientState state)
{
	clientSocket = socket;
	clientAddress = socketAddress;
	clientState = state;
}

ClientDataInfo::~ClientDataInfo()
{
	if (inputBitStream) delete inputBitStream;
}

void ClientDataInfo::SetData(const char* inBuffer, uint32_t inBitCount)
{
	char* buffer = (char*)std::malloc(inBitCount);
	if (!buffer) { DebugEngineTrap(); return; }

	std::memcpy(buffer, inBuffer, inBitCount);

	if (inputBitStream) delete inputBitStream;
	inputBitStream = new InputMemoryBitStream(buffer, inBitCount);
}
