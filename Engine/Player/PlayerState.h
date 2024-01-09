#pragma once

#include "Engine/EngineTypes.h"

class PlayerState : public EngineGenericType
{
public:
	PlayerState() {}
	virtual ~PlayerState() {}

	virtual [[nodiscard]] EngineGenericType* Clone() const override;
	virtual std::string GetTypeName() const override;
};
