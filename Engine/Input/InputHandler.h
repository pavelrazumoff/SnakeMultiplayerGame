#pragma once

#include "InputUtility.h"

class IInputHandler
{
public:
	virtual bool PassInput(const InputState& is) = 0;
};
