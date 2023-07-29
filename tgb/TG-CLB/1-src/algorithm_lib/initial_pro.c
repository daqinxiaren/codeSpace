#include "initial_pro.h"
#include "config.h"
#include "algor_interface.h"

static float ZXZC_low_limit[Diag_Num_ZXZC_Wheel] ={ZXZC_low_limit_1,ZXZC_low_limit_2,ZXZC_low_limit_3,ZXZC_low_limit_4,ZXZC_low_limit_5};
static float ZXZC_high_limit[Diag_Num_ZXZC_Wheel] ={ZXZC_high_limit_1,ZXZC_high_limit_2,ZXZC_high_limit_3,ZXZC_high_limit_4,ZXZC_high_limit_5};

static float ZXZC_base[Diag_Num_ZXZC_Wheel] = {ZXZC_base_1,ZXZC_base_2,ZXZC_base_3,ZXZC_base_4,ZXZC_base_5};  //特征值基准
static float ZXZC_bia[Diag_Num_ZXZC_Wheel] = {ZXZC_bia_1,ZXZC_bia_2,ZXZC_bia_3,ZXZC_bia_4,ZXZC_bia_5};	//偏置量，初期设置为10,内环，外环，滚动体，保持架，踏面

static float DJZC_low_limit[Diag_Num_DJZC] = {DJ_low_limit_1,DJ_low_limit_2,DJ_low_limit_3,DJ_low_limit_4,DJ_low_limit_5,DJ_low_limit_6,DJ_low_limit_7,DJ_low_limit_8};
static float DJZC_high_limit[Diag_Num_DJZC] ={DJ_high_limit_1,DJ_high_limit_2,DJ_high_limit_3,DJ_high_limit_4,DJ_high_limit_5,DJ_high_limit_6,DJ_high_limit_7,DJ_high_limit_8};

static float DJZC_base[Diag_Num_DJZC] = {DJ_base_1,DJ_base_2,DJ_base_3,DJ_base_4,DJ_base_5, DJ_base_6, DJ_base_7, DJ_base_8};  //特征值基准
static float DJZC_bia[Diag_Num_DJZC] = {DJ_bia_1,DJ_bia_2,DJ_bia_3,DJ_bia_4,DJ_bia_5,DJ_bia_6,DJ_bia_7,DJ_bia_8};	//偏置量，初期设置为10,内环，外环，滚动体，保持架，踏面

static float CLXZC_low_limit[Diag_Num_CLXZC] ={CLX_low_limit_1,CLX_low_limit_2,CLX_low_limit_3,CLX_low_limit_4,CLX_low_limit_5,CLX_low_limit_6,CLX_low_limit_7,CLX_low_limit_8};
static float CLXZC_high_limit[Diag_Num_CLXZC] ={CLX_high_limit_1,CLX_high_limit_2,CLX_high_limit_3,CLX_high_limit_4,CLX_high_limit_5,CLX_high_limit_6,CLX_high_limit_7,CLX_high_limit_8,};

static float CLXZC_base[Diag_Num_CLXZC] = {CLX_base_1,CLX_base_2,CLX_base_3,CLX_base_4,CLX_base_5, CLX_base_6, CLX_base_7, CLX_base_8};
static float CLXZC_bia[Diag_Num_CLXZC] = {CLX_bia_1,CLX_bia_2,CLX_bia_3,CLX_bia_4,CLX_bia_5,CLX_bia_6,CLX_bia_7,CLX_bia_8};

void fft_diag_ZXZC_Wheel_init(struct FFT_DIAG_PARA* fft_diag_paras)  //输出依次是轴箱轴承的内外滚保和踏面
{
    if (fft_diag_paras == 0)
    {
    	algor_printf("fft init fail!  fft diag paras is NULL!\n");
    	return;
    } 
	float threshold_prognosis[Diag_Num_ZXZC_Wheel]= {threshold_NGB_prognosis,threshold_W_prognosis,threshold_NGB_prognosis,threshold_NGB_prognosis,threshold_Wheel_prognosis};
	float threshold_prewarning[Diag_Num_ZXZC_Wheel]={threshold_NGB_prewarning,threshold_W_prewarning,threshold_NGB_prewarning,threshold_NGB_prewarning,threshold_Wheel_prewarning};
	float threshold_warning[Diag_Num_ZXZC_Wheel]=   {threshold_NGB_warning,threshold_W_warning,threshold_NGB_warning,threshold_NGB_warning,threshold_Wheel_warning};

	fft_diag_paras->diag_num = Diag_Num_ZXZC_Wheel;				//检测几个位置,最多只能检测Diag_Num个
	for(uint16_t i = 0; i < Diag_Num_ZXZC_Wheel; i++)
	{
		fft_diag_paras->low_limit[i] = ZXZC_low_limit[i];   //内环故障频率与车速的比例下限
		fft_diag_paras->high_limit[i] = ZXZC_high_limit[i];
		fft_diag_paras->base[i] = ZXZC_base[i];  //轴承特征值转换为dB需要的参数
		fft_diag_paras->bia[i] = ZXZC_bia[i];
		fft_diag_paras->threshold_prognosis[i]=threshold_prognosis[i];
		fft_diag_paras->threshold_prewarning[i]=threshold_prewarning[i];
		fft_diag_paras->threshold_warning[i]=threshold_warning[i];
	}
}


