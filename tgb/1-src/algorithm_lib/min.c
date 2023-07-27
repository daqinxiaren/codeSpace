#include "min.h"

float min(float* buff, int N)
{
	float temp=buff[0];
	int k;
	for(k=1;k<N;k++)
	{
		if(buff[k]<temp)
			temp=buff[k];
	}
	return temp;
}

int min_int(int* buff, int N)
{
	int temp=buff[0];
	int k;
	for(k=1;k<N;k++)
	{
		if(buff[k]<temp)
			temp=buff[k];
	}
	return temp;
}
