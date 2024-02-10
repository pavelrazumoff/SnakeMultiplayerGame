#pragma once

#include "GameObject.h"
#include <type_traits>

#include "Engine/GarbageCollector.h"

template<typename T>
T* CreateNewObject(GameObject* Owner)
{
	static_assert(std::is_base_of<GameObject, T>::value, "T must be derived from GameObject.");

	T* NewObject = new T();
	GarbageCollector::GetInstance().StartTrackObject(NewObject);
	
	if (Owner)
	{
		Owner->AddAsChild(NewObject);
	}

	return NewObject;
}
