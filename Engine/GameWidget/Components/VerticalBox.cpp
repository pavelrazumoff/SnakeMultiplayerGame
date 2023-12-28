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

	for (size_t i = 0; i < underlayWidgetsCount; ++i)
	{
		GameWidget* underlayWidget = underlayWidgets[i];
		ApplyAlignmentSettings(this, underlayWidget);
	}
}
