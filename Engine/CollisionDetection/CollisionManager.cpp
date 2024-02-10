#include "CollisionManager.h"

#include "Engine/SceneObjects/Components/BoxComponent.h"
#include "QuadTree.h"

#include "Core/RenderConsoleLibrary.h"

#include "Engine/Other/ProfilerManager.h"

CollisionManager::~CollisionManager()
{
}

CollisionManager& CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return instance;
}

void CollisionManager::Initialize()
{
	//ReconstructQuadTree();
}

void CollisionManager::Reconstruct()
{
	ReconstructQuadTree();

	for (auto& trackCollisionComp : TrackedCollisionComponents)
		CollisionQuadTree->Insert(trackCollisionComp.Get());
}

void CollisionManager::ReconstructQuadTree()
{
	RC_SIZE screenDim = RenderConsoleLibrary::GetConsoleDimensions();
	AABB treeBoundary(0.0f, 0.0f, (float)screenDim.cx, (float)screenDim.cy);

	CollisionQuadTree = std::make_shared<QuadTree>(treeBoundary, 4);
}

void CollisionManager::ResetAll()
{
	ReconstructQuadTree();
	TrackedCollisionComponents.clear();
}

void CollisionManager::UpdateTracking()
{
	if (!ProfilerManager::GetInstance().IsEngineFeatureEnabled(ProfilerEngineFeature::CollisionDetection))
		return;

	for (auto& trackCollisionComp : TrackedCollisionComponents)
	{
		if (!trackCollisionComp->CanCollide()) continue;

		std::vector<ICollider*> intersections;
		CollisionQuadTree->QueryRangeFor(trackCollisionComp.Get(), intersections);

		trackCollisionComp->HandleCollisionWith(intersections);
	}
}

void CollisionManager::StartTrackingCollisionComponent(CollisionComponent* pCollisionComponent)
{
	if (!IsTrackingCollisionComponent(pCollisionComponent))
	{
		TrackedCollisionComponents.push_back(pCollisionComponent);
		CollisionQuadTree->Insert(pCollisionComponent);
	}
}

void CollisionManager::StopTrackingCollisionComponent(CollisionComponent* pCollisionComponent)
{
	auto it = FindTrackingCollisionComponent(pCollisionComponent);
	if (it != TrackedCollisionComponents.end())
	{
		TrackedCollisionComponents.erase(it);
		CollisionQuadTree->Remove(pCollisionComponent);
	}
}

void CollisionManager::UpdateCollisionComponent(CollisionComponent* pCollisionComponent)
{
	if (IsTrackingCollisionComponent(pCollisionComponent))
	{
		CollisionQuadTree->Remove(pCollisionComponent);
		CollisionQuadTree->Insert(pCollisionComponent);
	}
	//else DebugEngineTrap();
}

void CollisionManager::AddCollisionChecker(uint32_t collisionPairPrimitiveMask, CollisionChecker checker)
{
	if (CollisionCheckers.find(collisionPairPrimitiveMask) != CollisionCheckers.cend()) return;

	CollisionCheckers[collisionPairPrimitiveMask] = checker;
}

bool CollisionManager::CheckForIntersection(const ICollider* first, const ICollider* second)
{
	uint32_t primitiveMask = TO_UINT32(first->GetCollisionPrimitiveType(), second->GetCollisionPrimitiveType());
	auto it = CollisionCheckers.find(primitiveMask);
	if (it != CollisionCheckers.end())
		return it->second(first, second);
	
	if (first->GetCollisionPrimitiveType() == second->GetCollisionPrimitiveType()) return false;

	// Just swap the mask and try the second combination.
	primitiveMask = TO_UINT32(second->GetCollisionPrimitiveType(), first->GetCollisionPrimitiveType());
	it = CollisionCheckers.find(primitiveMask);
	if (it != CollisionCheckers.end())
		return it->second(second, first);

	return false;
}

bool CollisionManager::IsTrackingCollisionComponent(CollisionComponent* pCollisionComponent)
{
	auto it = FindTrackingCollisionComponent(pCollisionComponent);
	return it != TrackedCollisionComponents.end();
}

std::vector<TObjectPtr<CollisionComponent>>::iterator CollisionManager::FindTrackingCollisionComponent(CollisionComponent* pCollisionComponent)
{
	return std::find_if(TrackedCollisionComponents.begin(), TrackedCollisionComponents.end(),
		[pCollisionComponent](const auto& collisionComp) -> bool {
			return pCollisionComponent == collisionComp.Get();
		});
}
