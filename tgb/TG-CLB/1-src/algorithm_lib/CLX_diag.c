#include "CLX_diag.h"
#include "fft_diag.h"
#include "min.h"
#include "status_judge.h"

int CLX_diag(float *acc_buff, const struct FFT_DIAG_PARA fft_para, struct FFT_DIAG_SAVE *fft_diag_save, dj_clx_stack_t* stack)
{
	fft_diag(acc_buff, fft_para.Sample_rate, fft_para, fft_diag_save);
/****************************************内环****************************************/
	int CLX_block_3s_N_input_DJ_side[3] = { 0 };
	int CLX_block_3s_N_input_Wheel_side[3] = { 0 };

	CLX_block_3s_N_input_DJ_side[0] = stack->block_3s_N[0];  //输入电机侧轴承内环第一秒
	CLX_block_3s_N_input_DJ_side[1] = stack->block_3s_N[1];  //输入电机侧轴承内环第二秒
	CLX_block_3s_N_input_DJ_side[2] = stack->block_3s_N[2];  //输入电机侧轴承内环第三秒

	CLX_block_3s_N_input_Wheel_side[0] = stack->block_3s_N[3];	//输入车轮侧轴承内环第一秒
	CLX_block_3s_N_input_Wheel_side[1] = stack->block_3s_N[4];	//输入车轮侧轴承内环第二秒
	CLX_block_3s_N_input_Wheel_side[2] = stack->block_3s_N[5];	//输入车轮侧轴承内环第三秒
/****************************************外环****************************************/
	int CLX_block_3s_W_input_DJ_side[3] = { 0 };
	int CLX_block_3s_W_input_Wheel_side[3] = { 0 };
	CLX_block_3s_W_input_DJ_side[0] = stack->block_3s_W[0];  //输入电机侧轴承外环第一秒
	CLX_block_3s_W_input_DJ_side[1] = stack->block_3s_W[1];  //输入电机侧轴承外环第二秒
	CLX_block_3s_W_input_DJ_side[2] = stack->block_3s_W[2];  //输入电机侧轴承外环第三秒

	CLX_block_3s_W_input_Wheel_side[0] = stack->block_3s_W[3];	//输入车轮侧轴承外环第一秒
	CLX_block_3s_W_input_Wheel_side[1] = stack->block_3s_W[4];	//输入车轮侧轴承外环第二秒
	CLX_block_3s_W_input_Wheel_side[2] = stack->block_3s_W[5];	//输入车轮侧轴承外环第三秒
/****************************************滚动体****************************************/
	int CLX_block_3s_G_input_DJ_side[3] = { 0 };
	int CLX_block_3s_G_input_Wheel_side[3] = { 0 };
	CLX_block_3s_G_input_DJ_side[0] = stack->block_3s_G[0];  //输入电机侧轴承滚动体第一秒
	CLX_block_3s_G_input_DJ_side[1] = stack->block_3s_G[1];  //输入电机侧轴承滚动体第二秒
	CLX_block_3s_G_input_DJ_side[2] = stack->block_3s_G[2];  //输入电机侧轴承滚动体第三秒

	CLX_block_3s_G_input_Wheel_side[0] = stack->block_3s_G[3];	//输入车轮侧轴承滚动体第一秒
	CLX_block_3s_G_input_Wheel_side[1] = stack->block_3s_G[4];	//输入车轮侧轴承滚动体第二秒
	CLX_block_3s_G_input_Wheel_side[2] = stack->block_3s_G[5];	//输入车轮侧轴承滚动体第三秒
/****************************************保持架****************************************/
	int CLX_block_3s_B_input_DJ_side[3] = { 0 };
	int CLX_block_3s_B_input_Wheel_side[3] = { 0 };
	CLX_block_3s_B_input_DJ_side[0] = stack->block_3s_B[0];  //输入电机侧轴承保持架第一秒
	CLX_block_3s_B_input_DJ_side[1] = stack->block_3s_B[1];  //输入电机侧轴承保持架第二秒
	CLX_block_3s_B_input_DJ_side[2] = stack->block_3s_B[2];  //输入电机侧轴承保持架第三秒

	CLX_block_3s_B_input_Wheel_side[0] = stack->block_3s_B[3];	//输入车轮侧轴承保持架第一秒
	CLX_block_3s_B_input_Wheel_side[1] = stack->block_3s_B[4];	//输入车轮侧轴承保持架第二秒
	CLX_block_3s_B_input_Wheel_side[2] = stack->block_3s_B[5];	//输入车轮侧轴承保持架第三秒
	
	int count_3=3;   //3秒数据
	
	int status_bearing_input_DJ_side[4] = { 0 };  	//输入侧电机侧轴承内、外、滚、保状态
	int status_bearing_input_Wheel_side[4] = { 0 }; //输入侧车轮侧轴承内、外、滚、保状态

	
	int i;
	/**************************内环状态堆栈区更新**************************/
	for(i=0;i<count_3-1;i++)
	{
		CLX_block_3s_N_input_DJ_side[i]=CLX_block_3s_N_input_DJ_side[i+1];
		CLX_block_3s_N_input_Wheel_side[i]=CLX_block_3s_N_input_Wheel_side[i+1];
	}
	CLX_block_3s_N_input_DJ_side[2]=(int)fft_diag_save->DB[0];  //第三秒幅值（新的结果）
	CLX_block_3s_N_input_Wheel_side[2]=(int)fft_diag_save->DB[4];  //第三秒幅值（新的结果）
	int min_CLX_block_3s_N_input_DJ_side;
	int min_CLX_block_3s_N_input_Wheel_side;
	min_CLX_block_3s_N_input_DJ_side = min_int(CLX_block_3s_N_input_DJ_side,3);
	min_CLX_block_3s_N_input_Wheel_side = min_int(CLX_block_3s_N_input_Wheel_side,3);
	/**************************外环状态堆栈区更新**************************/
	for(i=0;i<count_3-1;i++)
	{
		CLX_block_3s_W_input_DJ_side[i]=CLX_block_3s_W_input_DJ_side[i+1];
		CLX_block_3s_W_input_Wheel_side[i]=CLX_block_3s_W_input_Wheel_side[i+1];
	}
	CLX_block_3s_W_input_DJ_side[2]=(int)fft_diag_save->DB[1];  //第三秒幅值（新的结果）
	CLX_block_3s_W_input_Wheel_side[2]=(int)fft_diag_save->DB[5];  //第三秒幅值（新的结果）		
	int min_CLX_block_3s_W_input_DJ_side;
	int min_CLX_block_3s_W_input_Wheel_side;
	min_CLX_block_3s_W_input_DJ_side = min_int(CLX_block_3s_W_input_DJ_side,3);
	min_CLX_block_3s_W_input_Wheel_side = min_int(CLX_block_3s_W_input_Wheel_side,3);
	/**************************滚动体状态堆栈区更新**************************/
	for(i=0;i<count_3-1;i++)
	{
		CLX_block_3s_G_input_DJ_side[i]=CLX_block_3s_G_input_DJ_side[i+1];
		CLX_block_3s_G_input_Wheel_side[i]=CLX_block_3s_G_input_Wheel_side[i+1];
	}
	CLX_block_3s_G_input_DJ_side[2]=(int)fft_diag_save->DB[2];  //第三秒幅值（新的结果）
	CLX_block_3s_G_input_Wheel_side[2]=(int)fft_diag_save->DB[6];  //第三秒幅值（新的结果）
	float min_CLX_block_3s_G_input_DJ_side;
	float min_CLX_block_3s_G_input_Wheel_side;
	min_CLX_block_3s_G_input_DJ_side = min_int(CLX_block_3s_G_input_DJ_side,3);
	min_CLX_block_3s_G_input_Wheel_side = min_int(CLX_block_3s_G_input_Wheel_side,3);
	/**************************保持架状态堆栈区更新**************************/
	for(i=0;i<count_3-1;i++)
	{
		CLX_block_3s_B_input_DJ_side[i]=CLX_block_3s_B_input_DJ_side[i+1];
		CLX_block_3s_B_input_Wheel_side[i]=CLX_block_3s_B_input_Wheel_side[i+1];
	}
	CLX_block_3s_B_input_DJ_side[2]=(int)fft_diag_save->DB[3];  //第三秒幅值（新的结果）
	CLX_block_3s_B_input_Wheel_side[2]=(int)fft_diag_save->DB[7];  //第三秒幅值（新的结果）
	int min_CLX_block_3s_B_input_DJ_side;
	int min_CLX_block_3s_B_input_Wheel_side;
	min_CLX_block_3s_B_input_DJ_side = min_int(CLX_block_3s_B_input_DJ_side,3);
	min_CLX_block_3s_B_input_Wheel_side = min_int(CLX_block_3s_B_input_Wheel_side,3);
/**************************************单轴承状态输出区**************************************/
	float min_value_NWGB[4] = { 0 };
/*************************电机侧轴承报警状态*************************/	
	min_value_NWGB[0] = min_CLX_block_3s_N_input_DJ_side;
	min_value_NWGB[1] = min_CLX_block_3s_W_input_DJ_side;
	min_value_NWGB[2] = min_CLX_block_3s_G_input_DJ_side;
	min_value_NWGB[3] = min_CLX_block_3s_B_input_DJ_side;
	for(i=0;i<4;i++)
	{
		status_bearing_input_DJ_side[i]=status_judge(min_value_NWGB[i], fft_para.threshold_prognosis[i], fft_para.threshold_prewarning[i], fft_para.threshold_warning[i]);
	}
/*************************车轮侧轴承内环是否满足报警以及报警状态*************************/
	min_value_NWGB[0] = min_CLX_block_3s_N_input_Wheel_side;
	min_value_NWGB[1] = min_CLX_block_3s_W_input_Wheel_side;
	min_value_NWGB[2] = min_CLX_block_3s_G_input_Wheel_side;
	min_value_NWGB[3] = min_CLX_block_3s_B_input_Wheel_side;
	for(i=0;i<4;i++)
		status_bearing_input_Wheel_side[i]=status_judge(min_value_NWGB[i], fft_para.threshold_prognosis[i+4], fft_para.threshold_prewarning[i+4], fft_para.threshold_warning[i+4]);
/**************************轴承综合状态输出**************************/
	/****************************电机侧轴承****************************/
	int status_bearing_input_DJ_side_comprehensive = 0;  
	for(i=0;i<4;i++)
	{
		if(status_bearing_input_DJ_side[i]>status_bearing_input_DJ_side_comprehensive)
			status_bearing_input_DJ_side_comprehensive = status_bearing_input_DJ_side[i];
	}
	/****************************车轮侧轴承****************************/
	int status_bearing_input_Wheel_side_comprehensive = 0;  
	for(i=0;i<4;i++)
	{
		if(status_bearing_input_Wheel_side[i]>status_bearing_input_Wheel_side_comprehensive)
			status_bearing_input_Wheel_side_comprehensive = status_bearing_input_Wheel_side[i];
	}
	
	/************************最终齿轮箱状态结果输出************************/
	int status;

	status=status_bearing_input_Wheel_side_comprehensive;
	if(status_bearing_input_DJ_side_comprehensive>status)
		status=status_bearing_input_DJ_side_comprehensive;
	
/************************堆栈区更新************************/
	/***************************内环***************************/
	stack->block_3s_N[0] = CLX_block_3s_N_input_DJ_side[0];   //输入电机侧轴承内环第一秒
	stack->block_3s_N[1] = CLX_block_3s_N_input_DJ_side[1];	 //输入电机侧轴承内环第二秒
	stack->block_3s_N[2] = CLX_block_3s_N_input_DJ_side[2];   //输入电机侧轴承内环第三秒

	stack->block_3s_N[3] = CLX_block_3s_N_input_Wheel_side[0];//输入轮对侧轴承内环第一秒
	stack->block_3s_N[4] = CLX_block_3s_N_input_Wheel_side[1];//输入轮对侧轴承内环第一秒
	stack->block_3s_N[5] = CLX_block_3s_N_input_Wheel_side[2];//输入轮对侧轴承内环第一秒

/***************************外环***************************/
	stack->block_3s_W[0] = CLX_block_3s_W_input_DJ_side[0];   //输入电机侧轴承外环第一秒
	stack->block_3s_W[1] = CLX_block_3s_W_input_DJ_side[1];	 //输入电机侧轴承外环第二秒
	stack->block_3s_W[2] = CLX_block_3s_W_input_DJ_side[2];   //输入电机侧轴承外环第三秒

	stack->block_3s_W[3] = CLX_block_3s_W_input_Wheel_side[0];//输入轮对侧轴承外环第一秒
	stack->block_3s_W[4] = CLX_block_3s_W_input_Wheel_side[1];//输入轮对侧轴承外环第一秒
	stack->block_3s_W[5] = CLX_block_3s_W_input_Wheel_side[2];//输入轮对侧轴承外环第一秒

/***************************滚动体***************************/
	stack->block_3s_G[0] = CLX_block_3s_G_input_DJ_side[0];   //输入电机侧轴承外环第一秒
	stack->block_3s_G[1] = CLX_block_3s_G_input_DJ_side[1];	 //输入电机侧轴承外环第二秒
	stack->block_3s_G[2] = CLX_block_3s_G_input_DJ_side[2];   //输入电机侧轴承外环第三秒

	stack->block_3s_G[3] = CLX_block_3s_G_input_Wheel_side[0];//输入轮对侧轴承外环第一秒
	stack->block_3s_G[4] = CLX_block_3s_G_input_Wheel_side[1];//输入轮对侧轴承外环第一秒
	stack->block_3s_G[5] = CLX_block_3s_G_input_Wheel_side[2];//
	
/***************************保持架***************************/
	stack->block_3s_B[0] = CLX_block_3s_B_input_DJ_side[0];   //输入电机侧轴承保持架第一秒
	stack->block_3s_B[1] = CLX_block_3s_B_input_DJ_side[1];	 //输入电机侧轴承保持架第二秒
	stack->block_3s_B[2] = CLX_block_3s_B_input_DJ_side[2];   //输入电机侧轴承保持架第三秒

	stack->block_3s_B[3] = CLX_block_3s_B_input_Wheel_side[0];//输入轮对侧轴承保持架第一秒
	stack->block_3s_B[4] = CLX_block_3s_B_input_Wheel_side[1];//输入轮对侧轴承保持架第一秒
	stack->block_3s_B[5] = CLX_block_3s_B_input_Wheel_side[2];//输入轮对侧轴承保持架第一秒
	
	return status;
}
