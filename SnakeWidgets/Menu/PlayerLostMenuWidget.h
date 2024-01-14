#pragma once

#include "Engine/GameWidget/Components/UserWidget.h"
#include "Engine/Events/EventDelegate.h"

class VerticalBox;
class PanelWidget;
class Button;
class TextBlock;

DECLARE_EVENT_DELEGATE(MenuButtonClickDelegate);

class PlayerLostMenuWidget : public UserWidget
{
	typedef UserWidget Inherited;

public:
	PlayerLostMenuWidget();
	~PlayerLostMenuWidget();

	MenuButtonClickDelegate& OnPlayAgainClickEvent() { return PlayAgainClickEvent; }
	MenuButtonClickDelegate& OnExitGameClickEvent() { return ExitGameClickEvent; }

protected:
	void HandlePlayAgainButtonClick(Button* instigator);
	void HandleExitGameButtonClick(Button* instigator);

protected:
	MenuButtonClickDelegate PlayAgainClickEvent;
	MenuButtonClickDelegate ExitGameClickEvent;

protected:
	TObjectPtr<VerticalBox> FrameVerticalBox;

	TObjectPtr<PanelWidget> MenuFrameWidget;

	TObjectPtr<TextBlock> CaptionText;

	TObjectPtr<Button> PlayAgainButton;
	TObjectPtr<TextBlock> PlayAgainButtonText;

	TObjectPtr<Button> ExitGameButton;
	TObjectPtr<TextBlock> ExitGameButtonText;
};
