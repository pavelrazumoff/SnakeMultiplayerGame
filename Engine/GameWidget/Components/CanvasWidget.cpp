#include "CanvasWidget.h"
#include "../Utility/AlignmentSettings.h"

#include "Engine/EngineUtility.h"

CanvasWidget::CanvasWidget()
{
}

CanvasWidget::~CanvasWidget()
{
}

void CanvasWidget::ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount)
{
	engine_assert(underlayWidgetsCount == 1);

	GameWidget* underlayWidget = underlayWidgets[0];
	ApplyAlignmentSettings(this, underlayWidget);
}

void CanvasWidget::DrawWidget(RCTexture* RenderTargetTexture)
{
}

void CanvasWidget::SetWidth(uint32_t width)
{
	CanvasWidth = width;
}

void CanvasWidget::SetHeight(uint32_t height)
{
	CanvasHeight = height;
}

RC_SIZE CanvasWidget::CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc)
{
	RC_SIZE dirtySize = { (RC_UINT)CanvasWidth, (RC_UINT)CanvasHeight };
	return dirtySize;
}
