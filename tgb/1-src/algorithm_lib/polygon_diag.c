#include "polygon_diag.h"
#include "new_fft.h"
#include "min.h"

/*
Function:    多边形诊断
Description: 输入一秒钟的加速度数据的buff，计算多边形之后输出结果
Input：       float *acc_buff                     采集存储了1s加速度buffer
             int acc_buff_size                   1s内数据个数
             struct POLYGON_DIAG_PARA polygon_para 诊断所需参数配置，输入参数结构体，包括轮径、采样频率、运行速度、预判、预警、报警阈值

             int *N_stack                        N个阶次存储堆栈
             float *K_stack                      N个指标存储堆栈
             float *speed_stack                  N个速度存储堆栈

Output:      struct POLYGON_DIAG_SAVE *polygon_diag_save 诊断结果存储空间
Return:      无
Others:
*/
void polygon_diag(float *acc_buff, struct POLYGON_DIAG_PARA polygon_para, struct POLYGON_DIAG_SAVE *polygon_diag_save,polygon_diag_stack_t* stack)
//		int *N_stack, float *K_stack, float *speed_stack)
{
	int i=0;
	int order=0;
	float K = 0;
	float speed=0;
	float fault_frequency=0;
	float fft_max_value=0;
	float fft_hz=0;
	float begin_fre=0;
	float end_fre=0;
	int N_stack_judge=0;	
	int speed_stack_judge=0;
	int status = 0;
	int time_length = polygon_para.time_length - 1;
	speed=polygon_para.polygon_diag_speed;//车速输入时是km/h
/******************速度栈存储区更新******************/
	for(i=0;i<time_length;i++)
		stack->speed_stack[i] = stack->speed_stack[i+1];
	stack->speed_stack[time_length]=speed;
/******************速度栈存储区判断******************/
	for(i=0;i<=time_length;i++)
	{
		if(stack->speed_stack[i]>5)		speed_stack_judge++;		//等于3则说明存在3次阶次相同
	}
	if (0.0 == polygon_para.wheel_diameter)
	{
		printf("polygon_diag.c in polygon_diag: wheel is 0, ERROR!!!\n");
		return;
	}
	fault_frequency=speed/3.6f / PI / polygon_para.wheel_diameter;  //轮对转频
	begin_fre=(float)round((double)fault_frequency)*15.0f;			//15*fault_frequency;搜索频率下限
	end_fre=(float)round((double)fault_frequency)*40.0f;					//40*fault_frequency;搜索频率上限
	
	substract_mean(acc_buff,polygon_para.Sample_rate);//去均值ֵ
	polygon_diag_save->rms_result = rms(acc_buff, polygon_para.Sample_rate);//有效值存储
	
	float DB_Rms = (float)20*(float)log10((double)polygon_diag_save->rms_result);//有效值对应的DB值
	if(DB_Rms<0)
	{
		DB_Rms=0;
	}
	float fft_size=0;
	fft_size=new_fft(acc_buff,polygon_para.Sample_rate);//原始数据做fft
	float fft_resolution = polygon_para.Sample_rate * 1.0f / fft_size;  //分辨率
	for(i=0; i<polygon_para.Sample_rate; i++)
	{
		acc_buff[i] = acc_buff[i]/(float)polygon_para.Sample_rate*2;
	}
	fft_max(acc_buff, begin_fre, end_fre, fft_resolution, &fft_max_value, &fft_hz);//搜索频率范围内的最大值

	if((int)fault_frequency!=0)
		order=(int)(float)round((double)fft_hz/(double)fault_frequency);//阶次计算
	else
		order=0;

/******************阶次栈存储区更新******************/
	for(i=0;i<time_length;i++)		stack->N_stack[i]=stack->N_stack[i+1];
	stack->N_stack[time_length]=order;
/******************阶次栈存储区判断******************/
	for(i=0;i<time_length;i++)
	{
		if((stack->N_stack[i+1]-stack->N_stack[i])==0)		N_stack_judge++;		//等于2则说明存在3次阶次相同
	}
/******************当前阶次存储******************/
	polygon_diag_save->order_result=order; //阶次存储
	
	polygon_diag_save->DB_result = 20*(float)log10((double)fft_max_value);//频谱幅值对应的DB值
	
	if(polygon_diag_save->DB_result<0)		polygon_diag_save->DB_result=0;
		
	K =(float)round(0.5*((double)polygon_diag_save->DB_result + (double)DB_Rms));//指标计算
	if(K < 0)		K=0;
	polygon_diag_save->indicator = K;  //指标存储
/******************多边形指标栈存储区更新******************/
	for(i=0;i<time_length;i++)
	{
		stack->K_stack[i]=stack->K_stack[i+1];
//		printf("------ k[%d] = %f\n",i,stack->K_stack[i]);
	}
	stack->K_stack[time_length] = K;
/******************多边形指标栈存储区判断******************/
	float K_stack_min;  //无脱轨板卡 用3秒堆栈区判断（分时采）
	int K_judge;  //判断
	K_stack_min = min(stack->K_stack,time_length+1);
	if((K_stack_min>polygon_para.threshold_prognosis) && (K_stack_min<=polygon_para.threshold_prewarning))
	{
		K_judge=1;
	}
	else if(K_stack_min>polygon_para.threshold_prewarning&&K_stack_min<=polygon_para.threshold_warning)
	{
		K_judge=2;
	}
	else if(K_stack_min>polygon_para.threshold_warning)
	{
		K_judge=3;
	}
	else
	{
		K_judge=0;
	}
	
	float nameda;
	nameda = (float)round ((double)speed / 3.6 / (double)fft_hz * 1000 );  //波磨波长 单位mm
	polygon_diag_save->nameda_result = nameda;  //波磨波长存储
	
	if((N_stack_judge==time_length)&&(K_judge==3)&&(speed_stack_judge==(time_length+1)))  //先判断是不是10次报警
		status=3;		//报警
	else if((N_stack_judge==time_length)&&(K_judge==2)&&(speed_stack_judge==(time_length+1))) //不满足报警再判断是不是10次预警
		status=2;   //预警
	else if((N_stack_judge==time_length)&&(K_judge==1)&&(speed_stack_judge==(time_length+1))) //不满足预警再判断是不是10次预判
		status=1; 	//预判
	polygon_diag_save->status_polygon=status;
	polygon_diag_save->speed = speed;
}
