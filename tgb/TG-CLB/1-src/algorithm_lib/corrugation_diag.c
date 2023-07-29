#include "corrugation_diag.h"
/*
Function:    波磨诊断
Description: 将8个轴箱多边形计算出的结果进行波磨的计算，并输出两边钢轨的计算结果
Input：      float *indicator  存放8个轴箱多边形计算的评价指标的buffer(按照轴箱编号顺序存储)
             float *nameda     存放8个轴箱多边形计算后的钢轨波磨波长的buffer(按照轴箱编号顺序存储)
Output:      struct CORRUGATION_DIAG_SAVE *corrugation_diag_save 诊断结果存储buffer
Return:      无
Others:
*/
void corrugation_diag(float *indicator, float *nameda, struct CORRUGATION_DIAG_SAVE *corrugation_diag_save)
{
	float corrugation_indicator1357;
	float corrugation_indicator2468;
	float corrugation_nameda1357;
	float corrugation_nameda2468;

	corrugation_indicator1357=( indicator[0]+indicator[2]+indicator[4]+indicator[6] ) / 4;
	corrugation_indicator2468=( indicator[1]+indicator[3]+indicator[5]+indicator[7] ) / 4;
	corrugation_nameda1357=( nameda[0]+nameda[2]+nameda[4]+nameda[6] ) / 4;
	corrugation_nameda2468=( nameda[1]+nameda[3]+nameda[5]+nameda[7] ) / 4;
	
	corrugation_diag_save->indicator1357_result = corrugation_indicator1357;
	corrugation_diag_save->indicator2468_result = corrugation_indicator2468;
	corrugation_diag_save->nameda1357_result = corrugation_nameda1357;
	corrugation_diag_save->nameda2468_result =	corrugation_nameda2468;
}
