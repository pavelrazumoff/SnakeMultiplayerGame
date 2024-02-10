#pragma once

#include "GameWidget.h"

#include "../Utility/AlignmentSettings.h"
#include "../Utility/LayoutSettings.h"

class PageContainerWidget : public GameWidget
{
public:
	PageContainerWidget();
	virtual ~PageContainerWidget();

	virtual void ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount) override;

	void SetCurrentPageIndex(int32_t newPageIndex);

	AlignmentSettings& GetAlignment() { return Alignment; }
	LayoutSettings& GetLayout() { return Layout; }

protected:
	int32_t CurrentPageIndex = -1;

	AlignmentSettings Alignment;
	LayoutSettings Layout;
};
