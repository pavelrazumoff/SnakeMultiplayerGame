#include "GameWidgetManager.h"

#include "Engine/GameWidget/Components/UserWidget.h"
#include "Engine/Render/RenderManager.h"

GameWidgetManager::GameWidgetManager()
{
}

GameWidgetManager::~GameWidgetManager()
{
}

GameWidgetManager& GameWidgetManager::GetInstance()
{
	static GameWidgetManager instance;
	return instance;
}

void GameWidgetManager::PlaceUserWidgetOnScreen(UserWidget* userWidget)
{
	WndUserWidget = userWidget;
}

void GameWidgetManager::RequestWidgetReconstruction(GameWidget* Instigator)
{
	if (WndUserWidget.IsValid())
		WndUserWidget->ForceReconstruct(Instigator);
}

void GameWidgetManager::Update(float DeltaTime)
{
	if (WndUserWidget.IsValid())
		RenderManager::GetInstance().PushToRenderQueue(WndUserWidget.Get());
}

bool GameWidgetManager::PassInput(const InputState& is)
{
	return WndUserWidget.IsValid() ? WndUserWidget->PassInput(is) : false;
}
