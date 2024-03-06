#include "NetworkEngineUtility.h"

#include "Serialization/MemoryBitStream.h"

#include "Engine/EngineUtility.h"
#include "Engine/Network/NetworkManager.h"

namespace NetworkUtility {

bool IsServer()
{
	return NetworkManager::GetInstance().IsServer();
}

bool IsClient()
{
	return NetworkManager::GetInstance().IsClient();
}

} // namespace NetworkUtility

namespace NetworkState {

/*
	Receive data from client.
*/

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

void RawClientPackageStateInfo::SetData(const char* inBuffer, uint32_t inByteCount)
{
	char* buffer = (char*)std::malloc(inByteCount);
	if (!buffer) { DebugEngineTrap(); return; }

	std::memcpy(buffer, inBuffer, inByteCount);

	if (inputBitStream) delete inputBitStream;
	inputBitStream = new InputMemoryBitStream(buffer, inByteCount << 3);
}

InputMemoryBitStream* RawClientPackageStateInfo::GetStream()
{
	return inputBitStream;
}

/*
	Send data to client.
*/

Server2ClientPackage::Server2ClientPackage(TCPSocketPtr _clientSocket, const char* _packageData, uint32_t _packageByteCount)
{
	clientSocket = _clientSocket;
	
	packageData = (char*)std::malloc(_packageByteCount);
	if (!packageData) { DebugEngineTrap(); return; }

	std::memcpy(packageData, _packageData, _packageByteCount);
	packageByteCount = _packageByteCount;
}

Server2ClientPackage::~Server2ClientPackage()
{
	if (packageData) std::free(packageData);
}

/*
	Receive data from server.
*/

RawServerPackageStateInfo::RawServerPackageStateInfo(const char* inBuffer, uint32_t inByteCount)
{
	char* buffer = (char*)std::malloc(inByteCount);
	if (!buffer) { DebugEngineTrap(); return; }

	std::memcpy(buffer, inBuffer, inByteCount);

	inputBitStream = new InputMemoryBitStream(buffer, inByteCount << 3);
}

RawServerPackageStateInfo::~RawServerPackageStateInfo()
{
	if (inputBitStream) delete inputBitStream;
}

InputMemoryBitStream* RawServerPackageStateInfo::GetStream()
{
	return inputBitStream;
}

} // namespace NetworkState
