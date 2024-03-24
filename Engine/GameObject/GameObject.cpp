#include "GameObject.h"

#include "Engine/MemoryReflectionSystem.h"
#include "Engine/Network/NetworkEngineUtility.h"
#include "Engine/Network/ReplicationUtility.h"

#include "Serialization/MemoryBitStream.h"

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
	DoReplicateDestroyObject(this);

	if (!bWaitForDestroy)
	{
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

void GameObject::Write(OutputMemoryBitStream& outStream)
{
	if (NetworkUtility::IsServer())
		Serialize(outStream);
}

void GameObject::WriteCreate(OutputMemoryBitStream& outStream)
{
	if (NetworkUtility::IsServer())
		SerializeCreate(outStream);
}

void GameObject::Read(InputMemoryBitStream& inStream)
{
	if (NetworkUtility::IsClient())
		Serialize(inStream);
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

void SerializeData(MemoryBitStream& stream, const DataType* inDataType,
	uint8_t* inData, uint64_t inProperties)
{
	stream.Serialize(inProperties);

	const auto& mvs = inDataType->GetMemberVariables();
	for (size_t mvIndex = 0, c = mvs.size(); mvIndex < c; ++mvIndex)
	{
		if ((((uint64_t)1 << mvIndex) & inProperties) != 0)
		{
			const auto& mv = mvs[mvIndex];
			void* mvData = inData + mv.GetOffset();

			switch (mv.GetPrimitiveType())
			{
				case EPrimitiveType::EPT_Int:
					stream.Serialize(*reinterpret_cast<int*>(mvData));
					break;
				case EPrimitiveType::EPT_Float:
					stream.Serialize(*reinterpret_cast<float*>(mvData));
					break;
				case EPrimitiveType::EPT_Bool:
					stream.Serialize(*reinterpret_cast<bool*>(mvData));
					break;
				case EPrimitiveType::EPT_String:
					stream.Serialize(*reinterpret_cast<std::string*>(mvData));
					break;
				default:
					break;
			}

			if (NetworkUtility::IsClient())
				mv.DoReplicatedCallback(reinterpret_cast<IReplicationObject*>(inData));
		}
	}
}