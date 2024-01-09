#include "EngineFactory.h"

EngineFactory::~EngineFactory()
{
	for (auto& it : prototypes)
	{
		delete it.second;
	}
}

EngineFactory& EngineFactory::GetInstance()
{
	static EngineFactory instance;
	return instance;
}

void EngineFactory::RegisterType(const std::string& type, EngineGenericType* prototype)
{
	prototypes[type] = prototype;
}

EngineGenericType* EngineFactory::Create(const std::string& type)
{
	auto it = prototypes.find(type);
	if (it != prototypes.end())
	{
		return it->second->Clone();
	}

	DebugEngineTrap();
	return nullptr;
}
