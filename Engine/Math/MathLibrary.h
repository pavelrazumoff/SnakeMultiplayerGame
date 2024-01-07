#pragma once

class MathLibrary
{
public:
	static void SeedRandom();
	static void SeedRandom(unsigned int seed);

	static int GetRandomInRange(int min, int max);
};
