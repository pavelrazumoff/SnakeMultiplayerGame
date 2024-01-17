#include "AlignmentSettings.h"

#include "../Components/GameWidget.h"
#include "WidgetContainerComponent.h"

void ApplyAlignmentSettings(GameWidget* parentWidget, GameWidget* underlayWidget)
{
	/*
		TODO: Take parentWidget, get some component that handle positioning inside a widget.
		Then take the free rect space from it, where we can put this underlayWidget.
		After calling RepositionWidget on underlayWidget, we need to update the free rect space of the parent widget.
	*/

	WidgetContainerComponent* parentContainerComponent = GameWidget::FindWidgetComponent<WidgetContainerComponent>(parentWidget);
	RC_RECT parentRect = parentContainerComponent ? parentContainerComponent->GetContainerFreeRect() : parentWidget->GetCachedActualRect();

	ApplyAlignmentSettings(underlayWidget, parentRect);
	if (parentContainerComponent) parentContainerComponent->PlaceInContainer(underlayWidget->GetCachedActualRect());
}

void ApplyAlignmentSettings(GameWidget* underlayWidget, RC_RECT parentRect)
{
	const AlignmentSettings* alignmentSettings = GameWidget::FindWidgetComponent<AlignmentSettings>(underlayWidget);
	if (!alignmentSettings) return;

	RC_SIZE underlayWidgetDirtySize = underlayWidget->GetCachedDirtySize();
	RC_RECT newUnderlayWidgetRect = { 0 };

	const bool bAllowStretch = (alignmentSettings->Stretch != AlignmentSettings::StretchMode::NoStretch);

	switch (alignmentSettings->Horizontal)
	{
		case AlignmentSettings::HorizontalAlignment::NoAlignment:
			{
				if (bAllowStretch || underlayWidgetDirtySize.cx == 0)
				{
					newUnderlayWidgetRect.left = parentRect.left + alignmentSettings->Padding.left;
					newUnderlayWidgetRect.right = parentRect.right - alignmentSettings->Padding.right;
				}
				else
				{
					newUnderlayWidgetRect.left = parentRect.left + alignmentSettings->Padding.left;
					newUnderlayWidgetRect.right = newUnderlayWidgetRect.left + underlayWidgetDirtySize.cx - 1;
				}
			}
			break;
		case AlignmentSettings::HorizontalAlignment::Left:
			{
				newUnderlayWidgetRect.left = parentRect.left + alignmentSettings->Padding.left;
				newUnderlayWidgetRect.right = newUnderlayWidgetRect.left + underlayWidgetDirtySize.cx - 1;
			}
			break;
		case AlignmentSettings::HorizontalAlignment::Center:
			{
				newUnderlayWidgetRect.left = parentRect.left + (parentRect.right - parentRect.left + 1 - underlayWidgetDirtySize.cx) / 2 + alignmentSettings->Padding.left;
				newUnderlayWidgetRect.right = newUnderlayWidgetRect.left + underlayWidgetDirtySize.cx - 1;
			}
			break;
		case AlignmentSettings::HorizontalAlignment::Right:
			{
				newUnderlayWidgetRect.right = parentRect.right - alignmentSettings->Padding.right;
				newUnderlayWidgetRect.left = newUnderlayWidgetRect.right - underlayWidgetDirtySize.cx + 1;
			}
			break;
		default:
			break;
	}

	switch (alignmentSettings->Vertical)
	{
		case AlignmentSettings::VerticalAlignment::NoAlignment:
			{
				if (bAllowStretch || underlayWidgetDirtySize.cy == 0)
				{
					newUnderlayWidgetRect.top = parentRect.top + alignmentSettings->Padding.top;
					newUnderlayWidgetRect.bottom = parentRect.bottom - alignmentSettings->Padding.bottom;
				}
				else
				{
					newUnderlayWidgetRect.top = parentRect.top + alignmentSettings->Padding.top;
					newUnderlayWidgetRect.bottom = newUnderlayWidgetRect.top + underlayWidgetDirtySize.cy - 1;
				}
			}
			break;
		case AlignmentSettings::VerticalAlignment::Top:
			{
				newUnderlayWidgetRect.top = parentRect.top + alignmentSettings->Padding.top;
				newUnderlayWidgetRect.bottom = newUnderlayWidgetRect.top + underlayWidgetDirtySize.cy - 1;
			}
			break;
		case AlignmentSettings::VerticalAlignment::Center:
			{
				newUnderlayWidgetRect.top = parentRect.top + (parentRect.bottom - parentRect.top + 1 - underlayWidgetDirtySize.cy) / 2 + alignmentSettings->Padding.top;
				newUnderlayWidgetRect.bottom = newUnderlayWidgetRect.top + underlayWidgetDirtySize.cy - 1;
			}
			break;
		case AlignmentSettings::VerticalAlignment::Bottom:
			{
				newUnderlayWidgetRect.bottom = parentRect.bottom - alignmentSettings->Padding.bottom;
				newUnderlayWidgetRect.top = newUnderlayWidgetRect.bottom - underlayWidgetDirtySize.cy + 1;
			}
			break;
		default:
			break;
	}

	underlayWidget->RepositionWidget(newUnderlayWidgetRect);
}

RC_SIZE GetWidgetOccupiedSize(GameWidget* underlayWidget)
{
	const AlignmentSettings* alignmentSettings = GameWidget::FindWidgetComponent<AlignmentSettings>(underlayWidget);
	if (!alignmentSettings) return RC_SIZE();

	RC_SIZE underlayWidgetDirtySize = underlayWidget->GetCachedDirtySize();
	RC_SIZE occupiedSize = { alignmentSettings->Padding.left + underlayWidgetDirtySize.cx + alignmentSettings->Padding.right,
		alignmentSettings->Padding.top + underlayWidgetDirtySize.cy + alignmentSettings->Padding.bottom };

	return occupiedSize;
}
