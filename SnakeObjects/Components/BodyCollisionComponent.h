#pragma once

#include "Engine/SceneObjects/Components/CollisionComponent.h"

class BodyCollisionComponent : public CollisionComponent
{
	typedef CollisionComponent Inherited;

protected:
	virtual void RegisterCollisionComponent() override;

public:
	BodyCollisionComponent();

	/** Extending the ICollider interface. */

	virtual bool Includes(const AABB& other) const override;
	virtual bool Intersects(const AABB& other) const override;
	virtual uint16_t GetCollisionPrimitiveType() const override;

	/** */

	void AddBodyAABB(const AABB& bodyAABB, int insertIndex = -1);
	void RemoveBodyAABB(size_t bodyPartIndex);

	void ClearBodyAABBs();

	size_t GetBodyAABBCount() const { return BodyAABBs.size(); }

protected:
	AABB ConstructFullBodyAABB() const;

protected:
	std::vector<AABB> BodyAABBs;
};
