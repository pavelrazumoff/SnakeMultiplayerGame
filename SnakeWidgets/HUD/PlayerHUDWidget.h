#pragma once

#include "Engine/GameWidget/Components/UserWidget.h"
#include "SnakeDelegates.h"

class HorizontalBox;
class TextBlock;
class PanelWidget;
class Button;

class PlayerHUDWidget : public UserWidget
{
	typedef UserWidget Inherited;

public:
	PlayerHUDWidget();
	~PlayerHUDWidget();

	void SetScore(uint32_t score);
	void SetFPSCounter(uint32_t fps);

	RC_RECT GetScreenFreeRect() const;

	MenuButtonClickDelegate& OnMenuOpenClickEvent() { return MenuOpenClickEvent; }

protected:
	void HandleOpenMenuButtonClick(Button* instigator);

protected:
	MenuButtonClickDelegate MenuOpenClickEvent;

protected:
	TObjectPtr<PanelWidget> TopPanelWidget;
	TObjectPtr<HorizontalBox> TopHorizontalBox;

	TObjectPtr<TextBlock> ScoreText;
	TObjectPtr<TextBlock> FPSCounterText;

	TObjectPtr<Button> MenuButton;
	TObjectPtr<TextBlock> MenuButtonText;
};
