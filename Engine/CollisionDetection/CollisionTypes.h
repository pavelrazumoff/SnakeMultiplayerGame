#pragma once

#include <map>

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

enum class CollisionObjectType
{
	Static,
	Dynamic,
	PlayerPawn,
};

struct CollisionSettings
{
public:
	void EnableCollision(bool bEnable) { bEnableCollision = bEnable; }
	bool IsCollisionEnabled() const { return bEnableCollision; }

	void SetCollisionObjectType(CollisionObjectType type) { CollisionType = type; }
	void SetCollisionWithObjectType(CollisionObjectType type, bool bCollidable)
	{
		CollisionWithTypeMap[type] = bCollidable;
	}

	CollisionObjectType GetCollisionObjectType() const { return CollisionType; }

	bool CanHandleCollisionWithAny() const
	{
		for (auto& it : CollisionWithTypeMap)
		{
			if (it.second)
			{
				return true;
			}
		}
		return false;
	}

	bool CanHandleCollisionWith(CollisionObjectType type) const
	{
		auto it = CollisionWithTypeMap.find(type);
		if (it != CollisionWithTypeMap.end())
		{
			return it->second;
		}
		return false;
	}

protected:
	bool bEnableCollision = true;

	CollisionObjectType CollisionType = CollisionObjectType::Static;
	std::map<CollisionObjectType, bool> CollisionWithTypeMap;
};
