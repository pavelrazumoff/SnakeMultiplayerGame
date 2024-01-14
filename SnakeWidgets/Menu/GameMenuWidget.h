#pragma once

#include "Engine/GameWidget/Components/UserWidget.h"
#include "SnakeDelegates.h"

class VerticalBox;
class PanelWidget;
class Button;
class TextBlock;

class GameMenuWidget : public UserWidget
{
	typedef UserWidget Inherited;

public:
	GameMenuWidget();
	~GameMenuWidget();

	MenuButtonClickDelegate& OnResumeClickEvent() { return ResumeClickEvent; }
	MenuButtonClickDelegate& OnExitGameClickEvent() { return ExitGameClickEvent; }

protected:
	void HandleResumeButtonClick(Button* instigator);
	void HandleExitGameButtonClick(Button* instigator);

protected:
	MenuButtonClickDelegate ResumeClickEvent;
	MenuButtonClickDelegate ExitGameClickEvent;

protected:
	TObjectPtr<VerticalBox> FrameVerticalBox;

	TObjectPtr<PanelWidget> MenuFrameWidget;

	TObjectPtr<TextBlock> CaptionText;

	TObjectPtr<Button> ResumeButton;
	TObjectPtr<TextBlock> ResumeButtonText;

	TObjectPtr<Button> ExitGameButton;
	TObjectPtr<TextBlock> ExitGameButtonText;
};
