#include "GameWidget.h"

GameWidget::GameWidget()
{
}

GameWidget::~GameWidget()
{
}

void GameWidget::ReconstructWidget()
{
	// Try adjust size based on its content.
	// Mark this size as dirty (or temporal) if we're not sure about it (eg. there're some childrens with floating size and some with fixed).
	// Cache this size, not make as permanent, we will just use it for layouting later.

	CachedDirtySize = CalcDirtySize(bSizeXNeedsToRecalc, bSizeYNeedToRecalc);
}

void GameWidget::ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount)
{
	// Child class should override this and use cached size to return the actual size of this widget.
	//return { 0, 0, CachedDirtySize.cx, CachedDirtySize.cy };
}

void GameWidget::RepositionWidget(const RC_RECT& newRect)
{
	CachedActualRect = newRect;
}

void GameWidget::SetVisibility(bool newVisibility)
{
	bIsVisible = newVisibility;
}

bool GameWidget::IsVisible() const
{
	return bIsVisible;
}

RC_SIZE GameWidget::CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc)
{
	/*
		Dirty size is like minimal size of this widget.
	*/

	_bSizeXNeedsToRecalc = false; _bSizeYNeedToRecalc = false;
	return RC_SIZE();
}

void GameWidget::AddWidgetComponent(IWidgetComponent* newComponent)
{
	WidgetComponents.push_back(newComponent);
}
