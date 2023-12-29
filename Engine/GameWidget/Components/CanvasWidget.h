#pragma once

#include "GameWidget.h"

class CanvasWidget : public GameWidget
{
public:
	CanvasWidget();
	virtual ~CanvasWidget();

	virtual void ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount) override;

	virtual void DrawWidget(RCTexture* RenderTargetTexture) override;

	void SetWidth(uint32_t width);
	void SetHeight(uint32_t height);

	uint32_t GetWidth() const { return CanvasWidth; }
	uint32_t GetHeight() const { return CanvasHeight; }

protected:
	virtual RC_SIZE CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc) override;

protected:
	uint32_t CanvasWidth = 0;
	uint32_t CanvasHeight = 0;
};
