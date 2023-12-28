#pragma once

#include "Core/RCDataTypes.h"
#include "WidgetComponent.h"

class LayoutSettings : public IWidgetComponent
{
public:
	bool DoesOverrideWidth() const { return DimensionsOverride.cx != 0; }
	bool DoesOverrideHeight() const { return DimensionsOverride.cy != 0; }

public:
	RC_SIZE MinDimensions = { 0, 0 };
	RC_SIZE MaxDimensions = { 0, 0 };

	RC_SIZE DimensionsOverride = { 0, 0 };
};
