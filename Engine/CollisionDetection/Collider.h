#pragma once

#include "CollisionTypes.h"

class ICollider
{
public:
	virtual bool Intersects(const AABB& other) const = 0;
	virtual AABB GetAABB() const = 0;
};
