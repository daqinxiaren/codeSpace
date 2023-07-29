/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __APP_PARAS_CONFIG_H__
#define __APP_PARAS_CONFIG_H__

/* include */
//#include <../lwip/sockets.h>
#include "stdint.h"
#include "util.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include "net_config_protocol.h"

/* macro */

/* type declaration */
typedef struct
{
	char       config_ip[16];
	uint16_t   port;
}config_net_inform_t;
typedef struct
{
	config_net_inform_t      config_net_inform;
	uint32_t                 mcu_sn[3];
}app_paras_config_env_t;

/* variable declaration */
extern app_paras_config_env_t app_paras_config_env;

/* function declaration */
void app_paras_config_init(void);
void app_paras_config_get_data(uint8_t *data, uint16_t size, struct sockaddr_in recv_addr);

/**
 * @brief
 * @param
 * @return
 * @note
**/


#endif /*__APP_PARAS_CONFIG_H__*/


