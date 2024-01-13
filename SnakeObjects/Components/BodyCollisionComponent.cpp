#include "BodyCollisionComponent.h"

#include "Engine/SceneObjects/Components/BoxComponent.h"
#include "Engine/CollisionDetection/CollisionManager.h"

#include "SnakeUtility.h"

BodyCollisionComponent::BodyCollisionComponent()
{
}

void BodyCollisionComponent::RegisterCollisionComponent()
{
	uint32_t body2boxMask = TO_UINT32(SnakeUtility::BodyCollisionPrimitiveType, static_cast<uint16_t>(CollisionPrimitiveType::Box));
	CollisionManager::GetInstance().AddCollisionChecker(body2boxMask, [](const ICollider* first, const ICollider* second) -> bool {

		const BodyCollisionComponent* pBody = dynamic_cast<const BodyCollisionComponent*>(first);
		const BoxComponent* pBox = dynamic_cast<const BoxComponent*>(second);

		for (const AABB& bodyAABB : pBody->BodyAABBs)
			if (bodyAABB.Intersects(pBox->GetAABB()))
			{
				return true;
			}

		return false;
	});
}

bool BodyCollisionComponent::Includes(const AABB& other) const
{
	return ConstructFullBodyAABB().Includes(other);
}

bool BodyCollisionComponent::Intersects(const AABB& other) const
{
	return ConstructFullBodyAABB().Intersects(other);
}

uint16_t BodyCollisionComponent::GetCollisionPrimitiveType() const
{
	return SnakeUtility::BodyCollisionPrimitiveType;
}

void BodyCollisionComponent::AddBodyAABB(const AABB& bodyAABB, int insertIndex)
{
	if (insertIndex < 0)
		BodyAABBs.push_back(bodyAABB);
	else
		BodyAABBs.insert(BodyAABBs.begin() + insertIndex, bodyAABB);
}

void BodyCollisionComponent::RemoveBodyAABB(size_t bodyPartIndex)
{
	BodyAABBs.erase(BodyAABBs.begin() + bodyPartIndex);
}

void BodyCollisionComponent::ClearBodyAABBs()
{
	BodyAABBs.clear();
}

AABB BodyCollisionComponent::ConstructFullBodyAABB() const
{
	AABB fullBodyAABB = { -1.0f, -1.0f, -1.0f, -1.0f };
	for (const AABB& bodyAABB : BodyAABBs)
	{
		if (fullBodyAABB.minX == -1.0f)
		{
			fullBodyAABB = bodyAABB;
		}
		else
		{
			fullBodyAABB.minX = std::min(fullBodyAABB.minX, bodyAABB.minX);
			fullBodyAABB.minY = std::min(fullBodyAABB.minY, bodyAABB.minY);
			fullBodyAABB.maxX = std::max(fullBodyAABB.maxX, bodyAABB.maxX);
			fullBodyAABB.maxY = std::max(fullBodyAABB.maxY, bodyAABB.maxY);
		}
	}

	return fullBodyAABB;
}
