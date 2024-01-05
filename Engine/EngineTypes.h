#pragma once

#include "stdint.h"
#include <cmath>

/** Types specifically for objects placed on Game Levels. */

struct LV_VECTOR
{
public:
	LV_VECTOR(float _x = 0, float _y = 0)
		: x(_x), y(_y)
	{}

	void Align()
	{
		x = round(x);
		y = round(y);
	}

	bool Compare(const LV_VECTOR& other, float tolerance = 0.001f) const
	{
		return fabs(x - other.x) < tolerance && fabs(y - other.y) < tolerance;
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

public:
	float x = 0.0f;
	float y = 0.0f;
};

typedef LV_VECTOR LV_SIZE;
typedef LV_VECTOR LV_COORD;