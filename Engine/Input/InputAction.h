#pragma once

#include "stdint.h"
#include <map>

union InputValue
{
	bool bValue;
	float AxisValue;
};

struct InputAction
{
public:
	InputAction();

public:
	std::map<uint16_t, InputValue> Inputs;

private:
	static uint16_t ActionCount;

	uint16_t UID = 0;
};
