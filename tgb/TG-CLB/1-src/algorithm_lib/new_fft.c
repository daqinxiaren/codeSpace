#include "stdlib.h"
#include "math.h"
#include "new_fft.h"
#include "algor_interface.h"
#include "config.h"
#ifndef MCU_ARM
#include "fftw3.h"
#endif

typedef unsigned short uint16_t;
#define REAL      0
#define IMAG      1

#ifdef MCU_ARM
static float* fft_temp = NULL; //指向fft运算缓存的buffer，大小为：4096*2*4 最大4k采样率
#else
static fftw_complex *fftinput = NULL;
static fftw_complex *fftoutput = NULL;
#endif

float new_fft(float *buff, int size)
{
	uint16_t i;
	int fft_size;
	if(size>=32 && size<64)
		fft_size = 32;
	else if(size>=64 && size<128)
		fft_size = 64;
	else if(size>=128 && size<256)
		fft_size = 128;
	else if(size>=256 && size<512)
		fft_size = 256;
	else if(size>=512 && size<1024)
		fft_size = 512;
	else if(size>=1024 && size<=2048)
		fft_size = 1024;
	else if(size>=2048 && size<4096)
		fft_size = 2048;
	else if(size>=4096)
		fft_size = 4096;
	else
		fft_size = 0;

#ifdef MCU_ARM

	arm_cfft_radix4_instance_f32 scfft;
    if (NULL == fft_temp)
    {
    	return 0;
    }
	memset(fft_temp, 0, sizeof(float)*fft_size*2);
	for (i = 0; i < fft_size; i++)
	{
		 fft_temp[2 * i] = buff[i];
	}
	arm_cfft_radix4_init_f32(&scfft, (uint16_t)fft_size, 0, 1);
	arm_cfft_radix4_f32(&scfft, fft_temp);
	arm_cmplx_mag_f32(fft_temp, buff, (uint16_t)fft_size);
//	algor_free(fft_temp);

#else /* LINUX_ARM */
	fftw_plan fft;
    for (int i = 0; i < size; i++)
    {
    	fftinput[i][REAL] = buff[i]; //实部
    	fftinput[i][IMAG] = 0;       //虚部
    }

	fft = fftw_plan_dft_1d(size, fftinput, fftoutput, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(fft);
	fftw_destroy_plan(fft);
	fft = 0;

	//fft变换后，得到的结果数量和参与计算的数量相同，由于fft得到的结果是折中对称的，所以只取一半结果即可
	for (i=0; i<size/2; i++)
	{
		buff[i] = sqrt(fftoutput[i][REAL]*fftoutput[i][REAL] + fftoutput[i][IMAG]*fftoutput[i][IMAG]);
	}
#endif /* MCU_ARM */
	return fft_size;  //返回fft_size
}

/**
 * @brief  算法初始化函数,上电时需先调用此函数
 * @param
 * @return
 * @note   在上电时分配内存空间
**/
int fft_diag_init(void)
{
#ifdef MCU_ARM
	fft_temp = algor_malloc(sizeof(float) * 4096 * 2);
	if (NULL == fft_temp)
	{
		algor_printf("algor_malloc failed !!!!!\n");
		return -1;
	}
#else /*LINUX ARM*/
	fftinput = (fftw_complex*)algor_malloc(sizeof(fftw_complex)*4096);
	if (NULL == fftinput)
	{
		printf("algor_malloc fftinput failed !!!!!\n");
		return -1;
	}
	fftoutput = (fftw_complex*)algor_malloc(sizeof(fftw_complex)*4096);
	if (NULL == fftoutput)
	{
		printf("algor_malloc fftoutput failed !!!!!\n");
		return -1;
	}
#endif
	return 0;
}
/**
 * @brief  算法初始化函数
 * @param
 * @return
 * @note   在上电时分配内存空间
**/
void fft_diag_destroy(void)
{
#ifdef MCU_ARM
	algor_free(fft_temp);
#else
	algor_free(fftinput);
	algor_free(fftoutput);
#endif
}
