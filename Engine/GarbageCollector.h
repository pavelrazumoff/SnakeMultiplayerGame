#pragma once

#include <vector>
#include "GameObject/GameObject.h"

class GarbageCollector
{
private:
	GarbageCollector();
	
public:
	~GarbageCollector();

	static GarbageCollector& GetInstance();

	void Update();
	void Free();

	void StartTrackObject(GameObject* Object);

private:
	std::vector<GameObject*> TrackGameObjects;

	static uint32_t GameObjectsCounter;
};