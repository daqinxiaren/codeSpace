#include "status_judge.h"

int status_judge(float v, float threshold_prognosis, float threshold_prewarning, float threshold_warning)
{
	int status;
	if(v<=threshold_prognosis)
		status=0;  //内环状态正常
	else if(v>threshold_prognosis && v<=threshold_prewarning)
		status=1;  //内环状态预判
	else if(v>threshold_prewarning && v<=threshold_warning)
		status=2;  //内环状态预警
	else
		status=3;  //内环状态报警	
	return status;
}
