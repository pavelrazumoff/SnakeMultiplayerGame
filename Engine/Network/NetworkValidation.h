#pragma once

#include "Replication/ReplicationValidation.h"
#include "Socket/TCPSocket.h"

#include <map>

class ServerReplicationValidation : public IReplicationValidation
{
public:
	virtual bool ValidateObjectOwnership(uint32_t networkId) const override;

	void RegisterObjectOwnershipForClient(uint32_t networkId, TCPSocketPtr clientSocket);

	void StartValidationForClient(TCPSocketPtr clientSocket);
	void FinishValidation();

protected:
	bool FindNetworkIdForClient(uint32_t networkId, TCPSocketPtr clientSocket) const;

private:
	std::multimap<TCPSocketPtr, uint32_t> clientOwnedObjectsMap;

	TCPSocketPtr _currentCheckingClient;
};
