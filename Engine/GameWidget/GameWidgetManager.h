#pragma once

#include "Engine/GameObject/GameObjectPtr.h"
#include "Engine/Input/InputUtility.h"

class UserWidget;
class GameWidget;

class GameWidgetManager
{
public:
	GameWidgetManager();
	~GameWidgetManager();

	static GameWidgetManager& GetInstance();

	void PlaceUserWidgetOnScreen(UserWidget* userWidget);
	void RemoveUserWidgetFromScreen(UserWidget* userWidget);

	void RequestWidgetReconstruction(GameWidget* Instigator);

	bool GetBottomPlacedUserWidget(TObjectPtr<UserWidget>& userWidget) const;

	void Update(float DeltaTime);
	virtual bool PassInput(const InputState& is);

protected:
	std::vector<TObjectPtr<UserWidget>> UserWidgetList;
};
