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
	engine_assert(underlayWidgetsCount <= 1);
	
	for (size_t i = 0; i < underlayWidgetsCount; ++i)
	{
		GameWidget* underlayWidget = underlayWidgets[i];
		ApplyAlignmentSettings(this, underlayWidget);
	}
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

