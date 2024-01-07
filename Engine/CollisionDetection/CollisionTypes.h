#pragma once

struct AABB
{
public:
	AABB() : minX(0), minY(0), maxX(0), maxY(0) {}
	AABB(float minX, float minY, float maxX, float maxY) 
		: minX(minX), minY(minY), maxX(maxX), maxY(maxY) {}

	bool Intersects(const AABB& other) const
	{
		return minX <= other.maxX && maxX >= other.minX &&
			minY <= other.maxY && maxY >= other.minY;
	}

public:
	float minX, minY, maxX, maxY;
};
