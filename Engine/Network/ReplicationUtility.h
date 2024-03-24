#pragma once

#include "Replication/RMIManager.h"
#include "Replication/ReplicationManager.h"
#include "Replication/ReplicationHeader.h"

#include "Engine/Network/NetworkManager.h"

/*
	Remote method invocation.
*/

#define REGISTER_REMOTE_INVOCATION(inUnwrapMethod) \
	REGISTER_RMI_FUNC(inUnwrapMethod);
/*
	if (NetworkUtility::IsServer()) { \
		REGISTER_RMI_FUNC(inUnwrapMethod); \
	}*/

inline void _DoSerializeArgs(MemoryBitStream& /*stream*/)
{
	// Do nothing.
}

template<typename T>
void _DoSerializeArgs(MemoryBitStream & stream, T && arg)
{
	stream.Serialize(std::forward<T>(arg));
}

template<typename T, typename... Args>
void _DoSerializeArgs(MemoryBitStream& stream, T&& arg, Args&&... args)
{
	_DoSerializeArgs(stream, std::forward<T>(arg));
	_DoSerializeArgs(stream, std::forward<Args>(args)...);
}

template<typename... Args>
void _MakeRMIPackage(OutputMemoryBitStream& outStream, IReplicationObject* obj, const char* inUnwrapMethod, Args&&... args)
{
	PacketType type = PacketType::PT_ReplicationData;
	outStream.Serialize(type, NetworkUtilityLibrary::GetRequiredBits<PacketType::PT_MAX>());

	const uint32_t callerNetworkId = ReplicationManager::GetInstance().GetNetworkIdForObject(obj);

	ReplicationHeader rh(ReplicationAction::RA_RMI, callerNetworkId);
	rh.Write(outStream);

	uint32_t rmiId = NetworkUtilityLibrary::StringToUint32(inUnwrapMethod);//GET_RMI_FUNC_ID(inUnwrapMethod);
	outStream.Serialize(rmiId);

	_DoSerializeArgs(outStream, std::forward<Args>(args)...);

	ReplicationManager::GetInstance().CloseReplicationPackage(outStream);
}

#define CALL_REMOTE_INVOCATION_ON_SERVER(inUnwrapMethod, ...) \
	if (NetworkUtility::IsClient()) { \
		OutputMemoryBitStream _outStream; \
		_MakeRMIPackage(_outStream, this, #inUnwrapMethod, __VA_ARGS__); \
		NetworkManager::GetInstance().SendPackageToServer(_outStream); \
	}

#define CALL_REMOTE_INVOCATION_ON_OWNING_CLIENT(inUnwrapMethod, ...) \
	if (NetworkUtility::IsServer()) { \
		OutputMemoryBitStream _outStream; \
		_MakeRMIPackage(_outStream, this, #inUnwrapMethod, __VA_ARGS__); \
		const uint32_t objNetworkId = ReplicationManager::GetInstance().GetNetworkIdForObject(this); \
		NetworkManager::GetInstance().SendPackageToObjOwnerClient(objNetworkId, _outStream); \
	}

void DoReplicateDestroyObject(IReplicationObject* obj);
