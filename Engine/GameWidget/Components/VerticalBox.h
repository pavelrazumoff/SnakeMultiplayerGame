#pragma once

#include "GameWidget.h"
#include "../Utility/WidgetContainerComponent.h"
#include "../Utility/AlignmentSettings.h"

class VerticalBox : public GameWidget
{
	typedef GameWidget Inherited;

public:
	VerticalBox();
	~VerticalBox();

	virtual void ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount) override;
	virtual RC_SIZE CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc) override;

	// TODO: Make common interface for all containers.
	void ClearChildren();

	AlignmentSettings& GetAlignment() { return Alignment; }

protected:
	AlignmentSettings Alignment;
	WidgetContainerComponent ContainerComponent;
};
