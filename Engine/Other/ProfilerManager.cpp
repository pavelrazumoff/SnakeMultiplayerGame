#include "ProfilerManager.h"

#include "TimeManager.h"
#include "Engine/EngineUtility.h"

ProfilerManager::ProfilerManager()
{
}

ProfilerManager::~ProfilerManager()
{
}

ProfilerManager& ProfilerManager::GetInstance()
{
	static ProfilerManager instance;
	return instance;
}

void ProfilerManager::Initialize()
{
}

void ProfilerManager::Update()
{
	if (categoryMask & ENUM2BIT(ProfilerCategory::FPS))
		DoTrackFPS();
}

void ProfilerManager::EnableCategory(ProfilerCategory category, bool bEnable)
{
	if (bEnable)
		categoryMask |= ENUM2BIT(category);
	else
		categoryMask &= ~ENUM2BIT(category);
}

void ProfilerManager::DoTrackFPS()
{
	float deltaTime = TimeManager::GetInstance().GetFrameDeltaSeconds();

	frameTimes.push_back(deltaTime);
	frameTimeSum += deltaTime;

	while (frameTimeSum > 1.0f)
	{
		frameTimeSum -= frameTimes.front();
		frameTimes.pop_front();
	}

	FPS = frameTimes.size() / frameTimeSum;
}
