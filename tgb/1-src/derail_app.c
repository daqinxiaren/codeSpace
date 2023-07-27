/*
 * derail_app.c
 *
 *  Created on: Mar 1, 2021
 *      Author: linux-ls
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "user_data.h"
#include "derail_app.h"
#include "udp_client.h"
#include "polygon_diag.h"
#include "corrugation_diag.h"
#include "test_data.h"
#include "IIR_polygon.h"
#include "ad7606.h"

struct threshold_para
{
	int amplitude_alarm_num;
	float amplitude_alarm_thteshold;
	int abs_alarm_num;
	float abs_alarm_thteshold;

	float uplift_alarm_thteshold;
};
/*--------------------------- extern para -------------------*/
struct threshold_para para;
struct DERAIL_DAT derail_dat;
struct send_local_st local_st;
extern struct TCMS_PUBLIC_INFO tcms_pub_info;
uint8_t test_flag_g = 0;

static void derail_init(struct DERAIL_CALC *calc)
{
    calc->acc_buf = (float *)malloc( SAMPLE_SIZE* sizeof(float));
    memset(calc->acc_buf, 0, SAMPLE_SIZE * sizeof(float));
    calc->buf_len=SAMPLE_SIZE;
    para.abs_alarm_num=10;
    para.abs_alarm_thteshold=100.0;
    para.amplitude_alarm_num=50;
    para.amplitude_alarm_thteshold=30;
    para.uplift_alarm_thteshold=3;
}

//static void derail_tz_msg_send(tz_send_recode_t *tz)
//{
//	uint8_t send_buff[sizeof(tz_send_recode_t)];
//	memmove(send_buff, tz, sizeof(tz_send_recode_t));
//	inside_mcast_send(send_buff, sizeof(tz_send_recode_t),inside_mcast1_addr);
//
//}

void derail_acc_dat_send(struct AD_PUBLIC_INFO *info,uint16_t* accbuf,int len )
{
	int send_len=sizeof(struct AD_PUBLIC_INFO)+len;

	uint8_t *sendbuf=(uint8_t*)malloc(send_len);
	memmove(sendbuf, info, sizeof(struct AD_PUBLIC_INFO));
	memmove(&sendbuf[sizeof(struct AD_PUBLIC_INFO)], accbuf, len);

	//client_net_send(UDP,0, TRAIN1_MCU, DERAIL_RAW_DATA_TYPE, sendbuf, send_len);
}

/*采样数据转换电压*/
static float calculate_Acc_voltage(uint16_t value)
{
//    return (value - 32768.0) / 65536.0 * 20.0;
    return (float)((value-32768)/65536.0*20/0.02)/1.25/1.18; //注：最后除以1.18针对脱轨板AD芯片输出问题，同样输入情况下，脱轨板采集输出比处理板输出幅值高
}

