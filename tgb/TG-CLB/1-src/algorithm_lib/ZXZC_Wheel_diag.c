#include "ZXZC_Wheel_diag.h"
#include "fft_diag.h"
#include "min.h"
#include "algor_interface.h"
#include "status_judge.h"

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
ZXZC_status_out_t* ZXZC_Wheel_diag(float *acc_buff, const struct FFT_DIAG_PARA fft_para, struct FFT_DIAG_SAVE *fft_diag_save, zxzc_stack_t* stack)
{
	int count_3=3;   //3秒数据
	int status_bearing[4] = { 0 };
	int status_Wheel=0;
	int i;
	static ZXZC_status_out_t status = {0};

  	fft_diag(acc_buff, fft_para.Sample_rate, fft_para, fft_diag_save);
/**************************内环**************************/
	for(i=0;i<count_3-1;i++)
		stack->block_3s_N[i]=stack->block_3s_N[i+1];
	stack->block_3s_N[count_3-1]=fft_diag_save->DB[0];
	int min_block_3s_N;
	min_block_3s_N=min_int(stack->block_3s_N,3);
	status_bearing[0]=status_judge(min_block_3s_N, fft_para.threshold_prognosis[0], fft_para.threshold_prewarning[0], fft_para.threshold_warning[0]);
/**************************外环**************************/
	for(i=0;i<count_3-1;i++)
		stack->block_3s_W[i]=stack->block_3s_W[i+1];
	stack->block_3s_W[count_3-1]=fft_diag_save->DB[1];
	int min_block_3s_W;
	min_block_3s_W=min_int(stack->block_3s_W,3);
	status_bearing[1]=status_judge(min_block_3s_W, fft_para.threshold_prognosis[1], fft_para.threshold_prewarning[1], fft_para.threshold_warning[1]);
/**************************滚动体**************************/
	for(i=0;i<count_3-1;i++)
		stack->block_3s_G[i]=stack->block_3s_G[i+1];
	stack->block_3s_G[count_3-1]=fft_diag_save->DB[2];
	int min_block_3s_G;
	min_block_3s_G=min_int(stack->block_3s_G,3);
	status_bearing[2]=status_judge(min_block_3s_G, fft_para.threshold_prognosis[2], fft_para.threshold_prewarning[2], fft_para.threshold_warning[2]);
/**************************保持架**************************/
	for(i=0;i<count_3-1;i++)
		stack->block_3s_B[i]=stack->block_3s_B[i+1];
	stack->block_3s_B[count_3-1]=fft_diag_save->DB[3];
	int min_block_3s_B;
	min_block_3s_B=min_int(stack->block_3s_B,3);
	status_bearing[3]=status_judge(min_block_3s_B, fft_para.threshold_prognosis[3], fft_para.threshold_prewarning[3], fft_para.threshold_warning[3]);
/**************************踏面**************************/
	for(i=0;i<count_3-1;i++)
	{
		stack->block_3s_Wheel[i]=stack->block_3s_Wheel[i+1];
	}
	stack->block_3s_Wheel[count_3-1]=fft_diag_save->DB[4];
	int min_block_3s_Wheel;
	min_block_3s_Wheel=min_int(stack->block_3s_Wheel,3);
	status_Wheel=status_judge(min_block_3s_Wheel, fft_para.threshold_prognosis[4], fft_para.threshold_prewarning[4], fft_para.threshold_warning[4]);
/**************************轴承综合状态输出**************************/
	int status_bearing_comprehensive = 0;
	for(i=0;i<4;i++)
	{
		if(status_bearing[i]>status_bearing_comprehensive)
			status_bearing_comprehensive = status_bearing[i];
	}

	status.status_bearing_comprehensive = (int)status_bearing_comprehensive;   //输出轴承状态
	status.status_Wheel = (int)status_Wheel;									 //输出踏面状态
	return &status;
}
