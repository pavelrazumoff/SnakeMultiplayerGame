#pragma once

#include "Engine/GameWidget/Components/UserWidget.h"

class PanelWidget;
class VerticalBox;
class Button;
class TextBlock;

class MainMenuWidget : public UserWidget
{
	typedef UserWidget Inherited;

public:
	MainMenuWidget();
	~MainMenuWidget();

protected:
	TObjectPtr<VerticalBox> MainVerticalBox;

	TObjectPtr<PanelWidget> TopPanelWidget;
	TObjectPtr<PanelWidget> BottomPanelWidget;

	TObjectPtr<Button> StartGameButton;
	TObjectPtr<TextBlock> StartGameButtonText;
};
