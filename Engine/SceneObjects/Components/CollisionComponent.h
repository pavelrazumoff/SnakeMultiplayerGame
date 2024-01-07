#pragma once

#include "SceneComponent.h"
#include "Engine/CollisionDetection/Collider.h"

#include "Engine/Events/EventDelegate.h"
#include "Engine/GameObject/GameObjectPtr.h"

class CollisionComponent;
class SceneObject;

DECLARE_EVENT_DELEGATE(CollisionStartedDelegate, CollisionComponent*, SceneObject*, ICollider*);
DECLARE_EVENT_DELEGATE(CollisionEndedDelegate, CollisionComponent*, SceneObject*, ICollider*);

class CollisionComponent : public SceneComponent, public ICollider
{
	typedef SceneComponent Inherited;

public:
	CollisionComponent();
	virtual ~CollisionComponent();

	virtual void BeginPlayComponent() override;

	void HandleCollisionWith(std::vector<ICollider*> Intersections);

	/** Events. */

	CollisionStartedDelegate& OnCollisionStartEvent() { return CollisionStartEvent; }
	CollisionEndedDelegate& OnCollisionEndEvent() { return CollisionEndEvent; }

protected:
	virtual void SetSceneLocation(LV_COORD sceneLocation) override;

private:
	bool WasAlreadyCollidingWith(ICollider* otherCollider);

protected:
	CollisionStartedDelegate CollisionStartEvent;
	CollisionEndedDelegate CollisionEndEvent;

private:
	std::vector<TObjectPtr<CollisionComponent>> PrevIntersections;
};
