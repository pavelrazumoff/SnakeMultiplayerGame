#include "TimeManager.h"

TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
}

TimeManager& TimeManager::GetInstance()
{
	static TimeManager instance;
	return instance;
}

void TimeManager::Initialize()
{
}

void TimeManager::StartClock()
{
	StartTime = std::chrono::high_resolution_clock::now();
}

void TimeManager::Update()
{
	using namespace std::chrono;

	auto End = high_resolution_clock::now();

	SetFrameDeltaSeconds(duration_cast<duration<float>>(End - StartTime).count());
	StartTime = End;

	// TODO: Replace std::vector and std::map with our templated containers that can handle the TObjectPtr.
	for (size_t i = 0; i < TimerHandlers.size(); ++i)
	{
		if (TimerHandlers[i].IsValid())
		{
			TimerHandlers[i]->IncreaseTimeElapsed(GetFrameDeltaSeconds());
			if (TimerHandlers[i]->GetTimeLeft() <= 0.0f)
			{
				if (auto timerDelegate = GetTimerDelegate(i))
					timerDelegate->Trigger();

				if (!TimerHandlers[i]->IsSingleFire())
					TimerHandlers[i]->ResetTimer();
			}
		}
		else
		{
			TimerHandlers.erase(TimerHandlers.begin() + i);
			TimerDelegates.erase(TimerDelegates.begin() + i);
			--i;
		}
	}
}

void TimeManager::SetFrameDeltaSeconds(float deltaSeconds)
{
	FrameDeltaSeconds = deltaSeconds;
}

float TimeManager::GetFrameDeltaSeconds() const
{
	return FrameDeltaSeconds;
}

TimerDelegate* TimeManager::GetTimerDelegate(const TimerHandlerPtr& timerHandler)
{
	auto it = std::find_if(TimerHandlers.begin(), TimerHandlers.end(), [timerHandler](const TimerHandlerPtr& handler)
		{ return handler.Get() == timerHandler.Get(); }
	);

	if (it == TimerHandlers.end()) return nullptr;

	return GetTimerDelegate(std::distance(TimerHandlers.begin(), it));
}

TimerDelegate* TimeManager::GetTimerDelegate(size_t index)
{
	if (index < 0 || index >= TimerDelegates.size()) return nullptr;

	return &TimerDelegates[index];
}

//void TimeManager::AddTimerHandler(TimerHandlerPtr timerHandler)
//{
//	TimerHandlers.push_back(timerHandler);
//}
