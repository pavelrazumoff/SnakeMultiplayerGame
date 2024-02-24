#pragma once

#include "Engine/Level/GameLevel.h"

class UserWidget;

REGISTER_CLASS(LobbyLevel)
class LobbyLevel : public GameLevel
{
	typedef GameLevel Inherited;

	GAMEOBJECT_BODY(LobbyLevel)

public:
	virtual void OpenLevel() override;
	virtual void CloseLevel() override;

	virtual void ReconstructLevel() override;

protected:
	TObjectPtr<UserWidget> pLobbyWidget;
};
