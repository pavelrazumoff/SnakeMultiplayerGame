#pragma once

#include "Engine/SceneObjects/Pawn/GamePawn.h"

#include <queue>

class ImageComponent;
class CollisionComponent;
class BoxComponent;
class BodyCollisionComponent;
class ICollider;

REGISTER_CLASS(SnakePawn)
class SnakePawn : public GamePawn
{
	GAMEOBJECT_BODY(SnakePawn, GamePawn)

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

	void SyncBodyMovementWithCollision();

	/** Collision Events. */

	void HandleHeadCollisionStartEvent(CollisionComponent* InstigatorComp, SceneObject* OtherObject, ICollider* OtherCollider);
	void HandleHeadCollisionEndEvent(CollisionComponent* InstigatorComp, SceneObject* OtherObject, ICollider* OtherCollider);

	void HandleBodyCollisionStartEvent(CollisionComponent* InstigatorComp, SceneObject* OtherObject, ICollider* OtherCollider);

	/** Player Stat. */

	void UpdatePlayerStat();

protected:

protected:
	TObjectPtr<ImageComponent> HeadImageComponent;
	TObjectPtr<ImageComponent> BodyImageComponent;

	TObjectPtr<BoxComponent> HeadBoxComponent;
	TObjectPtr<BodyCollisionComponent> TailCollisionComponent;

	std::vector<LV_COORD> bodyPoints;

	/** Input Actions */

	InputAction MoveUpDownAction;
	InputAction MoveLeftRightAction;

private:
	LV_VECTOR MovingDirection;
};