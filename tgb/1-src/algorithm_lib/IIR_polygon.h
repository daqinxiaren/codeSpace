#ifndef __IIR_polygon_h__
#define __IIR_polygon_h__

#include "config.h"

//520Hz低通滤波器
float IIR_polygon(float *x, float *y);

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
float IIR_filter_polygon(float data, float* x, float* y);
#endif
