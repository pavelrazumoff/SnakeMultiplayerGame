#include "InputComponent.h"

bool InputComponent::PassInput(const InputState& is)
{
	bool bFoundAnyAction = false;
	for (const auto& key : is.KeyPressedQueue)
	{
		auto it = InputActions.begin();
		while (it != InputActions.end())
		{
			InputValue keyValue;
			auto itNext = std::find_if(it, InputActions.end(), [&key, &keyValue](const auto& pair) -> bool {
				auto inputIt = pair.first->Inputs.find(key);
				if (inputIt != pair.first->Inputs.end())
				{
					keyValue = inputIt->second;
					return true;
				}
				return false;
			});

			if (itNext == InputActions.end()) break;

			itNext->second.Trigger(keyValue);
			bFoundAnyAction = true;

			it = itNext;
			it++;
		}
	}

	return false;//bFoundAnyAction;
}
