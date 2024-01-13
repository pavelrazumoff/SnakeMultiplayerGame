#include "BoxComponent.h"

#include "Engine/CollisionDetection/CollisionManager.h"

BoxComponent::BoxComponent()
{
}

void BoxComponent::RegisterCollisionComponent()
{
	uint32_t box2boxMask = TO_UINT32(static_cast<uint16_t>(CollisionPrimitiveType::Box), static_cast<uint16_t>(CollisionPrimitiveType::Box));
	CollisionManager::GetInstance().AddCollisionChecker(box2boxMask, [](const ICollider* first, const ICollider* second) -> bool {

		const BoxComponent* pFirst = dynamic_cast<const BoxComponent*>(first);
		const BoxComponent* pSecond = dynamic_cast<const BoxComponent*>(second);

		return pFirst->GetAABB().Intersects(pSecond->GetAABB());
	});
}

bool BoxComponent::Includes(const AABB& other) const
{
	return GetAABB().Includes(other);
}

bool BoxComponent::Intersects(const AABB& other) const
{
	return GetAABB().Intersects(other);
}

AABB BoxComponent::GetAABB() const
{
	LV_COORD boxLocation = GetSceneLocation();
	boxLocation.Round();

	return { boxLocation.x, boxLocation.y, boxLocation.x + BoxExtent.x - 1.0f, boxLocation.y + BoxExtent.y - 1.0f };
}

uint16_t BoxComponent::GetCollisionPrimitiveType() const
{
	return static_cast<uint16_t>(CollisionPrimitiveType::Box);
}
