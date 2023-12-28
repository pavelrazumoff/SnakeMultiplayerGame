#pragma once

#include <map>
#include "GameObject/GameObject.h"

class MemoryReflectionSystem
{
private:
	MemoryReflectionSystem();

public:
	~MemoryReflectionSystem();

	static MemoryReflectionSystem& GetInstance();

	void AssignNewReference(GameObject** ObjectObserver, GameObject* ObjectPtr);
	uint32_t GetReferenceCount(GameObject* ObjectPtr) const;

	void ClearDanglingPointers(GameObject* InstigatorObject);

private:
	std::multimap<uint32_t, GameObject**> AssignmentPairs;
};
