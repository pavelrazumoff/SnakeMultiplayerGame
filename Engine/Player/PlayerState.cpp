#include "PlayerState.h"

EngineGenericType* PlayerState::Clone() const
{
	return new PlayerState();
}

std::string PlayerState::GetTypeName() const
{
	return "PlayerState";
}
