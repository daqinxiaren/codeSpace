#ifndef __ZXZC_WHEEL_DIAG_H__
#define __ZXZC_WHEEL_DIAG_H__
#include "fft_diag.h"

typedef struct
{
	int status_bearing_comprehensive;
	int status_Wheel;
}ZXZC_status_out_t;//输出状态结构

/*
Function:    轴箱轴承+踏面诊断
Description: 输入的数据计算后，返回轴箱轴承和踏面的诊断结果
             int *block_3s_N,int *block_3s_W，int *block_3s_G，int *block_3s_B，int *block_3s_Wheel
             以上内外滚保4个buffer用于诊断中间值存储，不做输入输出
Input：       float *acc_buff                     采集存储了1s加速度buffer（轴箱冲击数据）
             int acc_buff_size                   1s内数据个数
             const struct FFT_DIAG_PARA fft_para 诊断所需参数配置，包括轮径，速度，采样率，频率范围等
Output:      FFT_DIAG_SAVE *fft_diag_save        中输出对应DB值，包括：DB,amp,fre
Return:      返回诊断状态结构体。状态值：0:预警， 1:1级报警， 2:2级报警
Others:
*/

ZXZC_status_out_t* ZXZC_Wheel_diag(float *acc_buff, const struct FFT_DIAG_PARA fft_para, struct FFT_DIAG_SAVE *fft_diag_save, zxzc_stack_t* stack);
#endif
