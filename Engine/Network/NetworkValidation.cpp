#include "NetworkValidation.h"

#include "Engine/EngineUtility.h"

bool ServerReplicationValidation::ValidateObjectOwnership(uint32_t networkId) const
{
	if (!_currentCheckingClient) { DebugEngineTrap(); return false; }

	return FindNetworkIdForClient(networkId, _currentCheckingClient);
}

void ServerReplicationValidation::RegisterObjectOwnershipForClient(uint32_t networkId, TCPSocketPtr clientSocket)
{
	if (FindNetworkIdForClient(networkId, clientSocket)) return; // Already registered.

	clientOwnedObjectsMap.insert(std::make_pair(clientSocket, networkId));
}

void ServerReplicationValidation::StartValidationForClient(TCPSocketPtr clientSocket)
{
	_currentCheckingClient = clientSocket;
}

void ServerReplicationValidation::FinishValidation()
{
	_currentCheckingClient = nullptr;
}

bool ServerReplicationValidation::FindNetworkIdForClient(uint32_t networkId, TCPSocketPtr clientSocket) const
{
	auto range = clientOwnedObjectsMap.equal_range(clientSocket);
	auto it = std::find_if(range.first, range.second, [networkId](const auto& pair) -> bool {
		return pair.second == networkId;
	});

	return it != range.second;
}
