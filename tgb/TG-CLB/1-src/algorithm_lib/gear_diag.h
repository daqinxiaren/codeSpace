#ifndef __GEAR_DIAG_H__
#define __GEAR_DIAG_H__
#include "fft_diag.h"

int gear_diag(float *acc_buff, const struct FFT_DIAG_PARA fft_para, struct FFT_DIAG_SAVE *fft_diag_save, int* block_3s_gear);

#endif
