#pragma once

#include <vector>
#include "GameObject/GameObjectPtr.h"

class TimerHandler;

class GarbageCollector
{
private:
	GarbageCollector();
	
public:
	~GarbageCollector();

	static GarbageCollector& GetInstance();

	void Initialize();

	void Update();
	void Free();

	void StartTrackObject(GameObject* Object);

protected:
	void PerformGarbageCollection();

private:
	std::vector<GameObject*> TrackGameObjects;

	static uint32_t GameObjectsCounter;

	TObjectPtr<TimerHandler> CollectTimer;
};