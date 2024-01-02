#include "PanelWidget.h"

#include "Engine/GameObject/GameObjectUtility.h"

PanelWidget::PanelWidget()
{
	BackgroundBrush = CreateNewObject<BrushPrimitive>(this);

	AddWidgetComponent(&Alignment);
	AddWidgetComponent(&Layout);
}

PanelWidget::~PanelWidget()
{
}

void PanelWidget::ReconstructWidget()
{
	Inheried::ReconstructWidget();

	
}

void PanelWidget::ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount)
{
	// TODO: Take possible layouting and alignment into account.
	#if 0
	RC_RECT widgetRect;
	RC_SIZE widgetSize;
	const RC_SIZE dirtySize = GetCachedDirtySize();

	const RC_SIZE parentSize = { GetTexWidth(parentRect), GetTexHeight(parentRect) };

	widgetSize.cx = IsSizeXNeedsToRecalc() ? parentSize.cx : dirtySize.cx;
	widgetSize.cy = IsSizeYNeedToRecalc() ? parentSize.cy : dirtySize.cy;

	if (widgetSize.cx < dirtySize.cx) widgetSize.cx = dirtySize.cx;
	if (widgetSize.cy < dirtySize.cy) widgetSize.cy = dirtySize.cy;
	
	// TODO: Reposition based on alignment settings.

	// TODO: We need the actual X, Y coordinates where to draw this widget.
	widgetRect = { parentRect.left, parentRect.top, widgetSize.cx, widgetSize.cy };

	BackgroundBrush->UpdateDrawRect(widgetRect);
	#endif
	//return widgetRect;

	// We should already have the rect of this widget now.
	// Based on that we can calculate the size of each underlaying widget based on ours layouting settings and theirs dirty sizes.


}

void PanelWidget::RepositionWidget(const RC_RECT& newRect)
{
	Inheried::RepositionWidget(newRect);

	BackgroundBrush->UpdateDrawRect(GetCachedActualRect());
	BackgroundBrush->Construct();
}

void PanelWidget::DrawWidget(RCTexture* RenderTargetTexture)
{
	BackgroundBrush->Draw(RenderTargetTexture);
}

RC_SIZE PanelWidget::CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc)
{
	/*
		TODO: Take children if any into an account.
	*/

	RC_SIZE dirtySize = { 0, 0 };
	_bSizeXNeedsToRecalc = _bSizeYNeedToRecalc = false;

	if (Layout.DoesOverrideWidth()) dirtySize.cx = Layout.DimensionsOverride.cx;
	else _bSizeXNeedsToRecalc = true;
	if (Layout.DoesOverrideHeight()) dirtySize.cy = Layout.DimensionsOverride.cy;
	else _bSizeYNeedToRecalc = true;

	return dirtySize;
}

GameObject* PanelWidget::Clone(GameObject* _owner) const
{
	PanelWidget* newWidget = CreateNewObject<PanelWidget>(_owner);
	newWidget->BackgroundBrush.Get()->SetBrushStyle(BackgroundBrush.Get()->GetBrushStyle());

	newWidget->Alignment = this->Alignment;
	newWidget->Layout = this->Layout;

	return newWidget;
}

