#ifndef _LH_MATH_H
#define _LH_MATH_H
#include <stdio.h>
#include <stdlib.h>

#define PI 3.141592653589793

float float_mean(float *buff, int size);
void float_sub_mean(float *buff, int size);
float float_sum(float x[], int size);
float float_sum_xy(float x[], float y[], int size);
float float_multiply_sumx_sumy(float x[], float y[], int size);
float float_sum_x2(float x[], int size);
float float_sumx_2(float x[], int size);
float float_sum(float x[], int size);
float float_get_max(float *buff, int size);
float float_get_min(float *buff, int size);
float float_std_rms(float *src, int size);
void fre_get_fft_max(float *buff,int begin_fre,int end_fre,float *fre_resolution,float *max_val,float *fft_val);
#endif
