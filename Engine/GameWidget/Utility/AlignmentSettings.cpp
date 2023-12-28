#include "AlignmentSettings.h"

#include "../Components/GameWidget.h"
#include "WidgetContainerComponent.h"

void ApplyAlignmentSettings(GameWidget* parentWidget, GameWidget* underlayWidget)
{
	const AlignmentSettings* alignmentSettings = GameWidget::FindWidgetComponent<AlignmentSettings>(underlayWidget);
	if (!alignmentSettings) return;

	/*
		TODO: Take parentWidget, get some component that handle positioning inside a widget.
		Then take the free rect space from it, where we can put this underlayWidget.
		After calling RepositionWidget on underlayWidget, we need to update the free rect space of the parent widget.
	*/

	WidgetContainerComponent* parentContainerComponent = GameWidget::FindWidgetComponent<WidgetContainerComponent>(parentWidget);

	RC_RECT parentRect = parentContainerComponent ? parentContainerComponent->GetContainerFreeRect() : parentWidget->GetCachedActualRect();
	RC_SIZE underlayWidgetDirtySize = underlayWidget->GetCachedDirtySize();

	RC_RECT newUnderlayWidgetRect = { 0 };

	const bool bAllowStretch = (alignmentSettings->Stretch != AlignmentSettings::StretchMode::NoStretch);

	switch (alignmentSettings->Horizontal)
	{
		case AlignmentSettings::HorizontalAlignment::NoAlignment:
			{
				if (bAllowStretch || underlayWidgetDirtySize.cx == 0)
				{
					newUnderlayWidgetRect.left = parentRect.left;
					newUnderlayWidgetRect.right = parentRect.right;
				}
				else
				{
					newUnderlayWidgetRect.left = parentRect.left;
					newUnderlayWidgetRect.right = newUnderlayWidgetRect.left + underlayWidgetDirtySize.cx;
				}
			}
			break;
		case AlignmentSettings::HorizontalAlignment::Left:
			{
				newUnderlayWidgetRect.left = parentRect.left;
				newUnderlayWidgetRect.right = newUnderlayWidgetRect.left + underlayWidgetDirtySize.cx;
			}
			break;
		case AlignmentSettings::HorizontalAlignment::Center:
			{
				newUnderlayWidgetRect.left = parentRect.left + (parentRect.right - parentRect.left - underlayWidgetDirtySize.cx) / 2;
				newUnderlayWidgetRect.right = newUnderlayWidgetRect.left + underlayWidgetDirtySize.cx;
			}
			break;
		case AlignmentSettings::HorizontalAlignment::Right:
			{
				newUnderlayWidgetRect.right = parentRect.right;
				newUnderlayWidgetRect.left = newUnderlayWidgetRect.right - underlayWidgetDirtySize.cx;
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
					newUnderlayWidgetRect.top = parentRect.top;
					newUnderlayWidgetRect.bottom = parentRect.bottom;
				}
				else
				{
					newUnderlayWidgetRect.top = parentRect.top;
					newUnderlayWidgetRect.bottom = newUnderlayWidgetRect.top + underlayWidgetDirtySize.cy;
				}
			}
			break;
		case AlignmentSettings::VerticalAlignment::Top:
			{
				newUnderlayWidgetRect.top = parentRect.top;
				newUnderlayWidgetRect.bottom = newUnderlayWidgetRect.top + underlayWidgetDirtySize.cy;
			}
			break;
		case AlignmentSettings::VerticalAlignment::Center:
			{
				newUnderlayWidgetRect.top = parentRect.top + (parentRect.bottom - parentRect.top - underlayWidgetDirtySize.cy) / 2;
				newUnderlayWidgetRect.bottom = newUnderlayWidgetRect.top + underlayWidgetDirtySize.cy;
			}
			break;
		case AlignmentSettings::VerticalAlignment::Bottom:
			{
				newUnderlayWidgetRect.bottom = parentRect.bottom;
				newUnderlayWidgetRect.top = newUnderlayWidgetRect.bottom - underlayWidgetDirtySize.cy;
			}
			break;
		default:
			break;
	}

	underlayWidget->RepositionWidget(newUnderlayWidgetRect);
	if (parentContainerComponent) parentContainerComponent->PlaceInContainer(newUnderlayWidgetRect);
}
