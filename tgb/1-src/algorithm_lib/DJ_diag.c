#include "DJ_diag.h"
#include "fft_diag.h"
#include "min.h"

int DJ_diag(float *acc_buff, const struct FFT_DIAG_PARA fft_para, struct FFT_DIAG_SAVE *fft_diag_save, dj_clx_stack_t* stack)
{
	fft_diag(acc_buff, fft_para.Sample_rate, fft_para, fft_diag_save);
/****************************************内环****************************************/
	int DJ_block_3s_N_FCD_side[3] = { 0 };
	int DJ_block_3s_N_CD_side[3] = { 0 };
	DJ_block_3s_N_FCD_side[0] = stack->block_3s_N[0];  //电机非传动端轴承内环第一秒
	DJ_block_3s_N_FCD_side[1] = stack->block_3s_N[1];  //电机非传动端轴承内环第二秒
	DJ_block_3s_N_FCD_side[2] = stack->block_3s_N[2];  //电机非传动端轴承内环第三秒

	DJ_block_3s_N_CD_side[0] = stack->block_3s_N[3];	//电机传动端轴承内环第一秒
	DJ_block_3s_N_CD_side[1] = stack->block_3s_N[4];	//电机传动端轴承内环第二秒
	DJ_block_3s_N_CD_side[2] = stack->block_3s_N[5];	//电机传动端轴承内环第三秒
/****************************************外环****************************************/
	int DJ_block_3s_W_FCD_side[3] = { 0 };
	int DJ_block_3s_W_CD_side[3] = { 0 };
	DJ_block_3s_W_FCD_side[0] = stack->block_3s_W[0];  //电机非传动端轴承外环第一秒
	DJ_block_3s_W_FCD_side[1] = stack->block_3s_W[1];  //电机非传动端轴承外环第二秒
	DJ_block_3s_W_FCD_side[2] = stack->block_3s_W[2];  //电机非传动端轴承外环第三秒

	DJ_block_3s_W_CD_side[0] = stack->block_3s_W[3];	//电机传动端轴承外环第一秒
	DJ_block_3s_W_CD_side[1] = stack->block_3s_W[4];	//电机传动端轴承外环第二秒
	DJ_block_3s_W_CD_side[2] = stack->block_3s_W[5];	//电机传动端轴承外环第三秒
/****************************************滚动体****************************************/
	int DJ_block_3s_G_FCD_side[3] = { 0 };
	int DJ_block_3s_G_CD_side[3] = { 0 };
	DJ_block_3s_G_FCD_side[0] = stack->block_3s_G[0];  //电机非传动端轴承滚动体第一秒
	DJ_block_3s_G_FCD_side[1] = stack->block_3s_G[1];  //电机非传动端轴承滚动体第二秒
	DJ_block_3s_G_FCD_side[2] = stack->block_3s_G[2];  //电机非传动端轴承滚动体第三秒

	DJ_block_3s_G_CD_side[0] = stack->block_3s_G[3];	//电机传动端轴承滚动体第一秒
	DJ_block_3s_G_CD_side[1] = stack->block_3s_G[4];	//电机传动端轴承滚动体第二秒
	DJ_block_3s_G_CD_side[2] = stack->block_3s_G[5];	//电机传动端轴承滚动体第三秒
/****************************************滚动体****************************************/
	int DJ_block_3s_B_FCD_side[3] = { 0 };
	int DJ_block_3s_B_CD_side[3] = { 0 };
	DJ_block_3s_B_FCD_side[0] = stack->block_3s_B[0];  //输入电机侧轴承保持架第一秒
	DJ_block_3s_B_FCD_side[1] = stack->block_3s_B[1];  //输入电机侧轴承保持架第二秒
	DJ_block_3s_B_FCD_side[2] = stack->block_3s_B[2];  //输入电机侧轴承保持架第三秒

	DJ_block_3s_B_CD_side[0] = stack->block_3s_B[3];	//输入车轮侧轴承保持架第一秒
	DJ_block_3s_B_CD_side[1] = stack->block_3s_B[4];	//输入车轮侧轴承保持架第二秒
	DJ_block_3s_B_CD_side[2] = stack->block_3s_B[5];	//输入车轮侧轴承保持架第三秒
	
	int count_3=3;   //3秒数据
//	int judge_N_FCD_side=0;   			//电机非传动端轴承内环判断
//	int judge_N_CD_side=0;   				//电机传动端轴承内环判断
//
//	int judge_W_FCD_side=0;   			//电机非传动端轴承外环判断
//	int judge_W_CD_side=0; 					//电机传动端轴承外环判断
//
//	int judge_G_FCD_side=0;   			//电机非传动端轴承滚动体判断
//	int judge_G_CD_side=0; 					//电机传动端轴承滚动体判断
//
//	int judge_B_FCD_side=0;   			//电机非传动端轴承保持架判断
//	int judge_B_CD_side=0; 					//电机传动端轴承保持架判断
	
	int status_bearing_FCD_side[4] = { 0 };  	//电机非传动端轴承内、外、滚、保状态
	int status_bearing_CD_side[4] = { 0 };    //电机传动端轴承内、外、滚、保状态
	
	int i;
/**************************内环状态堆栈区更新**************************/
	for(i=0;i<count_3-1;i++)
	{
		DJ_block_3s_N_FCD_side[i]=DJ_block_3s_N_FCD_side[i+1];
		DJ_block_3s_N_CD_side[i]=DJ_block_3s_N_CD_side[i+1];
	}
	DJ_block_3s_N_FCD_side[2]=(int)fft_diag_save->DB[0];  //第三秒幅值（新的结果）
	DJ_block_3s_N_CD_side[2]=(int)fft_diag_save->DB[4];  //第三秒幅值（新的结果）
	int min_DJ_block_3s_N_FCD_side;
	int min_DJ_block_3s_N_CD_side;
	min_DJ_block_3s_N_FCD_side = min_int(DJ_block_3s_N_FCD_side,3);
	min_DJ_block_3s_N_CD_side = min_int(DJ_block_3s_N_CD_side,3);
/**************************外环状态堆栈区更新**************************/
	for(i=0;i<count_3-1;i++)
	{
		DJ_block_3s_W_FCD_side[i]=DJ_block_3s_W_FCD_side[i+1];
		DJ_block_3s_W_CD_side[i]=DJ_block_3s_W_CD_side[i+1];
	}
	DJ_block_3s_W_FCD_side[2]=(int)fft_diag_save->DB[1];  //第三秒幅值（新的结果）
	DJ_block_3s_W_CD_side[2]=(int)fft_diag_save->DB[5];  //第三秒幅值（新的结果）
	int min_DJ_block_3s_W_FCD_side;
	int min_DJ_block_3s_W_CD_side;
	min_DJ_block_3s_W_FCD_side = min_int(DJ_block_3s_W_FCD_side,3);
	min_DJ_block_3s_W_CD_side = min_int(DJ_block_3s_W_CD_side,3);
/**************************滚动体状态堆栈区更新**************************/
	for(i=0;i<count_3-1;i++)
	{
		DJ_block_3s_G_FCD_side[i]=DJ_block_3s_G_FCD_side[i+1];
		DJ_block_3s_G_CD_side[i]=DJ_block_3s_G_CD_side[i+1];
	}
	DJ_block_3s_G_FCD_side[2]=(int)fft_diag_save->DB[2];  //第三秒幅值（新的结果）
	DJ_block_3s_G_CD_side[2]=(int)fft_diag_save->DB[6];  //第三秒幅值（新的结果）
	int min_DJ_block_3s_G_FCD_side;
	int min_DJ_block_3s_G_CD_side;
	min_DJ_block_3s_G_FCD_side = min_int(DJ_block_3s_G_FCD_side,3);
	min_DJ_block_3s_G_CD_side = min_int(DJ_block_3s_G_CD_side,3);
/**************************保持架状态堆栈区更新**************************/
	for(i=0;i<count_3-1;i++)
	{
		DJ_block_3s_B_FCD_side[i]=DJ_block_3s_B_FCD_side[i+1];
		DJ_block_3s_B_CD_side[i]=DJ_block_3s_B_CD_side[i+1];
	}
	DJ_block_3s_B_FCD_side[2]=(int)fft_diag_save->DB[3];  //第三秒幅值（新的结果）
	DJ_block_3s_B_CD_side[2]=(int)fft_diag_save->DB[7];  //第三秒幅值（新的结果）
	int min_DJ_block_3s_B_FCD_side;
	int min_DJ_block_3s_B_CD_side;
	min_DJ_block_3s_B_FCD_side = min_int(DJ_block_3s_B_FCD_side,3);
	min_DJ_block_3s_B_CD_side = min_int(DJ_block_3s_B_CD_side,3);
	
/**************************************单轴承状态输出区**************************************/
	/*************************非传动端轴承内环是否满足报警以及报警状态*************************/
	if(min_DJ_block_3s_N_FCD_side<=fft_para.threshold_prognosis[0])
		status_bearing_FCD_side[0]=0;  //内环状态正常
	else if(min_DJ_block_3s_N_FCD_side>fft_para.threshold_prognosis[0]&&min_DJ_block_3s_N_FCD_side<=fft_para.threshold_prewarning[0])
		status_bearing_FCD_side[0]=1;  //内环状态预判
	else if(min_DJ_block_3s_N_FCD_side>fft_para.threshold_prewarning[0]&&min_DJ_block_3s_N_FCD_side<=fft_para.threshold_warning[0])
		status_bearing_FCD_side[0]=2;  //内环状态预警
	else
		status_bearing_FCD_side[0]=3;  //内环状态报警		
	/*************************非传动端轴承外环是否满足报警以及报警状态*************************/
	if(min_DJ_block_3s_W_FCD_side<=fft_para.threshold_prognosis[1])
		status_bearing_FCD_side[1]=0;  //外环状态正常
	else if(min_DJ_block_3s_W_FCD_side>fft_para.threshold_prognosis[1]&&min_DJ_block_3s_W_FCD_side<=fft_para.threshold_prewarning[1])
		status_bearing_FCD_side[1]=1;  //外环状态预判
	else if(min_DJ_block_3s_W_FCD_side>fft_para.threshold_prewarning[1]&&min_DJ_block_3s_W_FCD_side<=fft_para.threshold_warning[1])
		status_bearing_FCD_side[1]=2;  //外环状态预警
	else
		status_bearing_FCD_side[1]=3;  //外环状态报警	
	/*************************非传动端轴承滚动体是否满足报警以及报警状态*************************/
	if(min_DJ_block_3s_G_FCD_side<=fft_para.threshold_prognosis[2])
		status_bearing_FCD_side[2]=0;  //滚动体状态正常
	else if(min_DJ_block_3s_G_FCD_side>fft_para.threshold_prognosis[2]&&min_DJ_block_3s_G_FCD_side<=fft_para.threshold_prewarning[2])
		status_bearing_FCD_side[2]=1;  //滚动体状态预判
	else if(min_DJ_block_3s_G_FCD_side>fft_para.threshold_prewarning[2]&&min_DJ_block_3s_G_FCD_side<=fft_para.threshold_warning[2])
		status_bearing_FCD_side[2]=2;  //滚动体状态预警
	else
		status_bearing_FCD_side[2]=3;  //滚动体状态报警
	/*************************非传动端轴承保持架是否满足报警以及报警状态*************************/
	if(min_DJ_block_3s_B_FCD_side<=fft_para.threshold_prognosis[3])
		status_bearing_FCD_side[3]=0;  //保持架状态正常
	else if(min_DJ_block_3s_B_FCD_side>fft_para.threshold_prognosis[3]&&min_DJ_block_3s_B_FCD_side<=fft_para.threshold_prewarning[3])
		status_bearing_FCD_side[3]=1;  //保持架状态预判
	else if(min_DJ_block_3s_B_FCD_side>fft_para.threshold_prewarning[3]&&min_DJ_block_3s_B_FCD_side<=fft_para.threshold_warning[3])
		status_bearing_FCD_side[3]=2;  //保持架状态预警
	else
		status_bearing_FCD_side[3]=3;  //保持架状态报警
	/*************************传动端轴承内环是否满足报警以及报警状态*************************/
	if(min_DJ_block_3s_N_CD_side<=fft_para.threshold_prognosis[4])
		status_bearing_CD_side[0]=0;  //内环状态正常
	else if(min_DJ_block_3s_N_CD_side>fft_para.threshold_prognosis[4]&&min_DJ_block_3s_N_CD_side<=fft_para.threshold_prewarning[4])
		status_bearing_CD_side[0]=1;  //内环状态预判
	else if(min_DJ_block_3s_N_CD_side>fft_para.threshold_prewarning[4]&&min_DJ_block_3s_N_CD_side<=fft_para.threshold_warning[4])
		status_bearing_CD_side[0]=2;  //内环状态预警
	else
		status_bearing_CD_side[0]=3;  //内环状态报警	
	/*************************传动端轴承外环是否满足报警以及报警状态*************************/
	if(min_DJ_block_3s_W_CD_side<=fft_para.threshold_prognosis[5])
		status_bearing_CD_side[1]=0;  //外环状态正常
	else if(min_DJ_block_3s_W_CD_side>fft_para.threshold_prognosis[5]&&min_DJ_block_3s_W_CD_side<=fft_para.threshold_prewarning[5])
		status_bearing_CD_side[1]=1;  //外环状态预判
	else if(min_DJ_block_3s_W_CD_side>fft_para.threshold_prewarning[5]&&min_DJ_block_3s_W_CD_side<=fft_para.threshold_warning[5])
		status_bearing_CD_side[1]=2;  //外环状态预警
	else
		status_bearing_CD_side[1]=3;  //外环状态报警	
	/*************************传动端轴承滚动体是否满足报警以及报警状态*************************/
	if(min_DJ_block_3s_G_CD_side<=fft_para.threshold_prognosis[6])
		status_bearing_CD_side[2]=0;  //滚动体状态正常
	else if(min_DJ_block_3s_G_CD_side>fft_para.threshold_prognosis[6]&&min_DJ_block_3s_G_CD_side<=fft_para.threshold_prewarning[6])
		status_bearing_CD_side[2]=1;  //滚动体状态预判
	else if(min_DJ_block_3s_G_CD_side>fft_para.threshold_prewarning[6]&&min_DJ_block_3s_G_CD_side<=fft_para.threshold_warning[6])
		status_bearing_CD_side[2]=2;  //滚动体状态预警
	else
		status_bearing_CD_side[2]=3;  //滚动体状态报警
	/*************************传动端轴承保持架是否满足报警以及报警状态*************************/
	if(min_DJ_block_3s_B_CD_side<=fft_para.threshold_prognosis[7])
		status_bearing_CD_side[3]=0;  //保持架状态正常
	else if(min_DJ_block_3s_B_CD_side>fft_para.threshold_prognosis[7]&&min_DJ_block_3s_B_CD_side<=fft_para.threshold_prewarning[7])
		status_bearing_CD_side[3]=1;  //保持架状态预判
	else if(min_DJ_block_3s_B_CD_side>fft_para.threshold_prewarning[7]&&min_DJ_block_3s_B_CD_side<=fft_para.threshold_warning[7])
		status_bearing_CD_side[3]=2;  //保持架状态预警
	else
		status_bearing_CD_side[3]=3;  //保持架状态报警
/**************************轴承综合状态输出**************************/
	/****************************非传动端轴承****************************/
	int status_bearing_FCD_side_comprehensive = 0;  
	for(i=0;i<4;i++)
	{
		if(status_bearing_FCD_side[i]>status_bearing_FCD_side_comprehensive)
			status_bearing_FCD_side_comprehensive = status_bearing_FCD_side[i];
	}
	/****************************传动端轴承****************************/
	int status_bearing_CD_side_comprehensive = 0;  
	for(i=0;i<4;i++)
	{
		if(status_bearing_CD_side[i]>status_bearing_CD_side_comprehensive)
			status_bearing_CD_side_comprehensive = status_bearing_CD_side[i];
	}	
	/************************最终电机状态结果输出************************/
	int status = 0;

	status=status_bearing_FCD_side_comprehensive;
	if(status_bearing_CD_side_comprehensive>status)
		status=status_bearing_CD_side_comprehensive;
/************************堆栈区更新************************/
	/***************************内环***************************/	
	stack->block_3s_N[0] = DJ_block_3s_N_FCD_side[0];   //电机非传动侧轴承内环第一秒
	stack->block_3s_N[1] = DJ_block_3s_N_FCD_side[1];	 //电机非传动侧轴承内环第二秒
	stack->block_3s_N[2] = DJ_block_3s_N_FCD_side[2];   //电机非传动侧轴承内环第三秒

	stack->block_3s_N[3] = DJ_block_3s_N_CD_side[0];//电机非传动侧轴承内环第一秒
	stack->block_3s_N[4] = DJ_block_3s_N_CD_side[1];//电机非传动侧轴承内环第一秒
	stack->block_3s_N[5] = DJ_block_3s_N_CD_side[2];//电机非传动侧轴承内环第一秒
	
	/***************************外环***************************/	
	stack->block_3s_W[0] = DJ_block_3s_W_FCD_side[0];   //电机非传动侧轴承内环第一秒
	stack->block_3s_W[1] = DJ_block_3s_W_FCD_side[1];	 //电机非传动侧轴承内环第二秒
	stack->block_3s_W[2] = DJ_block_3s_W_FCD_side[2];   //电机非传动侧轴承内环第三秒

	stack->block_3s_W[3] = DJ_block_3s_W_CD_side[0];//电机非传动侧轴承内环第一秒
	stack->block_3s_W[4] = DJ_block_3s_W_CD_side[1];//电机非传动侧轴承内环第一秒
	stack->block_3s_W[5] = DJ_block_3s_W_CD_side[2];//电机非传动侧轴承内环第一秒
	
	/***************************滚动体***************************/	
	stack->block_3s_G[0] = DJ_block_3s_G_FCD_side[0];   //电机非传动侧轴承内环第一秒
	stack->block_3s_G[1] = DJ_block_3s_G_FCD_side[1];	 //电机非传动侧轴承内环第二秒
	stack->block_3s_G[2] = DJ_block_3s_G_FCD_side[2];   //电机非传动侧轴承内环第三秒

	stack->block_3s_G[3] = DJ_block_3s_G_CD_side[0];//电机非传动侧轴承内环第一秒
	stack->block_3s_G[4] = DJ_block_3s_G_CD_side[1];//电机非传动侧轴承内环第一秒
	stack->block_3s_G[5] = DJ_block_3s_G_CD_side[2];//电机非传动侧轴承内环第一秒
	
	/***************************保持架***************************/	
	stack->block_3s_B[0] = DJ_block_3s_B_FCD_side[0];   //电机非传动侧轴承内环第一秒
	stack->block_3s_B[1] = DJ_block_3s_B_FCD_side[1];	 //电机非传动侧轴承内环第二秒
	stack->block_3s_B[2] = DJ_block_3s_B_FCD_side[2];   //电机非传动侧轴承内环第三秒

	stack->block_3s_B[3] = DJ_block_3s_B_CD_side[0];//电机非传动侧轴承内环第一秒
	stack->block_3s_B[4] = DJ_block_3s_B_CD_side[1];//电机非传动侧轴承内环第一秒
	stack->block_3s_B[5] = DJ_block_3s_B_CD_side[2];//电机非传动侧轴承内环第一秒
	
	return status;
}
