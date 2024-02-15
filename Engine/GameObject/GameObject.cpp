#include "GameObject.h"
#include "Engine/MemoryReflectionSystem.h"

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

/*
	IReplicationObject implementation.
*/

void GameObject::Write(OutputMemoryBitStream& outStream)
{
}

void GameObject::Read(InputMemoryBitStream& inStream)
{
}

void GameObject::SafeDestroy()
{
	Destroy();
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
