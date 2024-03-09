#include "LobbyLevel.h"

#include "SnakeWidgets/Menu/LobbyServerWidget.h"
#include "SnakeWidgets/Menu/LobbyClientWidget.h"

#include "Engine/GameWidget/GameWidgetManager.h"
#include "Engine/Network/NetworkManager.h"

#include "Core/RenderConsoleLibrary.h"

#include "Engine/Player/PlayerManager.h"
#include "SnakePlayer/LobbyPlayerState.h"

LobbyLevel::LobbyLevel()
{
	PlayerManager::GetInstance().SetPlayerStateClass("LobbyPlayerState");
}

void LobbyLevel::OpenLevel()
{
	Inherited::OpenLevel();

	if (!pLobbyWidget.Get())
	{
		if (NetworkManager::GetInstance().IsServer())
			pLobbyWidget = CreateNewObject<LobbyServerWidget>(this);
		else
		{
			LobbyClientWidget* lobbyClientWidget = CreateNewObject<LobbyClientWidget>(this);
			pLobbyWidget = lobbyClientWidget;

			lobbyClientWidget->OnReady2PlayClickEvent().Subscribe(this, &LobbyLevel::HandleReadyToPlayBtnClicked);
		}

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

void LobbyLevel::HandleReadyToPlayBtnClicked(const char* playerName)
{
	if (LobbyPlayerState* playerState = dynamic_cast<LobbyPlayerState*>(PlayerManager::GetInstance().GetPlayerState()))
	{
		playerState->SetPlayerName(playerName);
		playerState->SetPlayerReady();
	}
}
