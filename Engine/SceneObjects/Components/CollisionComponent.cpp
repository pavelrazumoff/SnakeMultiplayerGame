#include "CollisionComponent.h"

#include "Engine/CollisionDetection/CollisionManager.h"
#include "Engine/SceneObjects/SceneObject.h"

CollisionComponent::CollisionComponent()
{
}

CollisionComponent::~CollisionComponent()
{
	CollisionManager::GetInstance().StopTrackingCollisionComponent(this);
}

void CollisionComponent::BeginPlayComponent()
{
	CollisionManager::GetInstance().StartTrackingCollisionComponent(this);
}

void CollisionComponent::HandleCollisionWith(std::vector<ICollider*> Intersections)
{
	for (auto& Intersection : Intersections)
	{
		if (WasAlreadyCollidingWith(Intersection)) continue;

		ObjectComponent* pIntersectedComponent = dynamic_cast<ObjectComponent*>(Intersection);
		if (!pIntersectedComponent) { DebugEngineTrap(); continue; }

		// 1. The component that collided.
		// 2. The object with which the component collided.
		// 3. The other collider component of that object with which ours component collided.
		CollisionStartEvent.Trigger(this, dynamic_cast<SceneObject*>(pIntersectedComponent->GetOwner()), Intersection);
	}

	for (auto& prevIntersection : PrevIntersections)
	{
		if (!prevIntersection.IsValid()) continue;

		auto it = std::find(Intersections.begin(), Intersections.end(), prevIntersection.Get());
		if (it == Intersections.end())
		{
			CollisionEndEvent.Trigger(this, dynamic_cast<SceneObject*>(prevIntersection->GetOwner()), prevIntersection.Get());
		}
	}

	PrevIntersections.clear();
	for (auto& Intersection : Intersections)
		if (auto _intersection = dynamic_cast<CollisionComponent*>(Intersection))
			PrevIntersections.push_back(_intersection);
}

void CollisionComponent::SetSceneLocation(LV_COORD sceneLocation)
{
	Inherited::SetSceneLocation(sceneLocation);

	CollisionManager::GetInstance().UpdateCollisionComponent(this);
}

bool CollisionComponent::WasAlreadyCollidingWith(ICollider* otherCollider)
{
	auto it = std::find_if(PrevIntersections.begin(), PrevIntersections.end(), [otherCollider](const auto& Other) -> bool {
		return otherCollider == Other.Get();
		});
		
	return it != PrevIntersections.end();
}
