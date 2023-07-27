#ifndef __CORRUGATION_DIAG_H__
#define __CORRUGATION_DIAG_H__

struct CORRUGATION_DIAG_SAVE
{
    float indicator1357_result;              	 //1357位车轮对应钢轨的波磨指标
    float indicator2468_result;                //2468位车轮对应钢轨的波磨指标
    float nameda1357_result;              	 //1357位车轮对应钢轨的波长指标
    float nameda2468_result;              	 //1357位车轮对应钢轨的波长指标
};

void corrugation_diag(float *indicator, float *nameda, struct CORRUGATION_DIAG_SAVE *corrugation_diag_save);

#endif 
