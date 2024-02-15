#pragma once

#include "Engine/GameWidget/Components/UserWidget.h"
#include "SnakeDelegates.h"

class VerticalBox;

class LobbyServerWidget : public UserWidget
{
	typedef UserWidget Inherited;

public:
	LobbyServerWidget();

protected:
	void HandlePlayerListChanged();

protected:
	TObjectPtr<VerticalBox> PlayerListVerticalBox; // TODO: Replace with scroll box.
};
