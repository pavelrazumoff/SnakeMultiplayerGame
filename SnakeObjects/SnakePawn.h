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

	virtual void Render(RCTexture* RenderTargetTexture) override;

protected:
	/** Input Action bindings. */

	void MoveUpDown_Triggered(InputValue actionValue);
	void MoveLeftRight_Triggered(InputValue actionValue);

	/** Snake movement logic. */

	void UpdateBodyMovement(float DeltaTime);

protected:
	ImageComponent HeadImageComponent;
	ImageComponent BodyImageComponent;

	std::vector<LV_COORD> bodyPoints;

	/** Input Actions */

	InputAction MoveUpDownAction;
	InputAction MoveLeftRightAction;

private:
	LV_VECTOR MovingDirection;

	float SpawnNewBodyPartTimer = 0.0f;
};