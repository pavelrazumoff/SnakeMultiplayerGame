#include "HorizontalBox.h"

HorizontalBox::HorizontalBox()
{
	Alignment.Stretch = AlignmentSettings::StretchMode::Fill;
	Alignment.Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
	Alignment.Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;

	AddWidgetComponent(&Alignment);
	AddWidgetComponent(&ContainerComponent);
}

HorizontalBox::~HorizontalBox()
{
}

void HorizontalBox::ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount)
{
	RC_RECT actualRect = GetCachedActualRect();
	ContainerComponent.ResetContainerFreeRect(actualRect);
	ContainerComponent.ResetAlignment(false);

	/*
		How UE works.

		1. If element's size is set to Fill, it occupies all of the remaining space.
		2. If element's size is set to Auto, it occupies only the space it needs based on its content.
		3. Consider we set the size to Fill, then
			3.1. No matter what alignment we choose, the element will be positioned relative to its occupied space.
				3.1.1. So, if we would choose Auto instead of Fill, nothing would change.
				3.1.2. If we choose Fill, then we must position the element to the left, center or right edge of the precalc occupied space.
				We don't need to look at our neighbors.
	*/

	RC_RECT containerFreeRect = ContainerComponent.GetContainerFreeRect();
	RC_UINT occupiedWidth = 0;
	RC_UINT numStretchElements = 0;

	// We need first to look at our neighbors.
	// We go through each of it, take all that has stretch disabled, calc the sum of theirs dirty sizes,
	// subtract it from containerFreeRect, and then divide the remaining space by the number of elements that has stretch enabled.
	for (size_t i = 0; i < underlayWidgetsCount; ++i)
	{
		const AlignmentSettings* alignmentSettings = GameWidget::FindWidgetComponent<AlignmentSettings>(underlayWidgets[i]);
		if (!alignmentSettings) continue;

		const bool bAllowStretch = (alignmentSettings->Stretch != AlignmentSettings::StretchMode::NoStretch);
		if (bAllowStretch)
		{
			numStretchElements++;
		}
		else
		{
			RC_SIZE underlayWidgetOccupiedSize = GetWidgetOccupiedSize(underlayWidgets[i]);
			occupiedWidth += underlayWidgetOccupiedSize.cx;
		}
	}

	int16_t availableContainerWidth = GetTexWidth(containerFreeRect);
	availableContainerWidth -= occupiedWidth;
	if (availableContainerWidth < 0) { DebugEngineTrap(); return; }

	RC_UINT stretchedFreeWidth = availableContainerWidth;
	if (numStretchElements > 0) stretchedFreeWidth /= numStretchElements;

	for (size_t i = 0; i < underlayWidgetsCount; ++i)
	{
		const AlignmentSettings* alignmentSettings = GameWidget::FindWidgetComponent<AlignmentSettings>(underlayWidgets[i]);
		if (!alignmentSettings) continue;

		const bool bAllowStretch = (alignmentSettings->Stretch != AlignmentSettings::StretchMode::NoStretch);
		if (bAllowStretch)
		{
			containerFreeRect.right = containerFreeRect.left + stretchedFreeWidth - 1;

			ApplyAlignmentSettings(underlayWidgets[i], containerFreeRect);
		}
		else
		{
			RC_SIZE underlayWidgetOccupiedSize = GetWidgetOccupiedSize(underlayWidgets[i]);
			containerFreeRect.right = containerFreeRect.left + underlayWidgetOccupiedSize.cx - 1;

			ApplyAlignmentSettings(underlayWidgets[i], containerFreeRect);
		}

		// Prepare container free space for the next widget.
		containerFreeRect.left = containerFreeRect.right + 1;
	}

	// Just in case we inherit HorizontalBox and override this method, it would be fine to actualize the container's free rect after we made our calcs here.
	containerFreeRect.right = ContainerComponent.GetContainerFreeRect().right;
	ContainerComponent.ResetContainerFreeRect(containerFreeRect);
}
