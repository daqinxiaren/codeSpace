#ifndef __CLX_DIAG_H__
#define __CLX_DIAG_H__
#include "fft_diag.h"

int CLX_diag(float *acc_buff, const struct FFT_DIAG_PARA fft_para, struct FFT_DIAG_SAVE *fft_diag_save, dj_clx_stack_t* stack);

#endif
