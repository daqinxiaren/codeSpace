/**
 * @file
 * @details
 * @author
 * @date    配置IP信息的协议
 * @version
**/

#ifndef __NET_CONFIG_PROTOCOL_H__
#define __NET_CONFIG_PROTOCOL_H__

/* include */
#include "stdint.h"

/* macro */
#define NET_CONFIG_TRAIN_HEAD     1
#define NET_CONFIG_TRAIN_TAIL     6

/* type declaration */
typedef enum
{
	APP_BROADCAST_CMD         = 0x01,
	APP_BROADCAST_ACK_CMD     = 0x02,
	APP_CONFIG_INFORM_CMD     = 0x03,
	APP_CONFIG_INFORM_ACK_CMD = 0x04,
}net_config_cmd_e;
typedef union
{
	struct
	{
		uint8_t record   :1;
		uint8_t txb      :1;
		uint8_t clb      :1;
		uint8_t tclb1    :1;
		uint8_t tclb2    :1;
		uint8_t zxb      :1;
		uint8_t reserve1 :2;
		
		uint8_t reserve2 :8;
	}bits;
	uint8_t byte[8];
}broadcast_cmd_t, *pbroadcast_cmd_t; //APP_BROADCAST_CMD
typedef struct
{
	uint8_t device_code;
	uint8_t sn[2];
	uint8_t local_ip[4];
	uint8_t mask[4];
	uint8_t gateway[4];
	uint8_t port[2];
	uint8_t train_id;
	uint8_t reserve[4];
}broadcast_ack_cmd_t, *pbroadcast_ack_cmd_t; //APP_BROADCAST_ACK_CMD
typedef union
{
	struct
	{
		uint8_t record   :1;
		uint8_t txb      :1;
		uint8_t clb      :1;
		uint8_t tclb1    :1;
		uint8_t tclb2    :1;
		uint8_t zxb      :1;
		uint8_t reserve1 :2;
	}bits;
	uint8_t byte;
}target_broad_t, *ptarget_broad_t;
typedef struct
{
	target_broad_t target_broad;
	uint8_t        device_code;
	uint8_t        sn[2];
	uint8_t        local_ip[4];
	uint8_t        mask[4];
	uint8_t        gateway[4];
	uint8_t        port[2];
	uint8_t        train_id;
	uint8_t        reserve[4];
	uint8_t        crc16[2];
}config_inform_cmd_t, *pconfig_inform_cmd_t; //APP_CONFIG_INFORM_CMD
typedef struct
{
	uint8_t device_code;
	uint8_t sn[2];
	uint8_t state;
	uint8_t reserve;
}config_inform_ack_cmd_t, *pconfig_inform_ack_cmd_t; //APP_CONFIG_INFORM_ACK_CMD
typedef struct
{
	uint8_t data_head[2];
	uint8_t data_len[2];
	uint8_t factory_code;
	uint8_t device_code;
	uint8_t reserve[5];
	uint8_t cmd;
	uint8_t data_inform[];
}net_config_protocol_t, *pnet_config_protocol_t;

typedef struct
{
	uint8_t data_head[2];
	uint8_t data_len[2];
	uint8_t factory_code;
	uint8_t device_code;
	uint8_t reserve[5];
	uint8_t cmd;
	uint8_t data_inform[sizeof(broadcast_ack_cmd_t)];
	uint8_t sun_crc[2];
}broadcast_ack_protocol_t, *pbroadcast_ack_protocol_t;

typedef struct
{
	uint8_t data_head[2];
	uint8_t data_len[2];
	uint8_t factory_code;
	uint8_t device_code;
	uint8_t reserve[5];
	uint8_t cmd;
	uint8_t data_inform[sizeof(config_inform_ack_cmd_t)];
	uint8_t sun_crc[2];
}config_ack_protocol_t, *pconfig_ack_protocol_t;

#endif /*__NET_CONFIG_PROTOCOL_H__*/





