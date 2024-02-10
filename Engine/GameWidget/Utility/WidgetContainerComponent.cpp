#include "WidgetContainerComponent.h"

#include "Engine/GameWidget/Components/GameWidget.h"

void WidgetContainerComponent::PutWidgetInContainer(GameWidget* widget)
{
	WidgetsInContainer.push_back(widget);
}

uint32_t WidgetContainerComponent::GetNumWidgetsInContainer() const
{
	return WidgetsInContainer.size();
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
