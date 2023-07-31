/*
 * udp_client.h
 *
 *  Created on: 2021年3月5日
 *      Author: root
 */

#ifndef _UDP_CLIENT_H_
#define _UDP_CLIENT_H_
#include <stdint.h>
#include "derail_app.h"

#define LOCAL_BOARD TCLB1_BOARD //


#define REC_BOARD_ADDR "192.168.1.11"
#define TXB_BOARD_ADDR "192.168.1.12"
#define CLB_BOARD_ADDR "192.168.1.13"
#define TCLB1_BOARD_ADDR "192.168.1.14"
#define TCLB2_BOARD_ADDR "192.168.1.15"
#define ZXB_BOARD_ADDR "192.168.1.16"
#define INSIDE_NET_PORT 8000

#define INSIDE_MCAST1_ADDR "239.255.10.1"//内部组播1用于发送公共信息到子板卡
#define INSIDE_MCAST2_ADDR "239.255.10.2"//内部组播2用于接受内部子板卡的信息

#define INSIDE_MCAST1_PORT 9001
#define INSIDE_MCAST2_PORT 9002

#define RSYNC_UPDATE_GET "ate_get.s/usr/local/sync_updh"
#define RSYNC_UPDATE_PUT "/usr/local/sync_update_put.sh"

#define REC_BOARD 0x11  	// 0x11：记录板
#define TXB_BOARD 0x22		// 0x22：通信版
#define CLB_BOARD 0x33		// 0x33：轴承处理板
#define TCLB1_BOARD 0x44	// 0x44：脱轨板1
#define TCLB2_BOARD 0x55	// 0x55：脱轨板2
#define ZXB_BOARD 0x66		// 0x66：转速总线板
#define PTU_BOARD 0Xff		//ptu平台

#define PROT_HEAD_LEN 26 //head+sumcheck
#define LOCAL_TRAIN_NUM 1//本车车厢号


/*recv/send单元eth数据头*/
struct SEND_DATA_HEAD
{
    uint8_t head[2];               //"0xAA50：过程数据包0xAA51-0xAA54：原始数据包（根据实际增减）
    uint8_t len[2];                //振动过程数据为360，失稳数据为256 平稳为256 原始为1024 轴温为256
    uint8_t company_id;            //板卡供应商编号 参考供应商定义
    uint8_t board_id;              //本身板卡编号 参考宏定义
    uint8_t life_signal[2];        //生命信号，每秒加1
    uint8_t target_board_group[2]; //目标板卡的位集合
    uint8_t resend_flag;           //"0x55：表示首次发送该包数据，0xAA：表示重发该包数据；重发时的数据与首次发送的数据需全部一样，且仅对未应答的单板重发数据（通过Byte8-9来选型），超时时间为300ms，最多重发3次。"
    uint8_t ack_flag;              //"0x5A:目标板需要返回给请求板收到一包数据的应答帧   0x00:无需应答其它无效"
    uint8_t packet_num;            //当前数据类型发送的总包数
    uint8_t res[11];               //预留
};

/*内部网络命令相关定义*/
enum INSIDE_NET_CMD
{
	APP_UPDATE_CMD = 0x01,
	SYSTEM_REBOOT_CMD = 0x02,
	CONFIG_PARA_SET_CMD = 0x03,
	CONFIG_PARA_READ_CMD = 0x04,
	CONFIG_DERAIL_CMD = 0x0A,
	CONFIG_SAFE_CMD = 0x0B,
	CONFIG_SPEED_CMD = 0x0C,
	SYNC_DATA_CMD  = 0x55,//主动同步数据
};

//temp
/*公共信息 列控获取*/
struct TCMS_PUBLIC_INFO
{
	uint8_t ncc[10];
	union VALID_FLAGS
	{
		struct
		{
			uint8_t time_valid:1;//时间有效
			uint8_t wheel_valid:1;//轮径值有效
			uint8_t speed_valid:1;//速度有效
			uint8_t PA_valid:1;
			uint8_t ATC_valid:1;
			uint8_t speed_0:1;
			uint8_t rcc:2;
		}bits;
		uint8_t byte;//
	}valid_flag;

	uint8_t year;//年 数值范围：00～99，10进制
	uint8_t mon;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;

	uint8_t train_line[2];//线路号
	uint8_t train_num[2];//列车号
	uint8_t wheel[16];

	uint8_t speed[2];//速度 1=0.01km/h
	uint8_t curr_id[2];//
	uint8_t next_id[2];//
	uint8_t ncc2[7];
};

struct version_t
{
	uint8_t soft_version[2];
	uint8_t hard_version[2];
	uint8_t date[4];
};

union sys_dev
{
	struct
	{
		uint8_t v1_err:1;//
		uint8_t v2_err:1;//
		uint8_t ad_err:1;
		uint8_t save_err:1;//
		uint8_t derail_del:1;
		uint8_t speed:1;//零速输入
		uint8_t rcc:2;
	}bits;
	uint8_t byte;//
};

union dev_check
{
	struct
	{
		uint8_t sf_signal_output:1;//
		uint8_t warn_signal_output:1;//
		uint8_t zero_speed_signal:1;//
		uint8_t alarm_output_enalbe:1;//
		uint8_t rcc :4;
	}bits;
	uint8_t byte;//
};

union derail_alarm
{
	struct
	{
		uint8_t ch1:1;//
		uint8_t ch2:1;//
		uint8_t ch3:1;//
		uint8_t ch4:1;//
		uint8_t ch5:1;//
		uint8_t ch6:1;//
		uint8_t ch7:1;//
		uint8_t ch8 :1;
	}bits;
	uint8_t byte;//
};

struct send_local_st //100 byte
{
	struct version_t derail_version;
	struct version_t ad_version;
	union sys_dev sys;
	union dev_check check;
	union derail_alarm alarm;
	uint8_t speed[2];  //特征计算的速度值
	uint8_t diag_valid_flag;		//诊断结果是否有效的标志 1:有效 0:无效  诊断无效原因：公共信息中没有收到有效轮径值或速度，因此结果不准确
	uint8_t sensor_check; //8位轴箱传感器自检:1:OK  0:ERR
	tz_send_recode_t tz_data;
	uint8_t nc[4];
};

extern union sys_dev sysdev;
extern int init_udp(void);
extern void ucast_send(uint8_t *buff, uint16_t buff_size,uint8_t target);
extern void mcast_send(uint8_t *buff, uint16_t buff_size,uint8_t target);
extern void tz_msg_send(struct send_local_st *local);
void init_version(struct version_t *version);

#endif /* 2_INC_UDP_CLIENT_H_ */
