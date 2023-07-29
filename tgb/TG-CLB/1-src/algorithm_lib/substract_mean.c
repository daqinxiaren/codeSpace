#include "stdio.h"
#include "substract_mean.h" 
#include "mean.h"

void substract_mean(float *buff,int size)
{
	int i;
	float s=mean(buff,size);
	for(i=0;i<size;i++)
	{
		buff[i]=buff[i]-s;
	}
}

