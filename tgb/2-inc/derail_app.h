/*
 * derail_app.h
 *
 *  Created on: Mar 1, 2021
 *      Author: linux-ls
 */
#ifndef _DERAIL_APP_H_
#define _DERAIL_APP_H_

#include <semaphore.h>
#include "spi_app.h"
#include "derail_diag.h"

#define DIAG_CHANNEL_NUM          8        //脱轨采集的轴箱数
#define DIAG_RATE                 4096
#define DIAG_STACK_LEN            10
#define DIAG_THRESHOLD_PROGNOSIS  17
#define DIAG_THRESHOLD_PREWARNING 22
#define DIAG_THRESHOLD_WARNING    27

typedef struct //8 byte
{
	uint8_t channel_id;		//通道号1-8，对应轴箱轴承位编号
	uint8_t diag_res;	   //诊断结果（报警状态）		RUNNING_OK=0x00,TEMP_WARN=0x01,TEMP_ALARM=0x02,ACC_ALARM_FIRST_GRADE=0x03,ACC_ALARM_SECOND_GRADE=0x04
	uint8_t TZ1_order_h;	//特征数据1  多边形阶次
	uint8_t TZ1_order_l;
	uint8_t TZ2_rms_h;		//特征数据2  加速度均方根(不存)
	uint8_t TZ2_rms_l;
	uint8_t TZ3_DB_h;		//特征数据3  DB值
	uint8_t TZ3_DB_l;
}polygon_tz_t;

typedef struct //8 byte
{
	uint8_t indi1357_result_h;       //1357位车轮对应钢轨的波磨指标
	uint8_t indi1357_result_l;
	uint8_t indi2468_result_h;       //2468位车轮对应钢轨的波磨指标
	uint8_t indi2468_result_l;
	uint8_t nameda1357_result_h;     //1357位车轮对应钢轨的波长指标
	uint8_t nameda1357_result_l;
	uint8_t nameda2468_result_h;     //2468位车轮对应钢轨的波长指标
	uint8_t nameda2468_result_l;
}wave_tz_t;

//用于存储特征数据的每个的公共部分
typedef struct //24 byte
{
	uint8_t data_head_h;     //报文头H: 0x66
	uint8_t data_head_l;     //报文头L: 0xbb
	uint8_t ad_serial_num_h; //串号
	uint8_t ad_serial_num_l; //串号
	uint8_t data_len_h;      //报文总长度H
	uint8_t data_len_l;      //报文总长度L
	uint8_t pack_count_h;    //包计数H，顺序计数
	uint8_t pack_count_l;    //包计数L，顺序计数
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t train_id;
	uint8_t proc_id;
	uint8_t train_number;
	uint8_t data_type;
	uint8_t curr_station_id_h;
	uint8_t curr_station_id_l;
	uint8_t next_station_id_h;
	uint8_t next_station_id_l;
	uint8_t wheel_h;
	uint8_t wheel_l;
}app_diag_save_head_t;

//需要保存的多边形及波磨特征数据的结构类型
typedef struct
{
	polygon_tz_t polygon_tz[8]; //对应8位轴箱的多边形特征值
	wave_tz_t wave_tz;          //8个轴箱算出的波磨特征值
	uint8_t valid_channel_num;  //1-8标出有效数据对应的通道，若此字段为0，代表所有通道数据都有效
	uint8_t reserve[7];
}tz_send_recode_t;

struct DERAIL_CALC
{
	float *acc_buf;
	uint16_t buf_len;
	uint8_t peak[DERAIL_N][DERAIL_DATA_NUM];
	uint8_t abs[DERAIL_N];
	float uplift[SAMPLE_SIZE];
};
#endif /* 2_INC_DERAIL_APP_H_ */
