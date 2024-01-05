#pragma once

#include "Engine/SceneObjects/Pawn/GamePawn.h"

#include "Engine/SceneObjects/Components/ImageComponent.h"

class SnakePawn : public GamePawn
{
public:
	SnakePawn();
	~SnakePawn();

protected:
	/** Input Action bindings. */

	void MoveUpDown_Triggered(InputValue actionValue);
	void MoveLeftRight_Triggered(InputValue actionValue);

protected:
	ImageComponent HeadImageComponent;

	/** Input Actions */

	InputAction MoveUpDownAction;
	InputAction MoveLeftRightAction;
};