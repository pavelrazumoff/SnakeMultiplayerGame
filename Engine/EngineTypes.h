#pragma once

#include "stdint.h"

/** Types specifically for objects placed on Game Levels. */

typedef float LV_FLOAT;

struct LV_COORD
{
	LV_FLOAT x;
	LV_FLOAT y;
};

struct LV_SIZE
{
	LV_FLOAT cx;
	LV_FLOAT cy;
};

struct LV_VECTOR
{
	LV_FLOAT x;
	LV_FLOAT y;
};
