#pragma once

#include "Engine/EngineTypes.h"
#include "Engine/EngineUtility.h"

#include "Replication/ReplicationObject.h"
#include "Replication/ObjectCreationRegistry.h"
#include "ReflectionSystem/DataType.h"

#include <vector>

class GameObject;

class MemoryBitStream;
class OutputMemoryBitStream;
class InputMemoryBitStream;

GameObject* InitializeRootObject();
GameObject* GetRootObject();
void DestroyRootObject();

template<typename T>
T* CreateNewObject(GameObject* Owner = GetRootObject());

void SerializeData(MemoryBitStream& stream, const DataType* inDataType, uint8_t* inData, uint64_t inProperties);

/*
	Class Registration.
*/

#define REGISTER_CLASS(inClassName) \
class inClassName; \
class inClassName##_register \
{ \
public: \
	inClassName##_register() \
	{ \
		ObjectCreationRegistry::GetInstance().RegisterCreationFunction<inClassName>(); \
		RegisterReplication<inClassName>(); \
	} \
private:\
	template<class T> \
	void RegisterReplication() \
	{ \
		T::RegisterReplicationMembers(); \
	} \
}; \
inline inClassName##_register inClassName##_instance;

/*
	Define the class body.
*/

#define GAMEOBJECT_BODY_BASE(inClassName) \
public: \
	enum { kClassId = EngineUtilityLibrary::StringToUint32(#inClassName) }; \
	virtual uint32_t GetClassId() const override { return kClassId; } \
	static IReplicationObject* CreateReplicationInstance() { return CreateNewObject<inClassName>(); } \
private:\
	friend class inClassName##_register;

#define GAMEOBJECT_BODY(inClassName, inParentClassName) \
	typedef inParentClassName Inherited; \
	GAMEOBJECT_BODY_BASE(inClassName)

/*
	Replication.
*/

#define ENABLE_REPLICATION(inClassName) \
public: \
	static DataType& GetDataType() {\
		static DataType sDataType; \
		return sDataType; \
	} \
protected: \
	static void RegisterReplicationMembers(); \
private: \
	uint64_t inClassName##_replicationPropertyMask = 0; \
public: \
	virtual void Serialize(MemoryBitStream& stream) override { \
		Inherited::Serialize(stream);\
		SerializeData(stream, &GetDataType(), reinterpret_cast<uint8_t*>(this), inClassName##_replicationPropertyMask);\
		inClassName##_replicationPropertyMask = 0; \
	}

#define MAKE_REPLICATED(inClassName, inMemberVariable, inPrimitiveType, inCallback) \
	{ \
		auto mv = GetDataType().AddMemberVariable(MemberVariable(#inMemberVariable, inPrimitiveType, mvOffsetOf(inClassName, inMemberVariable))); \
		if (inCallback) mv.SetReplicationCallback<inClassName>(inCallback); \
	}

//#define ASSIGN_REPLICATION_CALLBACK(inClassName, inMemberVariable, inCallback) \
//	if (auto mv = GetDataType().GetMemberByOffset(mvOffsetOf(inClassName, inMemberVariable))) \
//		mv->SetReplicationCallback(inCallback);

// TODO: Add some kind of conditional replication support.
#define MARK_FOR_REPLICATION(inClassName, inMemberVariable) \
	if (NetworkUtility::IsServer()) { \
		int inClassName##index = GetDataType().GetMemberIndexByOffset(mvOffsetOf(inClassName, inMemberVariable));\
		if (inClassName##index >= 0) \
			inClassName##_replicationPropertyMask |= ((uint64_t)1 << (size_t)inClassName##index); \
	}

/*
	GameObject class.
*/

REGISTER_CLASS(GameObject)
class GameObject : public EngineGenericType, public IReplicationObject
{
	GAMEOBJECT_BODY_BASE(GameObject)

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

	/** EngineGenericType implementation. */

	virtual [[nodiscard]] EngineGenericType* CloneGeneric() const override;
	virtual std::string GetGenericTypeName() const override;

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

	/** Replication. */

	static void RegisterReplicationMembers() {}

	virtual void Serialize(MemoryBitStream& stream) {}

	/** IReplicationObject implementation. */

	virtual void Write(OutputMemoryBitStream& outStream) override;
	virtual void Read(InputMemoryBitStream& inStream) override;

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
