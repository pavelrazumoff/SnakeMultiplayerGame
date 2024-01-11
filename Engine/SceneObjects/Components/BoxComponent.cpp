#include "BoxComponent.h"

bool BoxComponent::Intersects(const AABB& other) const
{
	return other.Intersects(GetAABB());
}

AABB BoxComponent::GetAABB() const
{
	LV_COORD boxLocation = GetSceneLocation();
	return { boxLocation.x, boxLocation.y, boxLocation.x + BoxExtent.x, boxLocation.y + BoxExtent.y };
}
