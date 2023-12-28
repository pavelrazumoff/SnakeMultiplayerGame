#pragma once

#include "WidgetComponent.h"
#include "Core/RCDataTypes.h"

class WidgetContainerComponent : public IWidgetComponent
{
public:
	void ResetContainerFreeRect(RC_RECT rect);
	void ResetAlignment(bool bVertical);

	void PlaceInContainer(const RC_RECT& occupiedRect);
	RC_RECT GetContainerFreeRect() const { return ContainerFreeRect; }

private:
	RC_RECT ContainerFreeRect = { 0 };
	bool bVerticalAlignment = true;
};
