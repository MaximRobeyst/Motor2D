#pragma once
#include <stdlib.h>

inline int Random(int min, int max)
{
	return min + rand() % (max - min + 1);
}

inline float Random(float min, float max)
{
	return min + (rand() / (RAND_MAX / (max - min)));
}
