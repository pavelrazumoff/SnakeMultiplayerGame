#pragma once

#include "GameWidget.h"
#include "../Primitives/BrushPrimitive.h"

#include "../Utility/AlignmentSettings.h"
#include "../Utility/LayoutSettings.h"

class PanelWidget : public GameWidget
{
	typedef GameWidget Inheried;

public:
	PanelWidget();
	virtual ~PanelWidget();

	virtual void ReconstructWidget() override;
	virtual void ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount) override;
	virtual void RepositionWidget(const RC_RECT& newRect) override;

	virtual void DrawWidget(RCTexture* RenderTargetTexture) override;

	virtual RC_SIZE CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc) override;

	BrushPrimitive* GetBackgroundBrush() { return BackgroundBrush.Get(); }
	AlignmentSettings& GetAlignment() { return Alignment; }
	LayoutSettings& GetLayout() { return Layout; }

protected:
	virtual GameObject* Clone(GameObject* _owner) const override;

protected:
	TObjectPtr<BrushPrimitive> BackgroundBrush;

	AlignmentSettings Alignment;
	LayoutSettings Layout;
};
