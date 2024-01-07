#pragma once

#include "GameObject.h"
#include "Engine/MemoryReflectionSystem.h"

template<typename T>
class TObjectPtr
{
public:
	TObjectPtr(T* Other) : ObjectPtr(Other) {
		static_assert(std::is_base_of<GameObject, T>::value, "T must be derived from GameObject.");

		MemoryReflectionSystem::GetInstance().AssignNewReference((GameObject**)&ObjectPtr, Other);
	}
	TObjectPtr() : ObjectPtr(nullptr) {}

	TObjectPtr(const TObjectPtr& Other) : ObjectPtr(Other.ObjectPtr) {
		MemoryReflectionSystem::GetInstance().AssignNewReference((GameObject**)&ObjectPtr, Other.ObjectPtr);
	}

	TObjectPtr(TObjectPtr&& Other) noexcept : ObjectPtr(Other.ObjectPtr) {
		Other.ObjectPtr = nullptr;
		MemoryReflectionSystem::GetInstance().AssignNewReference((GameObject**)&Other.ObjectPtr, nullptr);
		MemoryReflectionSystem::GetInstance().AssignNewReference((GameObject**)&ObjectPtr, ObjectPtr);
	}

	~TObjectPtr() {

		if (!ObjectPtr) return; // Already nulled.
		MemoryReflectionSystem::GetInstance().AssignNewReference((GameObject**)&ObjectPtr, nullptr);
	}

	void operator=(T* Other) {
		static_assert(std::is_base_of<GameObject, T>::value, "T must be derived from GameObject.");

		if (!ObjectPtr && !Other) return; // Already nulled.

		ObjectPtr = Other;
		MemoryReflectionSystem::GetInstance().AssignNewReference((GameObject**)&ObjectPtr, Other);
	}

	void operator=(const TObjectPtr& Other) {
		ObjectPtr = Other.ObjectPtr;
		MemoryReflectionSystem::GetInstance().AssignNewReference((GameObject**)&ObjectPtr, Other.ObjectPtr);
	}

	T* operator->() { return ObjectPtr; }
	const T* operator->() const { return ObjectPtr; }

	T* Get() { return ObjectPtr; }
	const T* Get() const { return ObjectPtr; }

	bool IsValid() const { return ObjectPtr != nullptr; }

private:
	T* ObjectPtr = nullptr;
};
