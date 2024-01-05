#include "InputAction.h"

uint16_t InputAction::ActionCount = 0;

InputAction::InputAction()
{
	ActionCount++;
}
