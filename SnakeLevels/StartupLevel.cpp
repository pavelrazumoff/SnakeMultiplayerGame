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
