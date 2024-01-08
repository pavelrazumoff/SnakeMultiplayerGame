#pragma once

#include "Engine/SceneObjects/Pawn/GamePawn.h"

#include <queue>

class ImageComponent;
class CollisionComponent;
class BoxComponent;
class ICollider;

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
	void IncreaseBody();

	/** Collision Events. */

	void HandleHeadCollisionStartEvent(CollisionComponent* InstigatorComp, SceneObject* OtherObject, ICollider* OtherCollider);
	void HandleHeadCollisionEndEvent(CollisionComponent* InstigatorComp, SceneObject* OtherObject, ICollider* OtherCollider);

protected:
	TObjectPtr<ImageComponent> HeadImageComponent;
	TObjectPtr<ImageComponent> BodyImageComponent;

	TObjectPtr<BoxComponent> HeadBoxComponent;

	std::vector<LV_COORD> bodyPoints;

	/** Input Actions */

	InputAction MoveUpDownAction;
	InputAction MoveLeftRightAction;

private:
	LV_VECTOR MovingDirection;
};