#pragma once

#include "stdint.h"
#include <deque>

enum class ProfilerCategory
{
	FPS,

	Other,
};

class ProfilerManager
{
public:
	ProfilerManager();
	~ProfilerManager();

	static ProfilerManager& GetInstance();

	void Initialize();
	void Update();

	void EnableCategory(ProfilerCategory category, bool bEnable);

	/** Request Categories results. */

	float GetAverageFPS() const { return FPS; }

protected:
	/** Track Categories. */

	void DoTrackFPS();

protected:
	uint64_t categoryMask = 0;

	/** Categories profile results. */

	float FPS = 0.0f;

private:
	/** Categories interim data. */

	// FPS.
	std::deque<float> frameTimes;
	float frameTimeSum = 0.0f;
};
