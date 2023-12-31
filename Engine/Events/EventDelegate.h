#pragma once

#include <functional>

template <typename... Args>
class EventDelegate
{
public:
	using Listener = std::function<void(Args...)>;
	
	template <typename T, typename... Args>
	void Subscribe(T* instance, void (T::* method)(Args...)) {
		Listeners.push_back([=](Args... args) {
			(instance->*method)(std::forward<Args>(args)...);
			});
	}

	template <typename... Args>
	void Trigger(Args... args) {
		for (const auto& listener : Listeners) {
			listener(std::forward<Args>(args)...);
		}
	}

private:
	std::vector<Listener> Listeners;
};
