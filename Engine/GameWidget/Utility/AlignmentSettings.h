#pragma once

#include "WidgetComponent.h"

class AlignmentSettings : public IWidgetComponent
{
public:
	enum class HorizontalAlignment
	{
		NoAlignment, // Left side.

		Left,
		Center,
		Right
	};

	enum class VerticalAlignment
	{
		NoAlignment, // Top side.

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
};

class GameWidget;

void ApplyAlignmentSettings(GameWidget* parentWidget, GameWidget* underlayWidget);
