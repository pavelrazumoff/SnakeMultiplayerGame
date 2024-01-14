#include "PlayerManager.h"

#include "Engine/EngineFactory.h"

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
	delete pPlayerState;
}

void PlayerManager::Initialize()
{
	if (PlayerStateClassName.empty()) { DebugEngineTrap(); return; }

	pPlayerState = dynamic_cast<PlayerState*>(EngineFactory::GetInstance().Create(PlayerStateClassName));
	if (!pPlayerState) DebugEngineTrap();
}

PlayerManager& PlayerManager::GetInstance()
{
	static PlayerManager instance;
	return instance;
}

void PlayerManager::SetPlayerStateClass(const std::string& className)
{
	if (PlayerStateClassName == className)
		return;

	PlayerStateClassName = className;

	if (pPlayerState)
	{
		delete pPlayerState;
		pPlayerState = nullptr;
	}
}
