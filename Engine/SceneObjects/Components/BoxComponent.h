#pragma once

#include "CollisionComponent.h"

class BoxComponent : public CollisionComponent
{
	typedef CollisionComponent Inherited;

protected:
	virtual void RegisterCollisionComponent() override;

public:
	BoxComponent();

	/** Extending the ICollider interface. */

	virtual bool Includes(const AABB& other) const override;
	virtual bool Intersects(const AABB& other) const override;

	virtual uint16_t GetCollisionPrimitiveType() const override;

	/**  */

	AABB GetAABB() const;

	void SetBoxExtent(const LV_SIZE& extent) { BoxExtent = extent; }
	const LV_SIZE& GetBoxExtent() const { return BoxExtent; }

protected:
	LV_SIZE BoxExtent = { 0.0f, 0.0f };
};
