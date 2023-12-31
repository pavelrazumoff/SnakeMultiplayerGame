#pragma once

#include <functional>
#include <map>
#include <vector>
#include <string>

/*
	DEPRECATED. USE EventDelegate.h INSTEAD.
*/

#define USE_EVENT_MANAGER() 0

#if USE_EVENT_MANAGER()
class EventManager 
{
public:
	typedef std::function<void(void*)> Listener;

	template <typename T>
	void Subscribe(const std::string& event, T* instance, void (T::*method)(void*)) {
		Listeners[event].push_back([=](void* package) {
			(instance->*method)(package);
			});
	}

	void Trigger(const std::string& event, void* package) {
		for (const auto& listener : Listeners[event]) {
			listener(package);
		}
	}

private:
	std::map<std::string, std::vector<Listener>> Listeners;
};
#endif // USE_EVENT_MANAGER()
