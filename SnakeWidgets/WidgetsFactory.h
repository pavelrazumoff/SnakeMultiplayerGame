#pragma once

#include "Core/RCDataTypes.h"
#include <string>

class GameWidget;
class UserWidget;
class Button;

class WidgetsFactory
{
public:
	// TODO: Calculate the size of the button based on the caption.
	static Button* CreateMenuButton(const std::string& caption, TEX_SIZE buttonSize, UserWidget* Owner, GameWidget* ParentWidget);
};
