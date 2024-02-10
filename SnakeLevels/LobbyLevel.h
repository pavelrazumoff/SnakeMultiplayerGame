#pragma once

#include "Engine/Level/GameLevel.h"

class UserWidget;

class LobbyLevel : public GameLevel
{
	typedef GameLevel Inherited;

public:
	virtual void OpenLevel() override;
	virtual void CloseLevel() override;

	virtual void ReconstructLevel() override;

protected:
	TObjectPtr<UserWidget> pLobbyWidget;
};
