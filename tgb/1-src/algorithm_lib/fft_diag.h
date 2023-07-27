#ifndef __FFT_DIAG_H__
#define __FFT_DIAG_H__

#include "stdio.h"
#include "mean.h"
#include "substract_mean.h"
#include "rms.h"
#include "new_fft.h"
#include "stdlib.h"
#include "config.h"
#ifndef MCU_ARM
#include "fft_max.h"
typedef unsigned short uint16_t;
#endif
#define Diag_Num_Max 64   //����Ǵ洢����ĸ�������ͬ�����������ж����diag_num
#define DIAG_SPEED_MIN    15
/**
 * ����㷨����
 */
 struct Para_Range
{
	float low_limit;			//Ƶ���복�ٵı�������
	float high_limit;			//Ƶ���복�ٵı�������
};
struct FFT_DIAG_PARA
{
	float wheel_diameter;       		//轮径，单位：
	float diag_speed;   				//车速
	int Sample_rate;								//采样频率
	int diag_num;										//诊断项点数
	float low_limit[Diag_Num_Max];	//下限
	float high_limit[Diag_Num_Max];	//上限
	float base[Diag_Num_Max];  		  //基准
	float bia[Diag_Num_Max];  			//偏置
	float threshold_prognosis[Diag_Num_Max];
	float threshold_prewarning[Diag_Num_Max];
	float threshold_warning[Diag_Num_Max];
};

struct FFT_DIAG_SAVE
{
	int DB[Diag_Num_Max];                 //诊断DB值
	float amp[Diag_Num_Max];                //诊断幅值
	float fre[Diag_Num_Max];                //频段最大值所在频率
};

typedef struct
{
	int block_3s_N[3];
	int block_3s_W[3];
	int block_3s_G[3];
	int block_3s_B[3];
	int block_3s_Wheel[3];
}zxzc_stack_t;

typedef struct
{
	int block_3s_N[6];
	int block_3s_W[6];
	int block_3s_G[6];
	int block_3s_B[6];
}dj_clx_stack_t;

typedef struct
{
	struct FFT_DIAG_PARA fft_diag_paras;
	struct FFT_DIAG_SAVE fft_diag_save;
}fft_diag_env_t;

//extern fft_diag_env_t fft_diag_env;

//void fft_diag_zxzc_init(void);
void fft_diag(float *acc_buff, int acc_buff_size, const struct FFT_DIAG_PARA fft_para, struct FFT_DIAG_SAVE *fft_diag_save);

#endif
