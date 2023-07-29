#include "stdio.h"
#include "mean.h" 

float mean(float *buff,int size)
{
	int i;
	float sum=buff[0];
	if(buff==NULL)
		return 0;		//
	for(i=1;i<size;i++)
	{
		sum+=buff[i];	
	}
	return sum/(float)size;
}  
