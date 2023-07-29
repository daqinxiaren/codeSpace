#include "main.h"
#include "fft_diag.h"
#include "initial_pro.h"
#include "ZXZC_Wheel_diag.h"
#include "CLX_diag.h"
#include "DJ_diag.h"
#include "gear_diag.h"
#include "polygon_diag.h"
#include "corrugation_diag.h"
#include "IIR_polygon.h"
#if 0
struct FFT_DIAG_PARA fft_diag_paras;
struct FFT_DIAG_SAVE fft_diag_save;

int main()
{
	float speed=100;
//	int *status_block_ZXZC_Wheel;
//	status_block_ZXZC_Wheel = malloc(sizeof(int) * (uint16_t)fft_diag_env.fft_diag_paras.diag_num * 3);  //*3意思是3秒的堆栈,这是轴箱轴承和踏面的3s block
//	
	int k=0;
	int i=0;
	float data_fft[40960] = { 0 };   //假如有40960个数据，共10秒
	float data_polygon[40960] = { 0 };   //假如有40960个数据，共10秒
	float *data_1s_fft;
	float *data_1s_polygon;
	data_1s_fft = malloc(sizeof(float) * (uint16_t)fft_diag_env.fft_diag_paras.Sample_Rate);
	data_1s_polygon = malloc(sizeof(float) * (uint16_t)fft_diag_env.fft_diag_paras.Sample_Rate);
/**********************************多边形区**********************************/
	int count_1s_polygon=0;
	int N1,N2,N3,N4,N5,N6,N7,N8; 											//8个踏面的多边形阶次
	float rms1,rms2,rms3,rms4,rms5,rms6,rms7,rms8; 		//8个踏面你的加速度均方根值
	float DB1,DB2,DB3,DB4,DB5,DB6,DB7,DB8;  					//8个踏面的频谱DB值
	float K1 = 0,K2= 0,K3= 0,K4= 0,K5= 0,K6= 0,K7= 0,K8=0;   								  //8个踏面的多边形指标
	float nameda1= 0,nameda2= 0,nameda3= 0,nameda4= 0,nameda5= 0,nameda6= 0,nameda7= 0,nameda8 = 0;//8个踏面对应的钢轨波磨波长
	int status1= 0,status2= 0,status3= 0,status4= 0,status5= 0,status6= 0,status7= 0,status8;  //8个踏面的多边形状态
	float speed1= 0,speed2= 0,speed3= 0,speed4= 0,speed5= 0,speed6= 0,speed7= 0,speed8;				//8个踏面的车速
	struct POLYGON_DIAG_PARA polygon_para;
	polygon_para.Sample_Rate=4096; //检查
	polygon_para.wheel_diameter=(float)0.805;
	polygon_para.polygon_diag_speed=100;
	polygon_para.threshold_prognosis=17;
	polygon_para.threshold_prewarning=22;	
	polygon_para.threshold_warning=27;
	polygon_para.time_length = 10;  //堆栈区长度为10
	int N_stack[10] = { 0 };   //10次
	float K_stack[10] = { 0 };
	float speed_stack[10] = { 0 };
	struct POLYGON_DIAG_SAVE polygon_save = {0};
/****************定义滤波空间*****************/
	float x[9]= { 0 };
	float y[9] ={ 0 };
/****************定义滤波结果*****************/
	float IIR_polygon_result=0;
/****************定义滤波器长度*****************/
	int filter_length = 9;   //滤波器长度为9
	
/**********************************波磨区**********************************/
	struct CORRUGATION_DIAG_SAVE corrugation_save = {0};
	float corrugation_K1357 = 0; 	 	//1 3 5 7侧踏面对应钢轨的波磨指标
	float corrugation_K2468 = 0;		//2 4 6 8侧踏面对应钢轨的波磨指标
	float corrugation_nameda1357 = 0;//1 3 5 7侧踏面对应钢轨的波磨波长
	float corrugation_nameda2468 = 0;//2 4 6 8侧踏面对应钢轨的波磨波长
	/********************************************轴承齿轮踏面区**********************************************
																						****
																				****		****
																			****				****
                                       存储区定义开始
																	 ****              ****
																 ****									 ****
******************************************************************************************/
	int count_1s_fft=0;
/****************************此处定义了轴箱轴承和踏面3s的堆栈***************************/
	int block_3s_ZXZC_N[3] = { 0 };  //轴箱轴承内环
	int block_3s_ZXZC_W[3] = { 0 };  //轴箱轴承外环
	int block_3s_ZXZC_G[3] = { 0 };  //轴箱轴承滚动体
	int block_3s_ZXZC_B[3] = { 0 };  //轴箱轴承保持架
	int block_3s_Wheel[3] = { 0 };   //踏面状态
	ZXZC_status_out_t *status_bearing_wheel = 0;	 //轴箱轴承和踏面函数输出
	status_bearing_wheel = malloc(sizeof(int) * 2);
	int status_bearing_ZX = 0;  //轴箱轴承最终状态
	int status_wheel = 0; 			//踏面最终状态
	/****************************轴箱轴承和踏面DB值****************************/
	int ZXZC_DB_N = 0;  //轴箱轴承DB值
	int ZXZC_DB_W = 0;
	int ZXZC_DB_G = 0;
	int ZXZC_DB_B = 0;
	int Wheel_DB = 0;
	/****************************轴箱轴承和踏面amp值****************************/
	float ZXZC_amp_N = 0; //轴箱轴承amp值
	float ZXZC_amp_W = 0;
	float ZXZC_amp_G = 0;
	float ZXZC_amp_B = 0;	
	float Wheel_amp = 0;
	/****************************轴箱轴承和踏面fre值****************************/
	float ZXZC_fre_N = 0; //轴箱轴承fre值
	float ZXZC_fre_W = 0;
	float ZXZC_fre_G = 0;
	float ZXZC_fre_B = 0;		
	float Wheel_fre = 0;
/****************************此处定义了齿轮箱轴承3s的堆栈***************************/
	int block_3s_CLX_N[2 * 3] = { 0 };  //齿轮箱轴承内环 2种轴承 3秒堆栈
	int block_3s_CLX_W[2 * 3] = { 0 };  //齿轮箱轴承外环 2种轴承 3秒堆栈
	int block_3s_CLX_G[2 * 3] = { 0 };  //齿轮箱轴承滚动体 2种轴承 3秒堆栈
	int block_3s_CLX_B[2 * 3] = { 0 };  //齿轮箱轴承保持架 2种轴承 3秒堆栈
	int status_bearing_CLX = 0; //齿轮箱轴承最终状态
	/****************************齿轮箱轴承DB值****************************/
	int CLX_DB_N_DJ_side = 0; 		//齿轮箱电机侧轴承DB值
	int CLX_DB_W_DJ_side = 0;
	int CLX_DB_G_DJ_side = 0;
	int CLX_DB_B_DJ_side = 0;
	
	int CLX_DB_N_Wheel_side = 0;  //齿轮箱车轮侧轴承DB值
	int CLX_DB_W_Wheel_side = 0;
	int CLX_DB_G_Wheel_side = 0;
	int CLX_DB_B_Wheel_side = 0;
	/****************************轴箱轴承和踏面amp值****************************/
	float CLX_amp_N_DJ_side = 0; 		//齿轮箱电机侧轴承amp值
	float CLX_amp_W_DJ_side = 0;
	float CLX_amp_G_DJ_side = 0;
	float CLX_amp_B_DJ_side = 0;	
	
	float CLX_amp_N_Wheel_side = 0; //齿轮箱车轮侧轴承amp值
	float CLX_amp_W_Wheel_side = 0;
	float CLX_amp_G_Wheel_side = 0;
	float CLX_amp_B_Wheel_side = 0;	
	/****************************齿轮箱轴承fre值****************************/
	float CLX_fre_N_DJ_side = 0; 	  //齿轮箱电机侧轴承fre值
	float CLX_fre_W_DJ_side = 0;
	float CLX_fre_G_DJ_side = 0;
	float CLX_fre_B_DJ_side = 0;	
	
	float CLX_fre_N_Wheel_side = 0; //齿轮箱车轮侧轴承fre值
	float CLX_fre_W_Wheel_side = 0;
	float CLX_fre_G_Wheel_side = 0;
	float CLX_fre_B_Wheel_side = 0;	
/****************************此处定义了电机轴承3s的堆栈***************************/
	int block_3s_DJ_N[2 * 3] = { 0 };  //齿轮箱轴承内环 2种轴承 3秒堆栈
	int block_3s_DJ_W[2 * 3] = { 0 };  //齿轮箱轴承外环 2种轴承 3秒堆栈
	int block_3s_DJ_G[2 * 3] = { 0 };  //齿轮箱轴承滚动体 2种轴承 3秒堆栈
	int block_3s_DJ_B[2 * 3] = { 0 };  //齿轮箱轴承保持架 2种轴承 3秒堆栈
	int status_bearing_DJ = 0;  //电机轴承最终状态
	/****************************电机轴承DB值****************************/
	int DJ_DB_N_FCD_side = 0; //电机非传动端轴承DB值
	int DJ_DB_W_FCD_side = 0;
	int DJ_DB_G_FCD_side = 0;
	int DJ_DB_B_FCD_side = 0;
	
	int DJ_DB_N_CD_side = 0;  //电机传动端轴承DB值
	int DJ_DB_W_CD_side = 0;
	int DJ_DB_G_CD_side = 0;
	int DJ_DB_B_CD_side = 0;
	/****************************电机轴承amp值****************************/
	float DJ_amp_N_FCD_side = 0; //电机非传动端轴承amp值
	float DJ_amp_W_FCD_side = 0;
	float DJ_amp_G_FCD_side = 0;
	float DJ_amp_B_FCD_side = 0;	
	
	float DJ_amp_N_CD_side = 0; //电机传动端轴承amp值
	float DJ_amp_W_CD_side = 0;
	float DJ_amp_G_CD_side = 0;
	float DJ_amp_B_CD_side = 0;	
	/****************************齿轮箱轴承fre值****************************/
	float DJ_fre_N_FCD_side = 0; 	//电机非传动端轴承fre值
	float DJ_fre_W_FCD_side = 0;
	float DJ_fre_G_FCD_side = 0;
	float DJ_fre_B_FCD_side = 0;	
	
	float DJ_fre_N_CD_side = 0;   //电机传动端轴承fre值
	float DJ_fre_W_CD_side = 0;
	float DJ_fre_G_CD_side = 0;
	float DJ_fre_B_CD_side = 0;	
/****************************此处定义了齿轮3s的堆栈***************************/
 	int block_3s_gear[3] = { 0 };  //齿轮箱轴承内环 2种轴承 3秒堆栈
	/****************************最终状态输出****************************/
	int status_gear = 0;				//齿轮最终状态
/****************************齿轮DB值****************************/
	int gear_DB = 0;	  //齿轮DB值
/****************************齿轮amp值****************************/
	float gear_amp = 0; //齿轮amp值	
/****************************齿轮fre值****************************/
	float gear_fre = 0; //齿轮amp值	
	
/******************************************************************************************
																  ****									****
																		****		          ****
																			****				  ****
                                        存储区定义结束
																	      ****      ****
																             ****			
******************************************************************************************/
	for(k=0;k<40960;k++)
	{
		data_1s_fft[count_1s_fft]=data_fft[k];
		count_1s_fft++;
		data_1s_polygon[count_1s_polygon]=data_polygon[k];
		count_1s_polygon++;
/********************************************轴承齿轮踏面区**********************************************/
		if(count_1s_fft==(int)fft_diag_env.fft_diag_paras.Sample_Rate)
		{
			/********************************此处应当用通道号判断是什么数据来了********************************/
			//如果是轴箱数据
			fft_diag_paras.Sample_Rate = 4096;//根据项目情况初始化数据
			fft_diag_paras.bear_diag_speed = 100;//根据项目情况初始化数据
			fft_diag_paras.wheel_diameter = 805;//根据项目情况初始化数据
			fft_diag_ZXZC_Wheel_init(&fft_diag_paras);
			status_bearing_wheel=ZXZC_Wheel_diag(data_1s_fft, (int)fft_diag_paras.Sample_Rate, fft_diag_paras, &fft_diag_save,\
				block_3s_ZXZC_N, block_3s_ZXZC_W, block_3s_ZXZC_G , block_3s_ZXZC_B, block_3s_Wheel);
/*****************************状态输出*****************************/
			status_bearing_ZX = status_bearing_wheel.status_bearing_comprehensive;
			status_wheel = status_bearing_wheel.status_Wheel;
/*****************************DB值输出*****************************/
//			ZXZC_DB_N = fft_diag_save.DB[0];			//轴箱轴承DB值
//			ZXZC_DB_W = fft_diag_save.DB[1];
//			ZXZC_DB_G = fft_diag_save.DB[2];
//			ZXZC_DB_B = fft_diag_save.DB[3];
//			Wheel_DB = fft_diag_save.DB[4];
/*****************************amp值输出*****************************/
//			ZXZC_amp_N=fft_diag_env.fft_diag_save.amp[0];		//轴箱轴承amp值
//			ZXZC_amp_W=fft_diag_env.fft_diag_save.amp[1];
//			ZXZC_amp_G=fft_diag_env.fft_diag_save.amp[2];
//			ZXZC_amp_B=fft_diag_env.fft_diag_save.amp[3];			
//			Wheel_amp=fft_diag_env.fft_diag_save.amp[4];
/*****************************fre值输出*****************************/
//			ZXZC_fre_N=fft_diag_env.fft_diag_save.fre[0];		//轴箱轴承fre值
//			ZXZC_fre_W=fft_diag_env.fft_diag_save.fre[1];
//			ZXZC_fre_G=fft_diag_env.fft_diag_save.fre[2];
//			ZXZC_fre_B=fft_diag_env.fft_diag_save.fre[3];		
//			Wheel_fre=fft_diag_env.fft_diag_save.fre[4];
		
			//如果是齿轮箱数据
			fft_diag_CLX_init((uint16_t)speed);
/*****************************状态输出*****************************/
			status_bearing_CLX=CLX_diag(data_1s_fft, (int)fft_diag_paras.Sample_Rate, fft_diag_paras, &fft_diag_save,\
				block_3s_CLX_N, block_3s_CLX_W, block_3s_CLX_G , block_3s_CLX_B);
/*****************************DB值输出*****************************/
			CLX_DB_N_DJ_side=fft_diag_save.DB[0];			//齿轮箱电机侧轴承DB值
			CLX_DB_W_DJ_side=fft_diag_save.DB[1];
			CLX_DB_G_DJ_side=fft_diag_save.DB[2];
			CLX_DB_B_DJ_side=fft_diag_save.DB[3];
			
			CLX_DB_N_Wheel_side=fft_diag_save.DB[4];			//齿轮箱车轮侧轴承DB值
			CLX_DB_W_Wheel_side=fft_diag_save.DB[5];
			CLX_DB_G_Wheel_side=fft_diag_save.DB[6];
			CLX_DB_B_Wheel_side=fft_diag_save.DB[7];
/*****************************amp值输出*****************************/
			CLX_amp_N_DJ_side=fft_diag_save.amp[0];			//齿轮箱电机侧轴承amp值
			CLX_amp_W_DJ_side=fft_diag_save.amp[1];
			CLX_amp_G_DJ_side=fft_diag_save.amp[2];
			CLX_amp_B_DJ_side=fft_diag_save.amp[3];
			
			CLX_amp_N_Wheel_side=fft_diag_save.amp[4];			//齿轮箱车轮侧轴承amp值
			CLX_amp_W_Wheel_side=fft_diag_save.amp[5];
			CLX_amp_G_Wheel_side=fft_diag_save.amp[6];
			CLX_amp_B_Wheel_side=fft_diag_save.amp[7];
/*****************************fre值输出*****************************/
			CLX_fre_N_DJ_side=fft_diag_save.fre[0];			//齿轮箱电机侧轴承fre值
			CLX_fre_W_DJ_side=fft_diag_save.fre[1];
			CLX_fre_G_DJ_side=fft_diag_save.fre[2];
			CLX_fre_B_DJ_side=fft_diag_save.fre[3];
			
			CLX_fre_N_Wheel_side=fft_diag_save.fre[4];			//齿轮箱车轮侧轴承fre值
			CLX_fre_W_Wheel_side=fft_diag_save.fre[5];
			CLX_fre_G_Wheel_side=fft_diag_save.fre[6];
			CLX_fre_B_Wheel_side=fft_diag_save.fre[7];
			
			//如果是电机数据
/*****************************状态值输出*****************************/
			fft_diag_DJ_init((uint16_t)speed);
			status_bearing_DJ=CLX_diag(data_1s_fft, (int)fft_diag_paras.Sample_Rate, fft_diag_paras, &fft_diag_save,\
				block_3s_DJ_N, block_3s_DJ_W, block_3s_DJ_G , block_3s_DJ_B);		
/*****************************DB值输出*****************************/
			DJ_DB_N_FCD_side=fft_diag_save.DB[0];		//电机非传动端轴承DB值
			DJ_DB_W_FCD_side=fft_diag_save.DB[1];
			DJ_DB_G_FCD_side=fft_diag_save.DB[2];
			DJ_DB_B_FCD_side=fft_diag_save.DB[3];
			
			DJ_DB_N_CD_side=fft_diag_save.DB[4];			//电机传动端轴承DB值
			DJ_DB_W_CD_side=fft_diag_save.DB[5];
			DJ_DB_G_CD_side=fft_diag_save.DB[6];
			DJ_DB_B_CD_side=fft_diag_save.DB[7];
/*****************************amp值输出*****************************/
			DJ_amp_N_FCD_side=fft_diag_save.amp[0];	//电机非传动端轴承amp值
			DJ_amp_W_FCD_side=fft_diag_save.amp[1];
			DJ_amp_G_FCD_side=fft_diag_save.amp[2];
			DJ_amp_B_FCD_side=fft_diag_save.amp[3];
			
			DJ_amp_N_CD_side=fft_diag_save.amp[4];		//电机传动端侧轴承amp值
			DJ_amp_W_CD_side=fft_diag_save.amp[5];
			DJ_amp_G_CD_side=fft_diag_save.amp[6];
			DJ_amp_B_CD_side=fft_diag_save.amp[7];
/*****************************fre值输出*****************************/
			DJ_fre_N_FCD_side=fft_diag_save.fre[0];	//电机非传动端侧轴承fre值
			DJ_fre_W_FCD_side=fft_diag_save.fre[1];
			DJ_fre_G_FCD_side=fft_diag_save.fre[2];
			DJ_fre_B_FCD_side=fft_diag_save.fre[3];
			
			DJ_fre_N_CD_side=fft_diag_save.fre[4];		//电机传动端轴承fre值
			DJ_fre_W_CD_side=fft_diag_save.fre[5];
			DJ_fre_G_CD_side=fft_diag_save.fre[6];
			DJ_fre_B_CD_side=fft_diag_save.fre[7];
			//如果是齿轮数据使用齿轮振动数据
			fft_diag_gear_init((uint16_t)speed);
/*****************************状态值输出*****************************/
			status_gear=gear_diag(data_1s_fft, (int)fft_diag_paras.Sample_Rate, fft_diag_paras, &fft_diag_save,\
				block_3s_gear);		
/*****************************DB值输出*****************************/
			gear_DB=fft_diag_save.DB[0];			//齿轮箱电机侧轴承DB值
/*****************************amp值输出*****************************/
			gear_amp=fft_diag_save.amp[0];
/*****************************fre值输出*****************************/
			gear_fre=fft_diag_save.fre[0];
				
			count_1s_fft=0;  //计数器更新
		}
		/********************************************多边形区**********************************************/
		//获取滤波数据
		data_1s_polygon[count_1s_polygon]=IIR_filter_polygon(data_polygon[k]);//跟新1秒数据
		count_1s_polygon++;
		if(count_1s_polygon==(int)polygon_para.Sample_Rate)
		{
			polygon_diag(data_1s_polygon, (int)polygon_para.Sample_Rate, polygon_para, &polygon_save, N_stack, K_stack, speed_stack);
			count_1s_polygon=0;
			N1=polygon_save.order_result;
			rms1=polygon_save.rms_result;
			DB1=polygon_save.DB_result;
			K1=polygon_save.indicator;
			nameda1=polygon_save.nameda_result;
			status1=polygon_save.status_polygon;
			speed1=polygon_save.speed;
		}	
		/********************************************波磨区**********************************************/
		//下面是波磨的程序，K1到K8是同车8个踏面位置算出来的多边形指标,nameda1到nameda8是同车8个踏面位置算出来的波长指标
			corrugation_diag(K1,K2,K3,K4,K5,K6,K7,K8,\
			nameda1, nameda2, nameda3, nameda4, nameda5, nameda6, nameda7, nameda8, &corrugation_save);//拉姆达 波长
			corrugation_K1357=corrugation_save.corrugation_indicator1357_result;  //1 3 5 7侧钢轨的波磨指标
			corrugation_K2468=corrugation_save.corrugation_indicator2468_result;  //2 4 6 8侧钢轨的波磨指标

			corrugation_nameda1357=corrugation_save.corrugation_nameda1357_result;  //1 3 5 7侧钢轨的波磨指标
			corrugation_nameda2468=corrugation_save.corrugation_nameda2468_result;  //2 4 6 8侧钢轨的波磨指标
	}
}

#endif /*#if 0*/


