#include "MemoryReflectionSystem.h"

MemoryReflectionSystem::MemoryReflectionSystem()
{
}

MemoryReflectionSystem::~MemoryReflectionSystem()
{
}

MemoryReflectionSystem& MemoryReflectionSystem::GetInstance()
{
	static MemoryReflectionSystem Instance;
	return Instance;
}

void MemoryReflectionSystem::AssignNewReference(GameObject** ObjectObserver, GameObject* ObjectPtr)
{
	for (auto it = AssignmentPairs.begin(); it != AssignmentPairs.end();)
	{
		if (it->second == ObjectObserver)
		{
			it = AssignmentPairs.erase(it);
			// TODO: Should be here only once, so no need to continue with the loop.
		}
		else it++;
	}

	if (ObjectPtr != nullptr)
		AssignmentPairs.insert(std::pair<uint32_t, GameObject**>(ObjectPtr->GetID(), ObjectObserver));
}

uint32_t MemoryReflectionSystem::GetReferenceCount(GameObject* ObjectPtr) const
{
	uint32_t Count = 0;

	auto range = AssignmentPairs.equal_range(ObjectPtr->GetID());
	for (auto it = range.first; it != range.second; ++it)
	{
		if (*it->second != nullptr)
			Count++;
	}

	return Count;
}

void MemoryReflectionSystem::ClearDanglingPointers(GameObject* InstigatorObject)
{
	auto range = AssignmentPairs.equal_range(InstigatorObject->GetID());
	for (auto it = range.first; it != range.second; ++it)
	{
		*it->second = nullptr;
	}

	AssignmentPairs.erase(InstigatorObject->GetID());
}
