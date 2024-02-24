#pragma once

#include "Engine/Level/GameLevel.h"

class MainMenuWidget;
class MultiplayerMenuWidget;

REGISTER_CLASS(StartupLevel)
class StartupLevel : public GameLevel
{
	typedef GameLevel Inherited;

	GAMEOBJECT_BODY(StartupLevel)

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

	/** Client Events. */

	void HandleJoinServerSuccess();
	void HandleJoinServerFailure();

protected:
	TObjectPtr<MainMenuWidget> StartupMenuWidget;
};
