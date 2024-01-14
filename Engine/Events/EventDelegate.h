#pragma once

#include <functional>
#include <map>

template <typename... Args>
class EventDelegate
{
public:
	using Listener = std::function<void(Args...)>;
	
	template <typename T, typename... Args>
	void Subscribe(T* instance, void (T::* method)(Args...)) 
	{
		Listeners[reinterpret_cast<void*>(instance)] = ([=](Args... args) {
			(instance->*method)(std::forward<Args>(args)...);
		});
	}

	template <typename T>
	void Unsubscribe(T* instance) 
	{
		Listeners.erase(reinterpret_cast<void*>(instance));
	}

	void UnsubscribeAll() 
	{
		Listeners.clear();
	}

	template <typename... Args>
	void Trigger(Args... args) 
	{
		for (const auto& listener : Listeners) {
			listener.second(std::forward<Args>(args)...);
		}
	}

private:
	std::map<void*, Listener> Listeners;
};

#define DECLARE_EVENT_DELEGATE(EventType, ...) \
	using EventType = EventDelegate<__VA_ARGS__> 
