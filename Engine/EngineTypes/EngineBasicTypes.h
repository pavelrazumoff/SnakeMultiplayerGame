#pragma once

#include "stdint.h"
#include <cmath>
#include <string>

/** Base Engine Type for use to create specific objects by factories. */

class EngineGenericType
{
public:
	virtual ~EngineGenericType() {}

	virtual [[nodiscard]] EngineGenericType* CloneGeneric() const = 0;
	virtual std::string GetGenericTypeName() const = 0;
};

/** Types specifically for objects placed on Game Levels. */

struct LV_VECTOR
{
public:
	LV_VECTOR(float _x = 0, float _y = 0)
		: x(_x), y(_y)
	{}

	void Round()
	{
		x = round(x);
		y = round(y);
	}

	bool Compare(const LV_VECTOR& other, float tolerance = 0.001f) const
	{
		return fabs(x - other.x) < tolerance && fabs(y - other.y) < tolerance;
	}

	bool Compare(const LV_VECTOR& other, const LV_VECTOR& tolerance) const
	{
		return fabs(x - other.x) < tolerance.x && fabs(y - other.y) < tolerance.y;
	}

	LV_VECTOR& Normalize()
	{
		float length = sqrt(x * x + y * y);
		if (length > 0.0f)
		{
			x /= length;
			y /= length;
		}
		return *this;
	}

	bool IsZero() const { return x == 0.0f && y == 0.0f; }

	LV_VECTOR operator+(const LV_VECTOR& other) const
	{
		LV_VECTOR vec = *this;
		vec.x += other.x;
		vec.y += other.y;
		return vec;
	}

	LV_VECTOR operator-(const LV_VECTOR& other) const
	{
		LV_VECTOR vec = *this;
		vec.x -= other.x;
		vec.y -= other.y;
		return vec;
	}

	LV_VECTOR operator+=(const LV_VECTOR& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	LV_VECTOR operator*(float scale) const
	{
		LV_VECTOR vec = *this;
		vec.x *= scale;
		vec.y *= scale;
		return vec;
	}

	LV_VECTOR operator*(const LV_VECTOR& other) const
	{
		LV_VECTOR vec = *this;
		vec.x *= other.x;
		vec.y *= other.y;
		return vec;
	}

	LV_VECTOR operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	LV_VECTOR operator/(float scale) const
	{
		LV_VECTOR vec = *this;
		vec.x /= scale;
		vec.y /= scale;
		return vec;
	}

	bool operator==(const LV_VECTOR& other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const LV_VECTOR& other) const
	{
		return x != other.x || y != other.y;
	}

	static inline LV_VECTOR Zero() { return LV_VECTOR(0, 0); }

	std::string ToString() const
	{
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}

public:
	float x = 0.0f;
	float y = 0.0f;
};

typedef LV_VECTOR LV_SIZE;
typedef LV_VECTOR LV_COORD;

struct LV_RECT
{
public:
	LV_RECT() {}
	LV_RECT(float _left, float _top, float _right, float _bottom)
		: left(_left), top(_top), right(_right), bottom(_bottom)
	{}

public:
	float left = 0.0f;
	float top = 0.0f;
	float right = 0.0f;
	float bottom = 0.0f;
};
