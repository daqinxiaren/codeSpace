#include "stdio.h"
#include "IIR_polygon.h" 
#include "config.h"
static double FILTER_NUM[9]={POLYGON_FILTER_NUM_1,POLYGON_FILTER_NUM_2,POLYGON_FILTER_NUM_3,POLYGON_FILTER_NUM_4,POLYGON_FILTER_NUM_5,POLYGON_FILTER_NUM_6,POLYGON_FILTER_NUM_7,POLYGON_FILTER_NUM_8,POLYGON_FILTER_NUM_9};
static double FILTER_DEN[9]={POLYGON_FILTER_DEN_1,POLYGON_FILTER_DEN_2,POLYGON_FILTER_DEN_3,POLYGON_FILTER_DEN_4,POLYGON_FILTER_DEN_5,POLYGON_FILTER_DEN_6,POLYGON_FILTER_DEN_7,POLYGON_FILTER_DEN_8,POLYGON_FILTER_DEN_9};

/* IIR滤波器*/

float IIR_polygon(float *x, float *y)
{
	
	int i;
	float x_result=0;
	float y_result=0;
	float result=0;
	int filter_Len_Num = sizeof(FILTER_NUM) / sizeof(FILTER_NUM[0]);
	int filter_Len_Den = sizeof(FILTER_DEN) / sizeof(FILTER_DEN[0]);

	double *num = FILTER_NUM;	
	double *den = FILTER_DEN;	
	
	if (x == NULL)
	{
		return 0;
	}

	if (y == NULL)
	{
		return 0;
	}

	if (x && y && num && den)
	{
		int n_Den = filter_Len_Den;
		int n_Num = filter_Len_Num;
		x_result=0;
		y_result=0;
		for(i=0; i<n_Num; i++)
		{
			x_result+=FILTER_NUM[i]*x[filter_Len_Num-i-1];
		}
		for(i=1;i<n_Den;i++)
		{
			y_result-=FILTER_DEN[i]*y[filter_Len_Den-i-1];
		}
		result=(x_result+y_result);
		return result;
	}
	else
	{
		return 0;
	}
}

/*
Function:         单个采集加速度滤波处理（仅多边形适用）
Description:      
Input：            float data 采集数据计算后的加速度
                   float* x 用于存放10s数据的当前通道的堆栈buffer
				   float* y 用于存放10s数据的当前通道的堆栈buffer
Output:           None
Return:           返回滤波后的结果。
Others:
*/
float IIR_filter_polygon(float data, float* x, float* y)
{
	int filter_length = 9;   //滤波器长度为9
	float IIR_polygon_result=0;
	int i = 0;

	for(i=0; i<filter_length-1; i++)
	{
		x[i] = x[i+1];
	}
	x[filter_length-1] = data;
	
	IIR_polygon_result = IIR_polygon(x,y);  //IIR滤波
	for(i=0; i<filter_length-1; i++)
	{
		y[i] = y[i+1];
	}
	y[filter_length-2] = IIR_polygon_result;
	return IIR_polygon_result;
}
