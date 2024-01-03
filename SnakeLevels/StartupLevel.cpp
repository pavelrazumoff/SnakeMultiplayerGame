#include "StartupLevel.h"

#include "SnakeWidgets/Menu/MainMenuWidget.h"
#include "Engine/GameObject/GameObjectUtility.h"
#include "Engine/Render/RenderManager.h"

#include "Core/RenderConsoleLibrary.h"

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
		if(StartupMenuWidget.Get())
			StartupMenuWidget->OnResponseEvent().Subscribe(this, &StartupLevel::HandleMainMenuResponse);

		PlaceObjectOnLevel(StartupMenuWidget.Get());
	}

	ReconstructLevel();
}

void StartupLevel::Update(float DeltaTime)
{
	Inherited::Update(DeltaTime);

	RenderManager::GetInstance()->PushToRenderQueue(StartupMenuWidget.Get());
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
			break;
		case MainMenuResponse::ExitGame:
			LevelCloseEvent.Trigger(this); // TODO: Replace with calling the LevelManager (OpenLevel/CloseLevel). Engine will subscribe to it's events.
			break;
		default:
			break;
	}
}
