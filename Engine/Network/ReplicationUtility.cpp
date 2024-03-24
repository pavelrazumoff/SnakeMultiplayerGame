#include "ReplicationUtility.h"

void DoReplicateDestroyObject(IReplicationObject* obj)
{
	if (!NetworkUtility::IsServer()) return;
	if (!obj->DoesSupportDestroyReplication() ||
		!ReplicationManager::GetInstance().DoesObjectSupportReplication(obj)) return;
	
	OutputMemoryBitStream outStream;

	PacketType type = PacketType::PT_ReplicationData;
	outStream.Serialize(type, NetworkUtilityLibrary::GetRequiredBits<PacketType::PT_MAX>());

	ReplicationManager::GetInstance().ReplicateDestroy(outStream, obj);
	ReplicationManager::GetInstance().CloseReplicationPackage(outStream);

	const uint32_t objNetworkId = ReplicationManager::GetInstance().GetNetworkIdForObject(obj);
	NetworkManager::GetInstance().SendPackageToObjOwnerClient(objNetworkId, outStream);
}
