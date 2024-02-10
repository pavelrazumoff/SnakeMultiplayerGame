#pragma once

#include "WidgetComponent.h"

#include "Core/RCDataTypes.h"

class AlignmentSettings : public IWidgetComponent
{
public:
	enum class HorizontalAlignment
	{
		NoAlignment, // Stretch to the whole available width.

		Left,
		Center,
		Right
	};

	enum class VerticalAlignment
	{
		NoAlignment, // Stretch to the whole available height.

		Top,
		Center,
		Bottom
	};

	enum class StretchMode
	{
		NoStretch,
		Fill,
	};

	HorizontalAlignment Horizontal = HorizontalAlignment::Center;
	VerticalAlignment Vertical = VerticalAlignment::Center;
	StretchMode Stretch = StretchMode::NoStretch; // Affects only the size of the widget.
	RC_RECT Padding = RC_RECT(); // Padding from the edges of the parent widget.
};

class GameWidget;

void ApplyAlignmentSettings(GameWidget* parentWidget, GameWidget* underlayWidget);
void ApplyAlignmentSettings(GameWidget* underlayWidget, RC_RECT parentRect);

RC_SIZE GetWidgetOccupiedSize(GameWidget* underlayWidget);