void derail_thread_entry()
{
	struct DERAIL_CALC calc;

	struct POLYGON_DIAG_PARA polygon_para = {0};
	struct POLYGON_DIAG_SAVE polygon_diag_save = {0};
	float indicator[DIAG_CHANNEL_NUM];
	float rail_corrugation[DIAG_CHANNEL_NUM];
	struct CORRUGATION_DIAG_SAVE corrugation_diag_save = {0};//波磨诊断结果
	polygon_diag_stack_t polygon_stack[8] = {0};
	uint16_t wheel = 0;
	uint16_t speed = 0;
	float x[9] = {0};
	float y[9] = {0};

	derail_init(&calc);
	sem_init(&derail_dat.derail_sem,0,0);

#ifdef RT_LINUX
    pthread_attr_t attr;       // 线程属性
    struct sched_param sched;  // 调度策略
    printf ("set SCHED_RR policy\n");
    api_set_thread_policy(&attr, SCHED_RR);
#endif
	while(1)
	{
		sem_wait(&derail_dat.derail_sem);
		{
		    memset(&local_st.tz_data, 0, sizeof(tz_send_recode_t));

		    if (test_flag_g)
		    {
				//模拟公共信息参数,用于算法测试
				public_msg.speed[0] = 0x27;
				public_msg.speed[1] = 0x10;
				public_msg.wheel[0] = 0x03;
				public_msg.wheel[1] = 0x24;
		    }
			wheel = (uint16_t)public_msg.wheel[0]<<8 | public_msg.wheel[1];
			speed = (uint16_t)public_msg.speed[0]<<8 | public_msg.speed[1];
			if(tcms_pub_info.valid_flag.bits.wheel_valid || tcms_pub_info.valid_flag.bits.speed_valid)
			{
				local_st.diag_valid_flag = 1;
			}
			local_st.speed[0] = public_msg.speed[0];
			local_st.speed[1] = public_msg.speed[1];
			//多边形诊断参数配置
			polygon_para.wheel_diameter = (float)wheel/1000;//单位 m    轮径，默认是0.805
			polygon_para.Sample_rate = DIAG_RATE;			//采样频率
			polygon_para.polygon_diag_speed = (float)speed/100;   // km/h 车辆运行的速度
			polygon_para.threshold_prognosis = DIAG_THRESHOLD_PROGNOSIS;
			polygon_para.threshold_prewarning = DIAG_THRESHOLD_PREWARNING;
			polygon_para.threshold_warning = DIAG_THRESHOLD_WARNING;
			polygon_para.time_length = DIAG_STACK_LEN;
//			float test[20] = {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10};
			//printf("sem_wait---\n");

			for(int t=0;t<DIAG_CHANNEL_NUM;t++)
			{
			    memset(x, 0, sizeof(float)*9);
			    memset(y, 0, sizeof(float)*9);

				for(int cnt_i = 0;cnt_i < SAMPLE_SIZE;cnt_i++)
				{
					//将采集数据转换为加速度并滤波后存到准备计算的buff中
					calc.acc_buf[cnt_i] =IIR_filter_polygon(calculate_Acc_voltage(derail_dat.data_buf[t][cnt_i]),x,y);
				}
				if (test_flag_g)
				{
					if (0 == t)
					{
						memset(x, 0, sizeof(float)*9);
						memset(y, 0, sizeof(float)*9);
						for(int cnt_i = 0;cnt_i < SAMPLE_SIZE;cnt_i++)
						{
							calc.acc_buf[cnt_i] = IIR_filter_polygon(prewarning0[cnt_i],x,y);
//							if (cnt_i<20)  printf(" %f+%f ",prewarning0[cnt_i],calc.acc_buf[cnt_i]);
						}
						printf("\n");
					}
				}

				polygon_diag(calc.acc_buf,polygon_para,&polygon_diag_save,&polygon_stack[t]);

				indicator[t] = polygon_diag_save.indicator;//保存到数组中用于波磨计算
				rail_corrugation[t] = polygon_diag_save.nameda_result;//保存到数组中用于波磨计算
                //保存多边形TZ数据到发送buff中
				local_st.tz_data.polygon_tz[t].TZ1_order_h = polygon_diag_save.order_result>>8&0x000000ff;
				local_st.tz_data.polygon_tz[t].TZ1_order_l = polygon_diag_save.order_result&0x000000ff;
				local_st.tz_data.polygon_tz[t].TZ2_rms_h = (int)(polygon_diag_save.rms_result*100)>>8&0x000000ff;
				local_st.tz_data.polygon_tz[t].TZ2_rms_l = (int)(polygon_diag_save.rms_result*100)&0x000000ff;
				local_st.tz_data.polygon_tz[t].TZ3_DB_h = (int)(polygon_diag_save.DB_result*100)>>8&0x000000ff;
				local_st.tz_data.polygon_tz[t].TZ3_DB_l = (int)(polygon_diag_save.DB_result*100)&0x000000ff;
				local_st.tz_data.polygon_tz[t].diag_res = polygon_diag_save.status_polygon&0x000000ff;
				local_st.tz_data.polygon_tz[t].channel_id = t+1;
#if 1
				printf("polygon_para---> wheel=%f, speed=%f\n",
						polygon_para.wheel_diameter,polygon_para.polygon_diag_speed);
				printf("polygon diag result: --->\n    order=%d, rms=%f, DB=%f, status=%d, speed=%f\n    k = %d n = %f\n",
						polygon_diag_save.order_result,polygon_diag_save.rms_result,
						polygon_diag_save.DB_result,polygon_diag_save.status_polygon,polygon_diag_save.speed,
						polygon_diag_save.indicator, polygon_diag_save.nameda_result);
#endif
			}
			//下面是波磨的程序，K1到K8是同车8个踏面位置算出来的多边形指标,nameda1到nameda8是同车8个踏面位置算出来的波长指标
			corrugation_diag(indicator,rail_corrugation, &corrugation_diag_save);
			//保存波磨TZ数据到发送buff中
			local_st.tz_data.wave_tz.indi1357_result_h = (int)(corrugation_diag_save.indicator1357_result*100)>>8&0x000000ff;
			local_st.tz_data.wave_tz.indi1357_result_l = (int)(corrugation_diag_save.indicator1357_result*100)&0x000000ff;
			local_st.tz_data.wave_tz.indi2468_result_h = (int)(corrugation_diag_save.indicator2468_result*100)>>8&0x000000ff;
			local_st.tz_data.wave_tz.indi2468_result_l = (int)(corrugation_diag_save.indicator2468_result*100)&0x000000ff;
			local_st.tz_data.wave_tz.nameda1357_result_h = (int)(corrugation_diag_save.nameda1357_result*100)>>8&0x000000ff;
			local_st.tz_data.wave_tz.nameda1357_result_h = (int)(corrugation_diag_save.nameda1357_result*100)&0x000000ff;
			local_st.tz_data.wave_tz.nameda2468_result_h = (int)(corrugation_diag_save.nameda2468_result*100)>>8&0x000000ff;
			local_st.tz_data.wave_tz.nameda2468_result_h = (int)(corrugation_diag_save.nameda2468_result*100)&0x000000ff;
			//send net
			tz_msg_send(&local_st);
			printf("send tz to record\n");
		}
	}
}

/*************************************************
Function:    init_polygon_thread
Description: 初始化轮对多边形诊断线程
Input:
Output:
Return: 成功：０
		失败：非０
Others:
*************************************************/
int init_derail_thread(void)
{
	pthread_t derail_thread_id;

	int ret;
	ret=pthread_create(&derail_thread_id,NULL,(void*)derail_thread_entry,NULL);
	if(ret!=0)
	{
		printf("Create self check thread error!\n");
		return ret;
	}

	return 0;
}
