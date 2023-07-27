#ifndef __POLYGON_DIAG_H__
#define __POLYGON_DIAG_H__

#include "stdlib.h"
#include "math.h"
#include "mean.h"
#include "rms.h"
#include "sqrt_sum.h"
#include "substract_mean.h"
#include "fft_max.h"
#include "config.h"
/**
 * 多边形算法需要的配置
 */
 
struct POLYGON_DIAG_PARA
{
	float wheel_diameter;       // m    轮径，默认是0.805
	float Sample_rate;			//采样频率
	float polygon_diag_speed;   // km/h 车辆运行的速度
	float threshold_prognosis;
	float threshold_prewarning;
	float threshold_warning;
	int time_length;            //堆栈空间的长度
};
/**
 * 多边形算法计算完之后的特征数据
 */
struct POLYGON_DIAG_SAVE
{
	int order_result;         //特征数据1  阶次
	float rms_result;         //特征数据2  加速度均方根
	float DB_result;          //特征数据3  DB值
	int indicator;            //特征数据4  评价指标，用于计算波磨指标
	float nameda_result;	  //特征数据5  钢轨波磨波长，用于计算波磨波长
	int status_polygon;       //诊断状态
	float speed;
};
typedef struct
{
	int N_stack[POLYGON_time_length];
	float K_stack[POLYGON_time_length];
	float speed_stack[POLYGON_time_length];
}polygon_diag_stack_t;
/*
Function:    多边形诊断
Description: 输入一秒钟的加速度数据的buff，计算多边形之后输出结果
Input：       float *acc_buff                     采集存储了1s加速度buffer
             int acc_buff_size                   1s内数据个数
             struct POLYGON_DIAG_PARA polygon_para 诊断所需参数配置，输入参数结构体，包括轮径、采样频率、运行速度、预判、预警、报警阈值

             int *N_stack                        N个阶次存储堆栈
             float *K_stack                      N个指标存储堆栈
             float *speed_stack                  N个速度存储堆栈

Output:      struct POLYGON_DIAG_SAVE *polygon_diag_save 诊断结果存储空间
Return:      无
Others:
*/
void polygon_diag(float *acc_buff, struct POLYGON_DIAG_PARA polygon_para, struct POLYGON_DIAG_SAVE *polygon_diag_save,polygon_diag_stack_t* stack);

#endif /* __POLYGON_DIAG_H__ */
