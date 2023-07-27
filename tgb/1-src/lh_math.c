#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "lh_math.h"


/*获取最大频率幅值*/
void fre_get_fft_max(float *buff,int begin_fre,int end_fre,float *fre_resolution,float *max_val,float *fft_val)
{

    int begin_index = 0;
    int end_index = 0;
    int cnt_i = 0;
    float max_value = 0.0;
    float resolution=*fre_resolution;
    if(begin_fre<=0)
    {
        begin_fre=1;
    }
    begin_index = (int)(begin_fre/resolution + 0.5);
    end_index =(int)(end_fre/resolution + 0.5);

    for(cnt_i = begin_index;cnt_i < end_index;cnt_i ++)
    {
        if(buff[cnt_i] > max_value)
        {
            max_value = buff[cnt_i];
            *max_val = max_value;
            *fft_val = cnt_i*resolution;
        }
    }

}

/**
 * 均值函数
*/
float float_mean(float *buff,int size)
{
	int i;
	float sum=buff[0];
	if(buff==NULL)
    	return 0;
    for(i=1;i<size;i++)
	{
		sum+=buff[i];
	}
	return sum/size;
}

/**
 * 去均值操作
 */
void float_sub_mean(float *buff,int size)
{
	int i;
	float s=float_mean(buff,size);
	for(i=0;i<size;i++)
	{
		buff[i]=buff[i]-s;
	}
}
/**
 * 计算均方根
 */
float float_std_rms(float *src, int size)
{
    return sqrtf(float_sum_x2(src, size) / size);
}

/**
 * 
 * 计算两个数组的乘积的和
 */
float float_sum_xy(float x[], float y[], int size)
{
    int  i;
    float s = 0.0f;

    for (  i = 0; i < size; i++)
    {
        s += x[i] * y[i];
    }
    return s;
}

/**
 * 计算两个数组的和乘积
 */
float float_multiply_sumx_sumy(float x[], float y[], int size)
{
    float sumx = 0.0f, sumy = 0.0f;
    sumx = float_sum(x, size);
    sumy = float_sum(y, size);
    return sumx * sumy;
}

/**
 * 计算一个数组的平方和
 * 
 */
float float_sum_x2(float x[], int size)
{
    int i;
    float s = 0.0f;

    for (i = 0; i < size; i++)
    {
        s += x[i] * x[i];
    }

    return s;
}

/**
 * 计算一个数组的和的平方
 */
float float_sumx_2(float x[], int size)
{
    float s = 0.0f;
    s = float_sum(x, size);
    return s * s;
}

/**
 * 计算一个数组的和
 */
float float_sum(float x[], int size)
{
    int i;
     float sum = 0.0f;

    for (  i = 0; i < size; i++)
    {
        sum += x[i];
    }

    return sum;
}

/**
 * 计算最大值
 * check
 */
float float_get_max(float *buff, int size)
{
    int i;
    float max_val = buff[0];

    for (  i = 0; i < size; i++)
    {
        if (max_val < buff[i])
        {
            max_val = buff[i];
        }
    }
    return max_val;
}

/**
 * 计算最小值
 * check
 */
float float_get_min(float *buff, int size)
{
    int i;
    float min_val = buff[0];
    for (  i = 0; i < size; i++)
    {
        if (min_val >buff[i])
        {
            min_val = buff[i];
        }
    }
    return min_val;
}
