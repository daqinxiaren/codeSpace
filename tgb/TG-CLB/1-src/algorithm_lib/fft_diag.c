#include "fft_diag.h"
#include "fft_max.h"

/* macro */

/* variable declaration */

/* function declaration */
/**
 * @brief
 * @param
 * @return
 * @note
**/
void fft_diag(float *acc_buff, int acc_buff_size, const struct FFT_DIAG_PARA fft_para, struct FFT_DIAG_SAVE *fft_diag_save)
{
	if(acc_buff==NULL||fft_diag_save==NULL)
		return;
	int i=0;

	float speed =fft_para.diag_speed;//�ٶ�
	float size = 0;
	float fft_resolution = 1;   //分辨率只和采用了几秒数据有关，和采样率，数据长度无关，因此不受重采样影响

	struct Para_Range search_range[Diag_Num_Max] = {0};		//Diag_Num_Max 目前设置最大搜索频段数为64

	
	for(i= 0;i<fft_para.diag_num ; i++)
	{
		search_range[i].low_limit = speed * fft_para.low_limit[i];	//搜索频率下限
		search_range[i].high_limit = speed * fft_para.high_limit[i];//搜索频率上限
	}
	float amp[Diag_Num_Max] = {0}; //幅值
	float fre[Diag_Num_Max] = {0}; //频段中最大幅值所在频率

	if(fft_para.diag_speed<DIAG_SPEED_MIN)
	{
		fft_diag_save->DB[i]= 0;
		fft_diag_save->amp[i] = amp[i];
		fft_diag_save->fre[i] = fre[i];
	}
	else
	{
		size=new_fft(acc_buff, (uint16_t)acc_buff_size);

		fft_resolution = (float)fft_para.Sample_rate / size;
		for(i=0;i<acc_buff_size;i++)
		{
			acc_buff[i] = acc_buff[i]/(float)acc_buff_size*2;
		}
		for(i= 0;i<fft_para.diag_num ; i++)
		{
			fft_max(acc_buff, search_range[i].low_limit, search_range[i].high_limit, fft_resolution,&amp[i],&fre[i]);
			fft_diag_save->DB[i]= (int)(20*log10(((double)amp[i]/(double)fft_para.base[i]))+(int)fft_para.bia[i]+0.5);
			if(fft_diag_save->DB[i]<0)
			{
				fft_diag_save->DB[i] = 0;
			}
			fft_diag_save->amp[i] = amp[i];
			fft_diag_save->fre[i] = fre[i];

		}
	}
}

