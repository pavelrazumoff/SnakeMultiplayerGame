#include "GameObject.h"

#include "Engine/MemoryReflectionSystem.h"
#include "Engine/Network/NetworkEngineUtility.h"
#include "Engine/Network/ReplicationUtility.h"
#include "Engine/Network/SerializationUtility.h"

static GameObject* g_RootObject = nullptr;

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	while (!ChildObjects.empty())
	{
		if (ChildObjects[0]) ChildObjects[0]->_SetAsOwner(nullptr);
		else ChildObjects.erase(ChildObjects.begin());
	}

	if (!bWaitForDestroy)
	{
		PostDestroy();
	}
}

void GameObject::ChangeOwner(GameObject* _owner)
{
	if (_owner)
		_owner->AddAsChild(this);
}

void GameObject::_SetAsOwner(GameObject* _owner)
{
	if (Owner) Owner->RemoveChild(this);
	Owner = _owner;
}

void GameObject::AddAsChild(GameObject* Child)
{
	Child->_SetAsOwner(this);
	ChildObjects.push_back(Child);
}

void GameObject::RemoveChild(GameObject* Child)
{
	auto it = std::find_if(ChildObjects.begin(), ChildObjects.end(), [Child](GameObject* Other) -> bool {
		return Child == Other;
		});

	if (it != ChildObjects.end())
		ChildObjects.erase(it);
}

void GameObject::PostDestroy()
{
	if (Owner) Owner->NotifyChildDestroy(this);

	MemoryReflectionSystem::GetInstance().ClearDanglingPointers(this);
}

void GameObject::NotifyChildDestroy(GameObject* Child)
{
	auto it = std::find_if(ChildObjects.begin(), ChildObjects.end(), [Child](GameObject* Other) -> bool {
		return Child == Other;
		});

	if (it != ChildObjects.end())
		ChildObjects.erase(it);
}

void GameObject::Destroy()
{
	if (!bWaitForDestroy)
	{
		DoReplicateDestroyObject(this);

		PostDestroy();
		bWaitForDestroy = true;
	}
}

/*
	EngineGenericType implementation.
*/

EngineGenericType* GameObject::CloneGeneric() const
{
	DebugEngineTrap();
	return nullptr;
	//return CreateNewObject<GameObject>();
}

std::string GameObject::GetGenericTypeName() const
{
	return "GameObject";
}

void GameObject::ReplDestroy()
{
	Destroy();
}

/*
	IReplicationObject implementation.
*/

void GameObject::WriteCreate(OutputMemoryBitStream& outStream, ReplicationType clientReplType)
{
	if (NetworkUtility::IsServer())
		SerializeCreate(outStream, clientReplType);
}

bool GameObject::WriteUpdate(OutputMemoryBitStream& outStream, ReplicationType clientReplType)
{
	if (NetworkUtility::IsServer())
		return SerializeUpdate(outStream, clientReplType);

	return false;
}

void GameObject::Read(InputMemoryBitStream& inStream)
{
	if (NetworkUtility::IsClient())
		SerializeUpdate(inStream, ReplicationType::All);
}

/*

*/

GameObject* InitializeRootObject()
{
	if (g_RootObject) return g_RootObject;

	g_RootObject = new GameObject();
	return g_RootObject;
}

GameObject* GetRootObject()
{
	return g_RootObject;
}

void DestroyRootObject()
{
	delete g_RootObject;
	g_RootObject = nullptr;
}

bool SerializeData(MemoryBitStream& stream, const DataType* inDataType,
	uint8_t* inData, uint64_t inProperties, ReplicationType clientReplType)
{
	// TODO: Pass in the ReplicationType value to determine to whom we allowed to serialize the data.
	// If the ReplicationType is Owner, then we should check the mv for the ReplicationType::Owner or All flag.

	// TODO: If there won't be any properties to serialize, return false and not create the empty package.

	stream.Serialize(inProperties);

	bool bSerializedAny = false;

	const auto& mvs = inDataType->GetMemberVariables();
	for (size_t mvIndex = 0, c = mvs.size(); mvIndex < c; ++mvIndex)
	{
		if ((((uint64_t)1 << mvIndex) & inProperties) != 0)
		{
			const auto& mv = mvs[mvIndex];

			if (clientReplType != ReplicationType::All)
			{
				if (mv.GetReplicationType() != ReplicationType::All &&
					mv.GetReplicationType() != clientReplType)
				{
					continue;
				}
			}

			void* mvData = inData + mv.GetOffset();

			bSerializedAny = SerializePrimitiveType(stream, mvData, mv.GetPrimitiveType(), mv.GetPrimitiveSubType()) ||
				bSerializedAny;

			if (NetworkUtility::IsClient())
				mv.DoReplicatedCallback(reinterpret_cast<IReplicationObject*>(inData));
		}
	}

	return bSerializedAny;
}