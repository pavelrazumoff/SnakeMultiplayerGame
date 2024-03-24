#pragma once

#include "Engine/GameWidget/Components/UserWidget.h"
#include "SnakeDelegates.h"
#include "Engine/Events/EventDelegate.h"

DECLARE_EVENT_DELEGATE(StartGameDelegate);

class VerticalBox;
class Button;

class LobbyServerWidget : public UserWidget
{
	typedef UserWidget Inherited;

public:
	LobbyServerWidget();

	virtual void Destroy() override;

	StartGameDelegate& OnStartGameClickEvent() { return StartGameClickEvent; }

protected:
	void HandlePlayerListChanged();
	void HandleStartGameButtonClick(Button* instigator);

protected:
	StartGameDelegate StartGameClickEvent;

protected:
	TObjectPtr<VerticalBox> PlayerListVerticalBox; // TODO: Replace with scroll box.
};
