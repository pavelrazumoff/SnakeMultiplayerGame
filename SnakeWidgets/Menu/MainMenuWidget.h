#pragma once

#include "Engine/GameWidget/Components/UserWidget.h"
#include "SnakeDelegates.h"

class PanelWidget;
class VerticalBox;
class Button;
class TextBlock;
class PageContainerWidget;

enum class MainMenuResponse
{
	Other,

	/** Welcome page content. */

	StartGame,
	Multiplayer,
	ExitGame,

	/** Multiplayer page content. */

	CreateServer,
	JoinServer,
	ReturnToMainMenu,

	/** Create server page content. */

	MakeLocalServer,

	/** Join server page content. */

	JoinLocalHost,
};

enum class ContentPageType
{
	Welcome,
	Multiplayer,
	CreateServer,
	JoinServer,
};

class MainMenuWidget : public UserWidget
{
	typedef UserWidget Inherited;

public:
	MainMenuWidget();
	~MainMenuWidget();

	void OpenPage(ContentPageType page);

	/** Events. */

	MenuWidgetResponseDelegate& OnResponseEvent() { return ResponseEvent; }

protected:
	/** Welcome page content. */

	void HandleStartGameButtonClick(Button* instigator);
	void HandleMultiplayerButtonClick(Button* instigator);
	void HandleExitGameButtonClick(Button* instigator);

	/** Multiplayer page content. */

	void HandleCreateServerButtonClick(Button* instigator);
	void HandleJoinServerButtonClick(Button* instigator);
	void HandleReturnToMainMenuButtonClick(Button* instigator);

	/** Create server page content. */

	void HandleMakeLocalServerClick(Button* instigator);
	void HandleReturnToMultiplayerPageClick(Button* instigator);

	/** Join server page content. */

	void JoinLocalHostClick(Button* instigator);

protected:
	MenuWidgetResponseDelegate ResponseEvent;

protected:
	TObjectPtr<VerticalBox> MainVerticalBox;

	TObjectPtr<PanelWidget> TopPanelWidget;
	TObjectPtr<PanelWidget> BottomPanelWidget;

	TObjectPtr<PageContainerWidget> MainMenuContent;
};
