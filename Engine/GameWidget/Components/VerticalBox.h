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

protected:
	AlignmentSettings Alignment;
	WidgetContainerComponent ContainerComponent;
};
