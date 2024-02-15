#include "EngineFactory.h"

EngineFactory& EngineFactory::GetInstance()
{
	static EngineFactory instance;
	return instance;
}

void EngineFactory::Free()
{
	for (auto& it : prototypes)
	{
		if (it.second) delete it.second;
	}
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
		return it->second->CloneGeneric();
	}

	DebugEngineTrap();
	return nullptr;
}
