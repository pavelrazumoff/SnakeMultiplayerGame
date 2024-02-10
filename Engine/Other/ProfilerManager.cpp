#include "ProfilerManager.h"

#include "TimeManager.h"
#include "Engine/EngineUtility.h"

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

/*
	Force disable Engine features.
*/

void ProfilerManager::EnableEngineFeature(ProfilerEngineFeature feature, bool bEnable)
{
	if (bEnable)
		engineDisabledFeatureMask &= ~ENUM2BIT(feature);
	else
		engineDisabledFeatureMask |= ENUM2BIT(feature);

	EngineFeatureChangedEvent.Trigger(feature, bEnable);
}

bool ProfilerManager::IsEngineFeatureEnabled(ProfilerEngineFeature feature) const
{
	return !(engineDisabledFeatureMask & ENUM2BIT(feature));
}

/*
	Track Categories.
*/

void ProfilerManager::EnableTrackCategory(ProfilerCategory category, bool bEnable)
{
	if (bEnable)
		categoryMask |= ENUM2BIT(category);
	else
		categoryMask &= ~ENUM2BIT(category);

	CategoryChangedEvent.Trigger(category, bEnable);
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
