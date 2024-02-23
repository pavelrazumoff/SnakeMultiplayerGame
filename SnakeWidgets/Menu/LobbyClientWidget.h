#pragma once

#include "Engine/GameWidget/Components/UserWidget.h"
#include "SnakeDelegates.h"

class Button;
class TextEditBox;

class LobbyClientWidget : public UserWidget
{
	typedef UserWidget Inherited;

public:
	LobbyClientWidget();

protected:
	void HandleReady2PlayButtonClick(Button* instigator);

protected:
	TObjectPtr<TextEditBox> EditNameBox;
};
