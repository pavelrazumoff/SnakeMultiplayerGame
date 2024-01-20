#pragma once

#include "stdint.h"
#include <deque>

#include "Engine/Events/EventDelegate.h"

enum class ProfilerCategory
{
	FPS,

	Other,
};

enum class ProfilerEngineFeature
{
	CollisionDetection,

	RenderScene,
	RenderWidgets,

	UpdateScene,
};

DECLARE_EVENT_DELEGATE(ProfilerEngineFeatureDelegate, ProfilerEngineFeature, bool);
DECLARE_EVENT_DELEGATE(ProfilerCategoryDelegate, ProfilerCategory, bool);

class ProfilerManager
{
public:
	ProfilerManager();
	~ProfilerManager();

	static ProfilerManager& GetInstance();

	void Initialize();
	void Update();

	/** Force disable Engine features. */

	void EnableEngineFeature(ProfilerEngineFeature feature, bool bEnable);

	bool IsEngineFeatureEnabled(ProfilerEngineFeature feature) const;
	uint64_t GetEngineDisabledFeatureMask() const { return engineDisabledFeatureMask; }

	/** Track Categories. */

	void EnableTrackCategory(ProfilerCategory category, bool bEnable);

	/** Request Categories results. */

	float GetAverageFPS() const { return FPS; }

	/** Events. */

	ProfilerEngineFeatureDelegate& OnEngineFeatureChangedEvent() { return EngineFeatureChangedEvent; }
	ProfilerCategoryDelegate& OnCategoryChangedEvent() { return CategoryChangedEvent; }

protected:
	/** Track Categories. */

	void DoTrackFPS();

protected:

	ProfilerEngineFeatureDelegate EngineFeatureChangedEvent;
	ProfilerCategoryDelegate CategoryChangedEvent;

protected:
	uint64_t categoryMask = 0;
	uint64_t engineDisabledFeatureMask = 0;

	/** Categories profile results. */

	float FPS = 0.0f;

private:
	/** Categories interim data. */

	// FPS.
	std::deque<float> frameTimes;
	float frameTimeSum = 0.0f;
};
