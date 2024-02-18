#include "NetworkEngineUtility.h"

#include "Serialization/MemoryBitStream.h"

#include "Engine/EngineUtility.h"

namespace NetworkState {

RawClientStateInfo::RawClientStateInfo(TCPSocketPtr socket, SocketAddressPtr socketAddress, ERawClientState state)
{
	clientSocket = socket;
	clientAddress = socketAddress;
	clientState = state;
}

RawClientPackageStateInfo::~RawClientPackageStateInfo()
{
	if (inputBitStream) delete inputBitStream;
}

void RawClientPackageStateInfo::SetData(const char* inBuffer, uint32_t inBitCount)
{
	char* buffer = (char*)std::malloc(inBitCount);
	if (!buffer) { DebugEngineTrap(); return; }

	std::memcpy(buffer, inBuffer, inBitCount);

	if (inputBitStream) delete inputBitStream;
	inputBitStream = new InputMemoryBitStream(buffer, inBitCount);
}

} // namespace NetworkState
