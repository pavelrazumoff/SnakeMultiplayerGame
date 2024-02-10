#pragma once

#include "EngineTypes.h"
#include "EngineUtility.h"

#include <map>

class EngineFactory
{
private:
	EngineFactory() {}

public:
	~EngineFactory();

	static EngineFactory& GetInstance();

	void RegisterType(const std::string& type, EngineGenericType* prototype);
	EngineGenericType* Create(const std::string& type);

private:
	std::map<std::string, EngineGenericType*> prototypes;
};
