#include "VerticalBox.h"

VerticalBox::VerticalBox()
{
	Alignment.Stretch = AlignmentSettings::StretchMode::Fill;
	Alignment.Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
	Alignment.Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;

	AddWidgetComponent(&Alignment);
	AddWidgetComponent(&ContainerComponent);
}

VerticalBox::~VerticalBox()
{
}

void VerticalBox::ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount)
{
	RC_RECT actualRect = GetCachedActualRect();
	ContainerComponent.ResetContainerFreeRect(actualRect);
	ContainerComponent.ResetAlignment(true);

	RC_RECT containerFreeRect = ContainerComponent.GetContainerFreeRect();
	RC_UINT occupiedHeight = 0;
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
			occupiedHeight += underlayWidgetOccupiedSize.cy;
		}
	}

	int16_t availableContainerHeight = GetTexHeight(containerFreeRect);
	availableContainerHeight -= occupiedHeight;
	if (availableContainerHeight < 0) { DebugEngineTrap(); return; }

	RC_UINT stretchedFreeHeight = availableContainerHeight;
	if (numStretchElements > 0) stretchedFreeHeight /= numStretchElements;

	for (size_t i = 0; i < underlayWidgetsCount; ++i)
	{
		const AlignmentSettings* alignmentSettings = GameWidget::FindWidgetComponent<AlignmentSettings>(underlayWidgets[i]);
		if (!alignmentSettings) continue;

		const bool bAllowStretch = (alignmentSettings->Stretch != AlignmentSettings::StretchMode::NoStretch);
		if (bAllowStretch)
		{
			containerFreeRect.bottom = containerFreeRect.top + stretchedFreeHeight - 1;

			ApplyAlignmentSettings(underlayWidgets[i], containerFreeRect);
		}
		else
		{
			RC_SIZE underlayWidgetOccupiedSize = GetWidgetOccupiedSize(underlayWidgets[i]);
			containerFreeRect.bottom = containerFreeRect.top + underlayWidgetOccupiedSize.cy - 1;

			ApplyAlignmentSettings(underlayWidgets[i], containerFreeRect);
		}

		// Prepare container free space for the next widget.
		containerFreeRect.top = containerFreeRect.bottom + 1;
	}

	// Just in case we inherit HorizontalBox and override this method, it would be fine to actualize the container's free rect after we made our calcs here.
	containerFreeRect.bottom = ContainerComponent.GetContainerFreeRect().bottom;
	ContainerComponent.ResetContainerFreeRect(containerFreeRect);
}
