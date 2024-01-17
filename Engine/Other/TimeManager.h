#pragma once

#include "Engine/GameObject/GameObjectPtr.h"
#include "Engine/Events/EventDelegate.h"

#include <chrono>
#include <vector>

class TimeManager;

class TimerHandler : public GameObject
{
public:
	TimerHandler() {}
	~TimerHandler() {}

	void SetTimeLimit(float timeLimit) { TimeLimit = timeLimit; }
	void SetSingleFire(bool singleFire) { bSingleFire = singleFire; }

	float GetTimeLeft() const { return TimeLimit - TimeElapsed; }
	bool IsSingleFire() const { return bSingleFire; }

protected:
	void IncreaseTimeElapsed(float deltaSeconds) { TimeElapsed += deltaSeconds; }
	void ResetTimer() { TimeElapsed = 0.0f; }

protected:
	friend class TimeManager;

	float TimeLimit = 1.0f;
	float TimeElapsed = 0.0f;

	bool bSingleFire = false;
};

typedef TObjectPtr<TimerHandler> TimerHandlerPtr;

DECLARE_EVENT_DELEGATE(TimerDelegate);

class TimeManager
{
public:
	TimeManager();
	~TimeManager();

	static TimeManager& GetInstance();

	void Initialize();

	void StartClock();
	void Update();

	void SetFrameDeltaSeconds(float deltaSeconds);
	float GetFrameDeltaSeconds() const;

	//void AddTimerHandler(TimerHandlerPtr timerHandler);

	template<class T>
	void BindTimerHandler(TimerHandlerPtr timerHandler, T* instance, void (T::* method)())
	{
		auto it = std::find_if(TimerHandlers.begin(), TimerHandlers.end(), [timerHandler](const auto& handler)
		{
			return handler.Get() == timerHandler.Get();
		});

		if (it == TimerHandlers.end())
		{
			TimerHandlers.push_back(timerHandler);
			TimerDelegates.push_back(TimerDelegate());
			TimerDelegates.back().Subscribe(instance, method);
		}
	}

protected:
	TimerDelegate* GetTimerDelegate(const TimerHandlerPtr& timerHandler);
	TimerDelegate* GetTimerDelegate(size_t index);

protected:
	std::vector<TimerHandlerPtr> TimerHandlers;
	std::vector<TimerDelegate> TimerDelegates;

private:
	std::chrono::steady_clock::time_point StartTime;

	float FrameDeltaSeconds = 0.0f;
};
