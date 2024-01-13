#pragma once

#include "Engine/GameObject/GameObjectPtr.h"

#include <vector>
#include <map>

#include <memory>
#include <functional>

class ICollider;
class CollisionComponent;
class QuadTree;

class CollisionManager
{
protected:
	using CollisionChecker = std::function<bool(const ICollider*, const ICollider*)>;

public:
	CollisionManager();
	~CollisionManager();

	static CollisionManager& GetInstance();

	void Initialize();
	void Reconstruct();

	void UpdateTracking();

	void StartTrackingCollisionComponent(CollisionComponent* pCollisionComponent);
	void StopTrackingCollisionComponent(CollisionComponent* pCollisionComponent);

	void UpdateCollisionComponent(CollisionComponent* pCollisionComponent);

	void AddCollisionChecker(uint32_t collisionPairPrimitiveMask, CollisionChecker checker);
	bool CheckForIntersection(const ICollider* first, const ICollider* second);

protected:
	void ReconstructQuadTree();

	bool IsTrackingCollisionComponent(CollisionComponent* pCollisionComponent);
	std::vector<TObjectPtr<CollisionComponent>>::iterator FindTrackingCollisionComponent(CollisionComponent* pCollisionComponent);

protected:
	std::vector<TObjectPtr<CollisionComponent>> TrackedCollisionComponents;
	std::shared_ptr<QuadTree> CollisionQuadTree;

	std::map<uint32_t, CollisionChecker> CollisionCheckers;
};
