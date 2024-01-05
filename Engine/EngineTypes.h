#pragma once

#include "stdint.h"

/** Types specifically for objects placed on Game Levels. */

struct LV_VECTOR
{
public:
	LV_VECTOR operator+(const LV_VECTOR& other) const
	{
		LV_VECTOR vec = *this;
		vec.x += other.x;
		vec.y += other.y;
		return vec;
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

public:
	float x = 0.0f;
	float y = 0.0f;
};

typedef LV_VECTOR LV_SIZE;
typedef LV_VECTOR LV_COORD;