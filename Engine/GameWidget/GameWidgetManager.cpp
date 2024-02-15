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
	UserWidgetList.push_back(userWidget);
}

void GameWidgetManager::RemoveUserWidgetFromScreen(UserWidget* userWidget)
{
	if (!userWidget) return;

	auto it = std::find_if(UserWidgetList.begin(), UserWidgetList.end(), [userWidget](TObjectPtr<UserWidget>& Other) -> bool {
		return Other.Get() == userWidget;
	});

	if (it != UserWidgetList.end())
		UserWidgetList.erase(it);
}

void GameWidgetManager::RequestWidgetReconstruction(GameWidget* Instigator)
{
	for (auto& userWidget : UserWidgetList)
	{
		if ((Instigator == userWidget.Get()) ||
			userWidget->DoesBelongToWidgetTree(Instigator))
		{
			AddToReconstructionQueue(userWidget.Get());
			return;
		}
	}
}

bool GameWidgetManager::GetBottomPlacedUserWidget(TObjectPtr<UserWidget>& userWidget) const
{
	if (UserWidgetList.empty()) return false;

	userWidget = UserWidgetList.back();
	return true;
}

void GameWidgetManager::Update(float DeltaTime)
{
	ReconstructUserWidgets();

	for (auto& userWidget : UserWidgetList)
	{
		if (userWidget.IsValid())
			RenderManager::GetInstance().PushToRenderQueue(userWidget.Get());
	}
}

bool GameWidgetManager::PassInput(const InputState& is)
{
	for (auto it = UserWidgetList.rbegin(); it != UserWidgetList.rend(); ++it)
	{
		if (it->IsValid())
		{
			if (it->Get()->PassInput(is))
				return true;

			// Always consume input from modal widgets (windows).
			if (it->Get()->IsWidgetModal()) return true;
		}
	}
	return false;
}

void GameWidgetManager::ReconstructUserWidgets()
{
	while (!WaitingReconstructionQueue.empty())
	{
		auto userWidget = WaitingReconstructionQueue.front();
		WaitingReconstructionQueue.erase(WaitingReconstructionQueue.begin());

		if (userWidget.IsValid()) userWidget->ForceReconstruct();
	}
}

void GameWidgetManager::DestroyGameWidget(GameWidget* widget)
{
	for (auto& userWidget : UserWidgetList)
	{
		if(userWidget.IsValid() &&
			userWidget->RemoveWidgetFromUserWigdet(widget))
		{
			AddToReconstructionQueue(userWidget.Get());
			return;
		}
	}
}

void GameWidgetManager::AddToReconstructionQueue(UserWidget* userWidget)
{
	auto it = std::find_if(WaitingReconstructionQueue.begin(), WaitingReconstructionQueue.end(),
		[userWidget](TObjectPtr<UserWidget>& Other) -> bool {
		return Other.Get() == userWidget;
	});

	if (it == WaitingReconstructionQueue.end())
		WaitingReconstructionQueue.push_back(userWidget);
}
