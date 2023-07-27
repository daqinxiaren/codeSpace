#include "stdio.h"
#include "sqrt_sum.h" 
float sqrt_sum(float *buff, int size)
{
	int i;
	float s = 0.0f;

	for (i = 0; i < size; i++)
	{
	   s += buff[i] * buff[i];
	}

	return s;
}
