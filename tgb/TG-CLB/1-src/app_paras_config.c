/**
 * @file
 * @details  System startup parameters. Other parameters are not 
             recommended to be saved in STM32 internal flash
 * @author   ren
 * @date     2021.09
 * @version  V1.0
**/

/* include */
#include "app_paras_config.h"
#include "dev_config.h"
#include "util.h"

/* macro */
#define APP_PARAS_CONFIG_IP     "255.255.255.255"
#define APP_PARAS_FACTORY_CODE  0x04
#define APP_PARAS_TGB_CODE      0x44
#define APP_PARAS_PTU_CODE      0xFF
#define APP_PARAS_MCU_SN_ADD    ((uint32_t)0x1FFF7A10)

/* type declaration */

/* variable declaration */
app_paras_config_env_t app_paras_config_env;
#define env app_paras_config_env

/* function declaration */
/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_paras_config_get_mcu_sn(void)
{
    for(uint8_t i = 0; i < 3; i++)
    {
        env.mcu_sn[i] = (*(uint32_t *)(APP_PARAS_MCU_SN_ADD + (4 * i)));
		printf("\tmcu_sn[%d] = 0x%02X \n", i, env.mcu_sn[i]);
	}
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_paras_config_msg_send(uint8_t *data, uint16_t size)
{
	extern int inside_send_fd_socket;
	struct sockaddr_in send_ipaddr;
	
	memset(&send_ipaddr, 0, sizeof(send_ipaddr));
    send_ipaddr.sin_family = AF_INET;
    send_ipaddr.sin_addr.s_addr = inet_addr(APP_PARAS_CONFIG_IP);//inet_addr(env.config_net_inform.config_ip);
    send_ipaddr.sin_port = htons(env.config_net_inform.port);
	sendto(inside_send_fd_socket, data, size, 0, (struct sockaddr *)&send_ipaddr, sizeof(send_ipaddr));
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static inline void app_paras_config_broadcast_ack(void)
{
//	paras_t *paras = RT_NULL;
	broadcast_ack_protocol_t broadcast_ack_data;
	/*
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
	uint8_t device_code;
	uint8_t sn[2];
	uint8_t local_ip[4];
	uint8_t mask[4];
	uint8_t gateway[4];
	uint8_t port[2];
	uint8_t train_id;
	uint8_t reserve[4];
}broadcast_ack_cmd_t, *pbroadcast_ack_cmd_t; //APP_BROADCAST_ACK_CMD
	*/
	
	memset(&broadcast_ack_data, 0, sizeof(broadcast_ack_protocol_t));
//	paras = app_paras_get();
	broadcast_ack_data.data_head[0] = 0xAA;
	broadcast_ack_data.data_head[1] = 0x51;
	broadcast_ack_data.data_len[0] = (uint8_t)(sizeof(broadcast_ack_protocol_t) >> 8);
	broadcast_ack_data.data_len[1] = (uint8_t)(sizeof(broadcast_ack_protocol_t));
	broadcast_ack_data.factory_code = APP_PARAS_FACTORY_CODE;
	broadcast_ack_data.device_code = APP_PARAS_TGB_CODE;
	broadcast_ack_data.cmd = APP_BROADCAST_ACK_CMD;
	pbroadcast_ack_cmd_t broadcast_ack_inform;
	broadcast_ack_inform  = (pbroadcast_ack_cmd_t)broadcast_ack_data.data_inform;
	broadcast_ack_inform->device_code = APP_PARAS_TGB_CODE;
	broadcast_ack_inform->sn[0] = (uint8_t)(env.mcu_sn[0]);
	broadcast_ack_inform->sn[1] = (uint8_t)(env.mcu_sn[1]);
	app_paras_net_config(comm_para.ip, broadcast_ack_inform->local_ip);
	app_paras_net_config(comm_para.mask, broadcast_ack_inform->mask);
	app_paras_net_config(comm_para.gw, broadcast_ack_inform->gateway);
	broadcast_ack_inform->port[0] = (uint8_t)(comm_para.port >> 8);
	broadcast_ack_inform->port[1] = (uint8_t)(comm_para.port);
	broadcast_ack_inform->train_id = comm_para.train_id;
	uint16_t sum = sum_check_16((uint8_t *)&broadcast_ack_data, sizeof(broadcast_ack_protocol_t) - 2);
	broadcast_ack_data.sun_crc[0] = (uint8_t)(sum >> 8);
	broadcast_ack_data.sun_crc[1] = (uint8_t)(sum);

	app_paras_config_msg_send((uint8_t *)&broadcast_ack_data, sizeof(broadcast_ack_protocol_t));
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static inline void app_paras_config_ack(uint8_t state)
{
	config_ack_protocol_t config_ack_protocol;
	
	memset(&config_ack_protocol, 0, sizeof(config_ack_protocol_t));
	config_ack_protocol.data_head[0] = 0xAA;
	config_ack_protocol.data_head[1] = 0x51;
	config_ack_protocol.data_len[0] = (uint8_t)(sizeof(config_ack_protocol_t) >> 8);
	config_ack_protocol.data_len[1] = (uint8_t)(sizeof(config_ack_protocol_t));
	config_ack_protocol.factory_code = APP_PARAS_FACTORY_CODE;
	config_ack_protocol.device_code = APP_PARAS_TGB_CODE;
	config_ack_protocol.cmd = APP_CONFIG_INFORM_ACK_CMD;
	pconfig_inform_ack_cmd_t config_inform;
	config_inform  = (pconfig_inform_ack_cmd_t)config_ack_protocol.data_inform;
	config_inform->device_code = APP_PARAS_TGB_CODE;
	config_inform->sn[0] = (uint8_t)(env.mcu_sn[0]);
	config_inform->sn[1] = (uint8_t)(env.mcu_sn[1]);
	config_inform->state = state;
	uint16_t sum = sum_check_16((uint8_t *)&config_ack_protocol, sizeof(config_ack_protocol_t) - 2);
	config_ack_protocol.sun_crc[0] = (uint8_t)(sum >> 8);
	config_ack_protocol.sun_crc[1] = (uint8_t)(sum);
	
	app_paras_config_msg_send((uint8_t *)&config_ack_protocol, sizeof(config_ack_protocol_t));
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static inline void app_paras_config_inform(pconfig_inform_cmd_t config_inform)
{
//	extern struct comm_ip_para comm_para;
//	paras_t *paras = RT_NULL;
	
	uint16_t crc_16 = modbus_crc((uint8_t *)config_inform, sizeof(config_inform_cmd_t) - 2);
	if((config_inform->device_code != APP_PARAS_TGB_CODE) ||
	   (config_inform->sn[0] != (uint8_t)(env.mcu_sn[0])) || 
	   (config_inform->sn[1] != (uint8_t)(env.mcu_sn[1])) || 
	   (*(uint16_t *)config_inform->crc16 != crc_16))
	{
		printf("app paras config device_cord error or sn error or crc16 error \n");
		return ;
	}
	else //all check ok
	{
//		paras = app_paras_get();
		if(app_paras_ip_check(config_inform->local_ip))
		{
			printf("app paras config local ip error \n");
			app_paras_config_ack(0);
			return ;
		}
		if(app_paras_mask_check(config_inform->mask))
		{
			printf("app paras config mask error \n");
			app_paras_config_ack(0);
			return ;
		}
		if(app_paras_gateway_check(config_inform->gateway))
		{
			printf("app paras config gateway error \n");
			app_paras_config_ack(0);
			return ;
		}
		if((config_inform->train_id < NET_CONFIG_TRAIN_HEAD) || 
		   (config_inform->train_id > NET_CONFIG_TRAIN_TAIL))
		{
			printf("app paras config train id error \n");
			app_paras_config_ack(0);
			return ;
		}
		sprintf(comm_para.ip,"%d.%d.%d.%d",config_inform->local_ip[0],config_inform->local_ip[1],\
				                           config_inform->local_ip[2],config_inform->local_ip[3]);
		sprintf(comm_para.mask,"%d.%d.%d.%d",config_inform->mask[0],config_inform->mask[1],\
				                             config_inform->mask[2],config_inform->mask[3]);
		sprintf(comm_para.gw,"%d.%d.%d.%d",config_inform->gateway[0],config_inform->gateway[1],\
				                           config_inform->gateway[2],config_inform->gateway[3]);
//		memmove(comm_para.ip, config_inform->local_ip, 4);
//		memmove(paras->netmask, config_inform->mask, 4);
//		memmove(paras->gateway, config_inform->gateway, 4);
		uint16_t port = (config_inform->port[0] << 8) + config_inform->port[1];
		comm_para.port = port;
		comm_para.train_id = config_inform->train_id;
		printf("\t->recv config paras inform: \n");
		display_comm_ini_para(&comm_para);
		uint8_t result = update_comm_para();
		app_paras_config_ack(1);
		printf("\t->reset sys %d \n", result);
	   system("reboot");
	}
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_paras_config_get_data(uint8_t *data, uint16_t size, struct sockaddr_in recv_addr)
{
	pnet_config_protocol_t net_config_data;
	net_config_data = (pnet_config_protocol_t)data;
	
	if((*(uint16_t*)&net_config_data->data_head != htons(0xAA51))) {
		return;
	}
	uint16_t host_sum = sum_check_16(data, size - 2);
	uint16_t net_sum = (data[size - 2] << 8) + data[size - 1];
	if(host_sum != net_sum)
	{
		printf("recv msg crc failed \n");
		return;
	}
	else
	{
		if((net_config_data->factory_code != APP_PARAS_FACTORY_CODE) || 
		   (net_config_data->device_code != APP_PARAS_PTU_CODE))
		{
			return ;
		}
		else
		{
			strcpy(env.config_net_inform.config_ip, inet_ntoa(recv_addr.sin_addr));
			env.config_net_inform.port = htons(recv_addr.sin_port);
			printf("recv config paras inform: %s\n", env.config_net_inform.config_ip);
			pbroadcast_cmd_t broadcast_cmd;
			broadcast_cmd = (pbroadcast_cmd_t)net_config_data->data_inform;
			pconfig_inform_cmd_t config_inform_cmd;
			config_inform_cmd = (pconfig_inform_cmd_t)net_config_data->data_inform;

			switch(net_config_data->cmd)
			{
				case APP_BROADCAST_CMD:
					if(broadcast_cmd->bits.txb == 0x01) {
						app_paras_config_broadcast_ack();
					}
					return ;
				case APP_CONFIG_INFORM_CMD:
					if(config_inform_cmd->target_broad.bits.txb == 0x01) {
						app_paras_config_inform(config_inform_cmd);
					}
					return ;
				default:
					return ;
			}
		}
	}
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_paras_config_init(void)
{
	memset(&env.config_net_inform, 0, sizeof(config_net_inform_t));
	app_paras_config_get_mcu_sn();
}



	
	
	
	
	
	
	
	
	









