#include "LobbyLevel.h"

#include "SnakeWidgets/Menu/LobbyServerWidget.h"
#include "SnakeWidgets/Menu/LobbyClientWidget.h"

#include "Engine/GameWidget/GameWidgetManager.h"
#include "Engine/Network/NetworkManager.h"

#include "Core/RenderConsoleLibrary.h"

void LobbyLevel::OpenLevel()
{
	Inherited::OpenLevel();

	if (!pLobbyWidget.Get())
	{
		if (NetworkManager::GetInstance().IsServer())
			pLobbyWidget = CreateNewObject<LobbyServerWidget>(this);
		else
			pLobbyWidget = CreateNewObject<LobbyClientWidget>(this);

		GameWidgetManager::GetInstance().PlaceUserWidgetOnScreen(pLobbyWidget.Get());
	}

	ReconstructLevel();
}

void LobbyLevel::CloseLevel()
{
	Inherited::CloseLevel();

	// TODO: Place it to the parent.
	GameWidgetManager::GetInstance().RemoveUserWidgetFromScreen(pLobbyWidget.Get());
}

void LobbyLevel::ReconstructLevel()
{
	if (!pLobbyWidget.IsValid()) { DebugEngineTrap(); return; }

	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
	pLobbyWidget->SetCanvasDimensions(consoleDim.cx, consoleDim.cy);
}
