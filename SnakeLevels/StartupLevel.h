#pragma once

#include "Engine/GameLevel.h"

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

protected:
	TObjectPtr<MainMenuWidget> StartupMenuWidget;
};
