#pragma once

#include "Engine/Level/GameLevel.h"

class MainMenuWidget;
class MultiplayerMenuWidget;

class StartupLevel : public GameLevel
{
	typedef GameLevel Inherited;

public:
	StartupLevel();
	virtual ~StartupLevel();

	virtual void OpenLevel() override;
	virtual void CloseLevel() override;

	virtual void ReconstructLevel() override;

protected:
	void HandleMainMenuResponse(uint8_t responseType);

	void StartLocalPlay();
	void OpenMultiplayerMenu();

	void OpenCreateServerMenu();
	void MakeLocalServer();

	void OpenJoinServerMenu();
	void JoinLocalHost();

	void OpenLobbyLevel();

	void ReturnToMainMenu();

protected:
	TObjectPtr<MainMenuWidget> StartupMenuWidget;
};
