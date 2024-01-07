#pragma once

#include "Engine/GameObject/GameObjectPtr.h"

#include <vector>
#include <memory>

class CollisionComponent;
class QuadTree;

class CollisionManager
{
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

protected:
	void ReconstructQuadTree();

	bool IsTrackingCollisionComponent(CollisionComponent* pCollisionComponent);
	std::vector<TObjectPtr<CollisionComponent>>::iterator FindTrackingCollisionComponent(CollisionComponent* pCollisionComponent);

protected:
	std::vector<TObjectPtr<CollisionComponent>> TrackedCollisionComponents;
	std::shared_ptr<QuadTree> CollisionQuadTree;
};
