#include "MathLibrary.h"

#include <cstdlib>
#include <ctime>

void MathLibrary::SeedRandom()
{
	srand(time(0));
}

void MathLibrary::SeedRandom(unsigned int seed)
{
	srand(seed);
}

int MathLibrary::GetRandomInRange(int min, int max)
{
	return min + rand() % (max - min + 1);
}
