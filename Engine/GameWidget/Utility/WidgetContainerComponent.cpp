#include "WidgetContainerComponent.h"

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
