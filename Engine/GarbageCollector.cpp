#include "GarbageCollector.h"

#include "MemoryReflectionSystem.h"
#include "Engine/Other/TimeManager.h"

#include "Engine/GameObject/GameObjectUtility.h"

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

void GarbageCollector::Initialize()
{
	CollectTimer = CreateNewObject<TimerHandler>();
	if (CollectTimer.IsValid())
	{
		CollectTimer->SetTimeLimit(2.0f);
		CollectTimer->SetSingleFire(false);

		TimeManager::GetInstance().BindTimerHandler(CollectTimer, this, &GarbageCollector::PerformGarbageCollection);
	}
}

void GarbageCollector::Update()
{
	/*
	static float GarbageCollectorWaitTime = 0.0f;
	const float GarbageCollectorWaitTimeLimit = 2.0f;

	GarbageCollectorWaitTime += TimeManager::GetInstance().GetFrameDeltaSeconds();
	if (GarbageCollectorWaitTime >= GarbageCollectorWaitTimeLimit)
	{
		for (auto it = TrackGameObjects.begin(); it != TrackGameObjects.end();)
		{
			// TODO: Somehow detect circular links.
			if ((*it)->IsWaitingForDestroy() ||
				(!MemoryReflectionSystem::GetInstance().GetReferenceCount(*it) ||
					!(*it)->GetOwner()))
			{
				delete* it;
				it = TrackGameObjects.erase(it);
			}
			else ++it;
		}

		GarbageCollectorWaitTime -= GarbageCollectorWaitTimeLimit;
	}*/
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

void GarbageCollector::PerformGarbageCollection()
{
	for (auto it = TrackGameObjects.begin(); it != TrackGameObjects.end();)
	{
		// TODO: Somehow detect circular links.
		if ((*it)->IsWaitingForDestroy() ||
			(!MemoryReflectionSystem::GetInstance().GetReferenceCount(*it) ||
				!(*it)->GetOwner()))
		{
			delete* it;
			it = TrackGameObjects.erase(it);
		}
		else ++it;
	}
}
