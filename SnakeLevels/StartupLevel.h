#pragma once

#include "Engine/Level/GameLevel.h"

class MainMenuWidget;

class StartupLevel : public GameLevel
{
	typedef GameLevel Inherited;

public:
	StartupLevel();
	virtual ~StartupLevel();

	virtual void OpenLevel() override;
	virtual void Update(float DeltaTime) override;

	virtual void ReconstructLevel() override;

protected:
	void HandleMainMenuResponse(uint8_t responseType);

	void StartPlay();

protected:
	TObjectPtr<MainMenuWidget> StartupMenuWidget;
};
