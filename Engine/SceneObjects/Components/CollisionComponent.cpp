#include "CollisionComponent.h"

#include "Engine/CollisionDetection/CollisionManager.h"
#include "Engine/SceneObjects/SceneObject.h"

#include "Engine/Other/ProfilerManager.h"

std::unordered_set<std::string> CollisionComponent::registeredTypes;

CollisionComponent::CollisionComponent()
{
}

CollisionComponent::~CollisionComponent()
{
	CollisionManager::GetInstance().StopTrackingCollisionComponent(this);
}

void CollisionComponent::BeginPlayComponent()
{
	const std::type_info& typeInfo = typeid(*this);
	if (registeredTypes.find(typeInfo.name()) == registeredTypes.end())
	{
		RegisterCollisionComponent();
		registeredTypes.insert(typeInfo.name());
	}

	CollisionManager::GetInstance().StartTrackingCollisionComponent(this);
}

bool CollisionComponent::Intersects(const ICollider* other) const
{
	return CollisionManager::GetInstance().CheckForIntersection(this, other);
}

bool CollisionComponent::CanCollide() const
{
	return Settings.CanHandleCollisionWithAny();
}

bool CollisionComponent::CanCollideWith(const ICollider* other) const
{
	if (auto otherComp = dynamic_cast<const CollisionComponent*>(other))
	{
		return Settings.CanHandleCollisionWith(otherComp->Settings.GetCollisionObjectType());
	}

	return false;
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
	if (GetSceneLocation() == sceneLocation) return;

	LV_COORD prevSceneLocation = GetSceneLocation();
	Inherited::SetSceneLocation(sceneLocation);

	if (ProfilerManager::GetInstance().IsEngineFeatureEnabled(ProfilerEngineFeature::CollisionDetection))
	{
		// For collision updates we should really only check, if the actual visible location has changed or not (we moved to another pixel on screen).
		sceneLocation.Round();
		prevSceneLocation.Round();

		if (sceneLocation != prevSceneLocation)
			CollisionManager::GetInstance().UpdateCollisionComponent(this);
	}
}

bool CollisionComponent::WasAlreadyCollidingWith(ICollider* otherCollider)
{
	auto it = std::find_if(PrevIntersections.begin(), PrevIntersections.end(), [otherCollider](const auto& Other) -> bool {
		return otherCollider == Other.Get();
		});
		
	return it != PrevIntersections.end();
}
