#include "LobbyLevel.h"

#include "SnakeWidgets/Menu/LobbyServerWidget.h"
#include "SnakeWidgets/Menu/LobbyClientWidget.h"

#include "Engine/GameWidget/GameWidgetManager.h"
#include "Engine/Network/NetworkManager.h"
#include "Engine/Level/LevelManager.h"

#include "Core/RenderConsoleLibrary.h"

#include "Engine/Player/PlayerManager.h"
#include "SnakePlayer/LobbyPlayerController.h"

#include "PlayLevel.h"

LobbyLevel::LobbyLevel()
{
	PlayerManager::GetInstance().SetPlayerControllerClass("LobbyPlayerController");
	PlayerManager::GetInstance().SetPlayerStateClass("LobbyPlayerState");
}

void LobbyLevel::OpenLevel()
{
	Inherited::OpenLevel();

	if (!pLobbyWidget.Get())
	{
		if (NetworkManager::GetInstance().IsServer())
		{
			LobbyServerWidget* lobbyServerWidget = CreateNewObject<LobbyServerWidget>(this);
			pLobbyWidget = lobbyServerWidget;

			lobbyServerWidget->OnStartGameClickEvent().Subscribe(this, &LobbyLevel::HandleStartGameBtnClicked);
		}
		else
		{
			LobbyClientWidget* lobbyClientWidget = CreateNewObject<LobbyClientWidget>(this);
			pLobbyWidget = lobbyClientWidget;

			lobbyClientWidget->OnReady2PlayClickEvent().Subscribe(this, &LobbyLevel::HandleReadyToPlayBtnClicked);
		}

		GameWidgetManager::GetInstance().PlaceUserWidgetOnScreen(pLobbyWidget.Get());
	}

	if (NetworkManager::GetInstance().IsClient())
	{
		if (LobbyPlayerController* playerState = dynamic_cast<LobbyPlayerController*>(PlayerManager::GetInstance().GetPlayerController()))
		{
			playerState->OnPlayerWaitGameEvent().Subscribe(this, &LobbyLevel::HandlePlayerWaitGameEvent);
		}
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

void LobbyLevel::HandleStartGameBtnClicked()
{
	PlayLevel* playLevel = CreateNewObject<PlayLevel>();
	LevelManager::GetInstance().OpenLevel(playLevel);
}

void LobbyLevel::HandleReadyToPlayBtnClicked(const char* playerName)
{
	if (LobbyPlayerController* playerController = dynamic_cast<LobbyPlayerController*>(PlayerManager::GetInstance().GetPlayerController()))
	{
		playerController->GetPlayerState()->SetPlayerName(playerName);
		playerController->SetPlayerReady();
	}
}

void LobbyLevel::HandlePlayerWaitGameEvent()
{
	if (auto lobbyClientWidget = dynamic_cast<LobbyClientWidget*>(pLobbyWidget.Get()))
		lobbyClientWidget->SetWaitForGameStartMode();
}
