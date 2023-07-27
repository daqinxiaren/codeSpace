#include "gear_diag.h"
#include "min.h"
int gear_diag(float *acc_buff, const struct FFT_DIAG_PARA fft_para, struct FFT_DIAG_SAVE *fft_diag_save, int* block_3s_gear)
{
	fft_diag(acc_buff, fft_para.Sample_rate, fft_para, fft_diag_save);

	int count_3=3;   //3秒数据
	int status_gear = 0;
	
	int i;
/**************************齿轮**************************/
	for(i=0;i<count_3-1;i++)
		block_3s_gear[i]=block_3s_gear[i+1];
	block_3s_gear[count_3-1]=(int)fft_diag_save->DB[0];
  int min_block_3s_gear;
	min_block_3s_gear = min_int(block_3s_gear,3);
	if(min_block_3s_gear<=fft_para.threshold_prognosis[0])
		status_gear=0;		//正常
	else if(min_block_3s_gear>fft_para.threshold_prognosis[0] && min_block_3s_gear<=fft_para.threshold_prewarning[0])
		status_gear=1;		//̤预判
	else if(min_block_3s_gear>fft_para.threshold_prewarning[0] && min_block_3s_gear<=fft_para.threshold_warning[0])
		status_gear=2;		//̤预警
	else
		status_gear=3;		//̤报警
/**************************齿轮状态输出**************************/

	return status_gear;
}
