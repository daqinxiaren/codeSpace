#ifndef __initial_pro_H__
#define __initial_pro_H__

#include "fft_diag.h"

typedef struct
{
	struct FFT_DIAG_PARA  fft_diag_zxzc_paras;
}initial_pro_env_t;
extern initial_pro_env_t initial_pro_env;
void fft_diag_ZXZC_Wheel_init(struct FFT_DIAG_PARA* fft_diag_paras);
void fft_diag_DJ_init(struct FFT_DIAG_PARA* fft_diag_paras);
void fft_diag_gear_init(struct FFT_DIAG_PARA* fft_diag_paras);
void fft_diag_CLX_init(struct FFT_DIAG_PARA* fft_diag_paras);
//void polygon_diag_init(struct FFT_DIAG_PARA* fft_diag_paras, float speed, float Wheel_diameter, int fs);


#endif 
