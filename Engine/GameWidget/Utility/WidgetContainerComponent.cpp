#include "WidgetContainerComponent.h"

#include "Engine/GameWidget/Components/GameWidget.h"

void WidgetContainerComponent::PutWidgetInContainer(GameWidget* widget)
{
	WidgetsInContainer.push_back(widget);
}

bool WidgetContainerComponent::RemoveWidgetFromContainer(GameWidget* widget)
{
	auto it = std::find_if(WidgetsInContainer.begin(), WidgetsInContainer.end(), [widget](const TObjectPtr<GameWidget>& other) -> bool {
		return widget == other.Get();
	});

	if (it != WidgetsInContainer.end())
	{
		WidgetsInContainer.erase(it);
		return true;
	}

	return false;
}

void WidgetContainerComponent::ClearContainer()
{
	while (!WidgetsInContainer.empty())
	{
		WidgetsInContainer.front()->Destroy();
	}
}

uint32_t WidgetContainerComponent::GetNumWidgetsInContainer() const
{
	return (uint32_t)WidgetsInContainer.size();
}

TObjectPtr<GameWidget> WidgetContainerComponent::GetWidgetFromContainer(int index)
{
	return (index >= 0 && index < WidgetsInContainer.size()) ? WidgetsInContainer[index].Get() : nullptr;
}

void WidgetContainerComponent::ResetContainerFreeRect(RC_RECT rect)
{
	ContainerFreeRect = rect;
}

void WidgetContainerComponent::ResetAlignment(bool bVertical)
{
	bVerticalAlignment = bVertical;
}

void WidgetContainerComponent::PlaceInContainer(const RC_RECT& occupiedRect)
{
	// TODO: Check for overflow.
	if (bVerticalAlignment)
	{
		ContainerFreeRect.top = occupiedRect.bottom + 1;
	}
	else
	{
		ContainerFreeRect.left = occupiedRect.right + 1;
	}
}
