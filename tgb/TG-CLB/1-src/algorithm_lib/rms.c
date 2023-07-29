#include "stdio.h"
#include "rms.h"
#include "sqrt_sum.h"
#include "math.h"
float rms(float *src, int size)
{
	return sqrtf(sqrt_sum(src, size) / (float)size);
}
