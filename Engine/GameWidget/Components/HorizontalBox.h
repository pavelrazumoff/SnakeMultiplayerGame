#pragma once

#include "GameWidget.h"
#include "../Utility/WidgetContainerComponent.h"
#include "../Utility/AlignmentSettings.h"

class HorizontalBox : public GameWidget
{
	typedef GameWidget Inherited;

public:
	HorizontalBox();
	~HorizontalBox();

	virtual void ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount) override;
	virtual RC_SIZE CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc) override;

	AlignmentSettings& GetAlignment() { return Alignment; }

	// TODO: Make common interface for all containers.
	void ClearChildren();

protected:
	AlignmentSettings Alignment;
	WidgetContainerComponent ContainerComponent;
};
