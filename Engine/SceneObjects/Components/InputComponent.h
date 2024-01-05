#pragma once

#include "ObjectComponent.h"

#include "Engine/Input/InputHandler.h"
#include "Engine/Input/InputAction.h"
#include "Engine/Events/EventDelegate.h"

#include <map>

DECLARE_EVENT_DELEGATE(InputActionDelegate, InputValue);

class InputComponent : public ObjectComponent, public IInputHandler
{
	typedef ObjectComponent Inherited;

public:
	InputComponent() {}
	virtual ~InputComponent() {}

	/** Extending the IInputHandler interface. */

	virtual bool PassInput(const InputState& is) override;

	/** Action bindings.  */

	template <class T>
	void BindAction(InputAction* action, T* instance, void (T::* method)(InputValue))
	{
		if (InputActions.find(action) == InputActions.end())
			InputActions[action].Subscribe(instance, method);
	}

protected:
	std::map<InputAction*, InputActionDelegate> InputActions;
};
