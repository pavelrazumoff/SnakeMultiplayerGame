#include "StartupLevel.h"

#include "SnakeWidgets/Menu/MainMenuWidget.h"

#include "Engine/GameObject/GameObjectUtility.h"
#include "Engine/Level/LevelManager.h"
#include "Engine/GameWidget/GameWidgetManager.h"

#include "Core/RenderConsoleLibrary.h"

#include "Engine/Player/PlayerManager.h"
#include "Engine/Network/NetworkManager.h"
#include "Engine/Log/Logger.h"

#include "PlayLevel.h"
#include "LobbyLevel.h"

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

void StartupLevel::CloseLevel()
{
	Inherited::CloseLevel();

	GameWidgetManager::GetInstance().RemoveUserWidgetFromScreen(StartupMenuWidget.Get());
}

void StartupLevel::ReconstructLevel()
{
	TObjectPtr<UserWidget> bottomPlacedWidget;
	if (!GameWidgetManager::GetInstance().GetBottomPlacedUserWidget(bottomPlacedWidget)) return;

	if (!bottomPlacedWidget.Get()) return;
	
	RC_SIZE consoleDim = RenderConsoleLibrary::GetConsoleDimensions();
	bottomPlacedWidget->SetCanvasDimensions(consoleDim.cx, consoleDim.cy);
}

void StartupLevel::HandleMainMenuResponse(uint8_t responseType)
{
	MainMenuResponse mainMenuResponse = static_cast<MainMenuResponse>(responseType);
	switch (mainMenuResponse)
	{
		/** Welcome page. */

		case MainMenuResponse::StartGame:
			StartLocalPlay();
			break;
		case MainMenuResponse::Multiplayer:
			OpenMultiplayerMenu();
			break;
		case MainMenuResponse::ExitGame:
			LevelManager::GetInstance().CloseLevel(this);
			break;

		/** Mutliplayer page. */

		case MainMenuResponse::CreateServer:
			OpenCreateServerMenu();
			break;
		case MainMenuResponse::JoinServer:
			OpenJoinServerMenu();
			break;
		case MainMenuResponse::ReturnToMainMenu:
			ReturnToMainMenu();
			break;

		/** Create Server page. */

		case MainMenuResponse::MakeLocalServer:
			MakeLocalServer();
			break;

		/** Join Server page. */

		case MainMenuResponse::JoinLocalHost:
			JoinLocalHost();
			break;

		default:
			break;
	}
}

void StartupLevel::StartLocalPlay()
{
	auto localPlayer = PlayerManager::GetInstance().MakeNewPlayer();
	if (!localPlayer) { DebugEngineTrap(); return; }

	PlayLevel* playLevel = CreateNewObject<PlayLevel>();
	LevelManager::GetInstance().OpenLevel(playLevel);
}

void StartupLevel::OpenMultiplayerMenu()
{
	StartupMenuWidget->OpenPage(ContentPageType::Multiplayer);
}

void StartupLevel::OpenCreateServerMenu()
{
	StartupMenuWidget->OpenPage(ContentPageType::CreateServer);
}

void StartupLevel::MakeLocalServer()
{
	if (!NetworkManager::GetInstance().MakeListenServer())
	{
		// TODO: Print some error message.
		return;
	}

	OpenLobbyLevel();
}

void StartupLevel::OpenJoinServerMenu()
{
	StartupMenuWidget->OpenPage(ContentPageType::JoinServer);
}

void StartupLevel::JoinLocalHost()
{
	if (!NetworkManager::GetInstance().JoinServer("127.0.0.1:48000"))
	{
		// TODO: Print some error message.
		return;
	}

	NetworkManager::GetInstance().OnJoinServerSuccess().Subscribe(this, &StartupLevel::HandleJoinServerSuccess);
	NetworkManager::GetInstance().OnJoinServerFailure().Subscribe(this, &StartupLevel::HandleJoinServerFailure);
}

void StartupLevel::OpenLobbyLevel()
{
	LobbyLevel* lobbyLevel = CreateNewObject<LobbyLevel>();
	LevelManager::GetInstance().OpenLevel(lobbyLevel);
}

void StartupLevel::ReturnToMainMenu()
{
	StartupMenuWidget->OpenPage(ContentPageType::Welcome);
}

void StartupLevel::HandleJoinServerSuccess()
{
	Logger::GetInstance().Write("Join server success.");
}

void StartupLevel::HandleJoinServerFailure()
{
	Logger::GetInstance().Write("Join server failure.");
}
