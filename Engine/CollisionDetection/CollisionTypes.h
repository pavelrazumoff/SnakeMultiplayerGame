#pragma once

#include <map>

#define LOWORD32(l)           ((uint16_t)(((uint32_t)(l)) & 0xffff))
#define HIWORD32(l)           ((uint16_t)((((uint32_t)(l)) >> 16) & 0xffff))

#define TO_UINT32(low, high) ((uint32_t)high << 16) | (uint32_t)low

enum class CollisionPrimitiveType
{
	Box = 0,
	Circle,

	Custom = 100,
};

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

	bool Includes(const AABB& other) const
	{
		return minX >= other.minX && maxX <= other.maxX &&
			minY >= other.minY && maxY <= other.maxY;
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
	void SetCollisionResponse(CollisionObjectType type, bool bCollidable)
	{
		CollisionResponseMap[type] = bCollidable;
	}

	CollisionObjectType GetCollisionObjectType() const { return CollisionType; }

	bool CanHandleCollisionWithAny() const
	{
		for (auto& it : CollisionResponseMap)
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
		auto it = CollisionResponseMap.find(type);
		if (it != CollisionResponseMap.end())
		{
			return it->second;
		}
		return false;
	}

protected:
	bool bEnableCollision = true;

	CollisionObjectType CollisionType = CollisionObjectType::Static;
	std::map<CollisionObjectType, bool> CollisionResponseMap;
};
