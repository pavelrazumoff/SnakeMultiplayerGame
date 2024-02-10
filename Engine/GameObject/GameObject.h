#pragma once

#include "Engine/EngineUtility.h"
#include "Replication/ReplicationObject.h"

#include <vector>

class GameObject;
class OutputMemoryBitStream;
class InputMemoryBitStream;

GameObject* InitializeRootObject();
GameObject* GetRootObject();
void DestroyRootObject();

template<typename T>
T* CreateNewObject(GameObject* Owner = GetRootObject());

#define GAMEOBJECT_BODY(inClassName) \
public: \
	enum { kClassId = EngineUtilityLibrary::StringToUint32(#inClassName) }; \
	virtual uint32_t GetClassId() const override { return kClassId; } \
	static IReplicationObject* CreateReplicationInstance() { return CreateNewObject<inClassName>(); }

class GameObject : public IReplicationObject
{
	GAMEOBJECT_BODY(GameObject)

public:
	GameObject();
	GameObject(const GameObject& _other) = delete;

	virtual ~GameObject();

	virtual void ChangeOwner(GameObject* _owner);
	virtual void AddAsChild(GameObject* Child);
	virtual void RemoveChild(GameObject* Child);

	virtual void NotifyChildDestroy(GameObject* Child);
	virtual void Destroy();
	bool IsWaitingForDestroy() const { return bWaitForDestroy; }

	uint32_t GetID() const { return ID; }
	GameObject* GetOwner() const { return Owner; }

	/** IReplicationObject implementation. */

	virtual void Write(OutputMemoryBitStream& outStream) override;
	virtual void Read(InputMemoryBitStream& inStream) override;

	virtual void SafeDestroy() override;

public:
	template<typename T>
	static T* CloneObject(const T* object, GameObject* Owner)
	{
		static_assert(std::is_base_of<GameObject, T>::value, "T must be derived from GameObject.");

		GameObject* _castedObj = (GameObject*)object;

		T* clonedObject = _castedObj ? dynamic_cast<T*>(_castedObj->Clone(Owner)) : nullptr;
		engine_assert(clonedObject != nullptr);

		return clonedObject;
	}

protected:
	void _SetAsOwner(GameObject* _owner);

	virtual GameObject* Clone(GameObject* _owner) const { return nullptr; }

private:
	void PostDestroy();

	void AssignID(uint32_t _id) { ID = _id; }

private:
	friend class GarbageCollector;

	GameObject* Owner = nullptr;
	std::vector<GameObject*> ChildObjects;

	bool bWaitForDestroy = false;

	uint32_t ID = 0;
};