void fft_diag_DJ_init(struct FFT_DIAG_PARA* fft_diag_paras)   //输出依次是6215的内外滚保和NU216的内外滚保
{
    if (0 == fft_diag_paras)
    {
    	algor_printf("fft init fail!  fft diag paras is NULL!\n");
    	return ;
    }
	//数据来自第几通道？？？？？
	float threshold_prognosis[Diag_Num_DJZC]= {threshold_NGB_prognosis,threshold_W_prognosis,threshold_NGB_prognosis,threshold_NGB_prognosis,threshold_NGB_prognosis,threshold_W_prognosis,threshold_NGB_prognosis,threshold_NGB_prognosis};
	float threshold_prewarning[Diag_Num_DJZC]={threshold_NGB_prewarning,threshold_W_prewarning,threshold_NGB_prewarning,threshold_NGB_prewarning,threshold_NGB_prewarning,threshold_W_prewarning,threshold_NGB_prewarning,threshold_NGB_prewarning};
	float threshold_warning[Diag_Num_DJZC]=   {threshold_NGB_warning,threshold_W_warning,threshold_NGB_warning,threshold_NGB_warning,threshold_NGB_warning,threshold_W_warning,threshold_NGB_warning,threshold_NGB_warning};

	fft_diag_paras->diag_num = Diag_Num_DJZC;		 //检测几个位置,最多只能检测Diag_Num个
	for(uint16_t i = 0; i < Diag_Num_DJZC; i++)
	{
		fft_diag_paras->low_limit[i] = DJZC_low_limit[i];   //内环故障频率与车速的比例下限
		fft_diag_paras->high_limit[i] = DJZC_high_limit[i];
		fft_diag_paras->base[i] = DJZC_base[i];  //轴承特征值转换为dB需要的参数
		fft_diag_paras->bia[i] = DJZC_bia[i];
		fft_diag_paras->threshold_prognosis[i]=threshold_prognosis[i];
		fft_diag_paras->threshold_prewarning[i]=threshold_prewarning[i];
		fft_diag_paras->threshold_warning[i]=threshold_warning[i];
	}
}

void fft_diag_gear_init(struct FFT_DIAG_PARA* fft_diag_paras)
{

//第一个是下限，第二个是上限，上面5个分别是轴箱轴承的内、外、滚、保频率搜索范围以及踏面故障频率搜索范围

	fft_diag_paras->diag_num = Diag_Num_gear;
	fft_diag_paras->low_limit[0] = gear_low_limit;   //内环故障频率与车速的比例下限
	fft_diag_paras->high_limit[0] = gear_high_limit;
	fft_diag_paras->base[0] = gear_base;  //轴承特征值转换为dB需要的参数
	fft_diag_paras->bia[0] = gear_bia;
	fft_diag_paras->threshold_prognosis[0]=threshold_gear_prognosis;
	fft_diag_paras->threshold_prewarning[0]=threshold_gear_prewarning;
	fft_diag_paras->threshold_warning[0]=threshold_gear_warning;
}

void fft_diag_CLX_init(struct FFT_DIAG_PARA* fft_diag_paras)   //输出依次是NU216EC的内外滚保、NQJ216N2的内外滚保、T2DC_220的内外滚保
{
	//数据来自第几通道？？？？？
	
	 if (0 == fft_diag_paras)
    {
    	algor_printf("fft init fail!  fft diag paras is NULL!\n");
    	return ;
    }
	//数据来自第几通道？？？？？
	float threshold_prognosis[Diag_Num_CLXZC]= {threshold_NGB_prognosis,threshold_W_prognosis,threshold_NGB_prognosis,threshold_NGB_prognosis,threshold_NGB_prognosis,threshold_W_prognosis,threshold_NGB_prognosis,threshold_NGB_prognosis};
	float threshold_prewarning[Diag_Num_CLXZC]={threshold_NGB_prewarning,threshold_W_prewarning,threshold_NGB_prewarning,threshold_NGB_prewarning,threshold_NGB_prewarning,threshold_W_prewarning,threshold_NGB_prewarning,threshold_NGB_prewarning};
	float threshold_warning[Diag_Num_CLXZC]=   {threshold_NGB_warning,threshold_W_warning,threshold_NGB_warning,threshold_NGB_warning,threshold_NGB_warning,threshold_W_warning,threshold_NGB_warning,threshold_NGB_warning};

	fft_diag_paras->diag_num = Diag_Num_CLXZC;		 //检测几个位置,最多只能检测Diag_Num个
	for(uint16_t i = 0; i < Diag_Num_CLXZC; i++)
	{
		fft_diag_paras->low_limit[i] = CLXZC_low_limit[i];   //内环故障频率与车速的比例下限
		fft_diag_paras->high_limit[i] = CLXZC_high_limit[i];
		fft_diag_paras->base[i] = CLXZC_base[i];  //轴承特征值转换为dB需要的参数
		fft_diag_paras->bia[i] = CLXZC_bia[i];
		fft_diag_paras->threshold_prognosis[i]=threshold_prognosis[i];
		fft_diag_paras->threshold_prewarning[i]=threshold_prewarning[i];
		fft_diag_paras->threshold_warning[i]=threshold_warning[i];
	}
}



