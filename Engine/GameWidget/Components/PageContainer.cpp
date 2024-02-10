#include "PageContainer.h"

#include "Engine/GameWidget/GameWidgetManager.h"

PageContainerWidget::PageContainerWidget()
{
	AddWidgetComponent(&Alignment);
	AddWidgetComponent(&Layout);
}

PageContainerWidget::~PageContainerWidget()
{
}

void PageContainerWidget::ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount)
{
	if (!underlayWidgetsCount) return;

	uint32_t showPageIndex = (CurrentPageIndex >= 0) ? CurrentPageIndex : 0;

	RC_RECT actualRect = GetCachedActualRect();
	for (size_t i = 0; i < underlayWidgetsCount; ++i)
	{
		if (GameWidget* underlayWidget = underlayWidgets[i])
		{
			underlayWidget->SetVisibility(i == showPageIndex ? true : false);

			if (underlayWidget->IsVisible())
				underlayWidget->RepositionWidget(actualRect);
		}
	}
}

void PageContainerWidget::SetCurrentPageIndex(int32_t newPageIndex)
{
	CurrentPageIndex = newPageIndex;

	GameWidgetManager::GetInstance().RequestWidgetReconstruction(this);
}
