#pragma once

#include "CollisionComponent.h"

class BoxComponent : public CollisionComponent
{
	typedef CollisionComponent Inherited;

public:
	/** Extending the ICollider interface. */

	virtual bool Intersects(const AABB& other) const override;
	virtual AABB GetAABB() const override;

	/**  */

	void SetBoxExtent(const LV_SIZE& extent) { BoxExtent = extent; }
	const LV_SIZE& GetBoxExtent() const { return BoxExtent; }

protected:
	LV_SIZE BoxExtent = { 0.0f, 0.0f };
};
