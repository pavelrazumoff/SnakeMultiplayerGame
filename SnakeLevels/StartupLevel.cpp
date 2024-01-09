#include "StartupLevel.h"

#include "SnakeWidgets/Menu/MainMenuWidget.h"
#include "Engine/GameObject/GameObjectUtility.h"
#include "Engine/Level/LevelManager.h"
#include "Engine/GameWidget/GameWidgetManager.h"

#include "Core/RenderConsoleLibrary.h"

#include "PlayLevel.h"

StartupLevel::StartupLevel()
{
}

StartupLevel::~StartupLevel()
{
}

void StartupLevel::OpenLevel()
{
	Inherited::OpenLevel();

	if (!StartupMenuWidget.Get())
	{
		StartupMenuWidget = CreateNewObject<MainMenuWidget>(this);
		if (StartupMenuWidget.Get())
			StartupMenuWidget->OnResponseEvent().Subscribe(this, &StartupLevel::HandleMainMenuResponse);

		GameWidgetManager::GetInstance().PlaceUserWidgetOnScreen(StartupMenuWidget.Get());
	}

	ReconstructLevel();
}

void StartupLevel::ReconstructLevel()
{
	if (!StartupMenuWidget.Get()) return;
	
	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
	StartupMenuWidget->SetCanvasDimensions(consoleDim.cx, consoleDim.cy);
}

void StartupLevel::HandleMainMenuResponse(uint8_t responseType)
{
	MainMenuResponse mainMenuResponse = static_cast<MainMenuResponse>(responseType);
	switch (mainMenuResponse)
	{
		case MainMenuResponse::StartGame:
			StartPlay();
			break;
		case MainMenuResponse::ExitGame:
			LevelManager::GetInstance().CloseLevel(this);
			break;
		default:
			break;
	}
}

void StartupLevel::StartPlay()
{
	PlayLevel* playLevel = CreateNewObject<PlayLevel>();
	LevelManager::GetInstance().OpenLevel(playLevel);
}
