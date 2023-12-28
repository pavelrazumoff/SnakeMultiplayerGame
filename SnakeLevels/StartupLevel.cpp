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
	}

	ReconstructLevel();
}

void StartupLevel::Update(float DeltaTime)
{
}

void StartupLevel::ReconstructLevel()
{
	RenderManager::GetInstance()->ClearRenderBuffer();

	if (!StartupMenuWidget.Get()) return;
	
	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
	StartupMenuWidget->SetCanvasDimensions(consoleDim.cx, consoleDim.cy);

	RenderManager::GetInstance()->PushToRenderQueue(StartupMenuWidget.Get());
}
