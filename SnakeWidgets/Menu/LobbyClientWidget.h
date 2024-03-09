#pragma once

#include "Engine/GameWidget/Components/UserWidget.h"
#include "Engine/Events/EventDelegate.h"

DECLARE_EVENT_DELEGATE(Ready2PlayDelegate, const char*);

class Button;
class TextEditBox;

class LobbyClientWidget : public UserWidget
{
	typedef UserWidget Inherited;

public:
	LobbyClientWidget();

	Ready2PlayDelegate& OnReady2PlayClickEvent() { return Ready2PlayClickEvent; }

protected:
	void HandleReady2PlayButtonClick(Button* instigator);

protected:
	Ready2PlayDelegate Ready2PlayClickEvent;

protected:
	TObjectPtr<TextEditBox> EditNameBox;
};
