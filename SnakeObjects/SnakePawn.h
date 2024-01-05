#pragma once

#include "Engine/SceneObjects/Pawn/GamePawn.h"
#include "Engine/SceneObjects/Components/ImageComponent.h"

#include <queue>

class SnakePawn : public GamePawn
{
	typedef GamePawn Inherited;

public:
	SnakePawn();
	~SnakePawn();

	virtual void BeginPlay() override;
	virtual void Update(float DeltaTime) override;

protected:
	/** Input Action bindings. */

	void MoveUpDown_Triggered(InputValue actionValue);
	void MoveLeftRight_Triggered(InputValue actionValue);

	/** Snake movement logic. */

	void UpdateBodyMovement(float DeltaTime);
	void HandleDirectionChange();

protected:
	ImageComponent HeadImageComponent;
	ImageComponent TailImageComponent;

	//std::queue<std::shared_ptr<ImageComponent>> BodyImageComponents;
	std::queue<LV_COORD> turnPoints;

	/** Input Actions */

	InputAction MoveUpDownAction;
	InputAction MoveLeftRightAction;

private:
	LV_VECTOR MovingDirection;
	LV_COORD TailTargetLocation;
};