#include "GarbageCollector.h"
#include "MemoryReflectionSystem.h"

uint32_t GarbageCollector::GameObjectsCounter = 0;

GarbageCollector::GarbageCollector()
{
}

GarbageCollector::~GarbageCollector()
{
}

GarbageCollector& GarbageCollector::GetInstance()
{
	static GarbageCollector Instance;
	return Instance;
}

void GarbageCollector::Update()
{
	for (auto it = TrackGameObjects.begin(); it != TrackGameObjects.end();)
	{
		// TODO: Somehow detect circular links.
		if ((*it)->IsWaitingForDestroy() ||
			(!MemoryReflectionSystem::GetInstance().GetReferenceCount(*it) ||
			!(*it)->GetOwner()))
		{
			delete *it;
			it = TrackGameObjects.erase(it);
		}
		else ++it;
	}
}

void GarbageCollector::Free()
{
	while (!TrackGameObjects.empty())
	{
		delete TrackGameObjects[0];
		TrackGameObjects.erase(TrackGameObjects.begin());
	}
}

void GarbageCollector::StartTrackObject(GameObject* Object)
{
	Object->AssignID(++GameObjectsCounter);
	TrackGameObjects.push_back(Object);
}
