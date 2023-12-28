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

	virtual void DrawWidget() override;

	virtual RC_SIZE CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc) override;

	BrushPrimitive* GetBackgroundBrush() { return BackgroundBrush.Get(); }
	AlignmentSettings& GetAlignment() { return Alignment; }
	LayoutSettings& GetLayout() { return Layout; }

protected:
	virtual GameObject* Clone() const override;

protected:
	TObjectPtr<BrushPrimitive> BackgroundBrush;

	// Put this into separate components, that implement common interface.
	// Then we will have utility function that can search for a specific component in a widget by the template type.
	AlignmentSettings Alignment;
	LayoutSettings Layout;
};
