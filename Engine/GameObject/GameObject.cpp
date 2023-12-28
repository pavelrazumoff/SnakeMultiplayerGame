#include "GameObject.h"
#include "Engine/MemoryReflectionSystem.h"

static GameObject* g_RootObject = nullptr;

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	for (auto child : ChildObjects)
		if (child) child->SetAsOwner(nullptr);

	if (!bWaitForDestroy)
	{
		PostDestroy();
	}
}

void GameObject::AddAsChild(GameObject* Child)
{
	Child->SetAsOwner(this);
	ChildObjects.push_back(Child);
}

void GameObject::PostDestroy()
{
	MemoryReflectionSystem::GetInstance().ClearDanglingPointers(this);
	if (Owner) Owner->NotifyChildDestroy(this);
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
	PostDestroy();
	bWaitForDestroy = true;
}

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
