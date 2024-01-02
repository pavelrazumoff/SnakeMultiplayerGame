#pragma once

#include "Engine/GameWidget/Components/UserWidget.h"
#include "Engine/Events/EventDelegate.h"

class PanelWidget;
class VerticalBox;
class Button;
class TextBlock;

enum class MainMenuResponse
{
	Other,

	StartGame,
	ExitGame
};

DECLARE_EVENT_DELEGATE(MenuWidgetResponseDelegate, uint8_t);

class MainMenuWidget : public UserWidget
{
	typedef UserWidget Inherited;

public:
	MainMenuWidget();
	~MainMenuWidget();

	/** Events. */

	MenuWidgetResponseDelegate& OnResponseEvent() { return ResponseEvent; }

protected:
	void HandleStartGameButtonClick(Button* instigator);
	void HandleExitGameButtonClick(Button* instigator);

protected:
	MenuWidgetResponseDelegate ResponseEvent;

protected:
	TObjectPtr<VerticalBox> MainVerticalBox;

	TObjectPtr<PanelWidget> TopPanelWidget;
	TObjectPtr<PanelWidget> BottomPanelWidget;

	TObjectPtr<Button> StartGameButton;
	TObjectPtr<TextBlock> StartGameButtonText;

	TObjectPtr<Button> ExitGameButton;
	TObjectPtr<TextBlock> ExitGameButtonText;

	TObjectPtr<TextBlock> WelcomeCaptionText;
};
