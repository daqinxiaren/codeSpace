/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the system_para.c API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2021年2月28日
 *
 * @environment  		: null
 *
 * @description  		: null 
 *
 * @module       		: null
 *
 * \n <b> @NOTE </b> 	: null
 *
 * \b @Warning      		: null
 * 
 * *
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		  kernel       create this file
 * 
 * 
 */
 
/**
 * \addtogroup dev module
 * @{
 */


/**
 * \defgroup module API
 *
 * The xxx API module defines a set of functions that a xxx device
 * driver must implement.
 *
 * @{
 */
 

/**
 ***************** include file ***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
//#include <drv_config.h>
//#include <rtconfig.h>

//#ifdef ENABLE_INIPARSER
#include <dev_config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ezini.h"
#include "util.h"
#include "net_config_protocol.h"

#define rt_kprintf printf

struct comm_ip_para comm_para;
/*
struct comm_ip_para
{
	uint8_t train_id; //车厢号
	char ip[16];      //当前车厢IP地址
	char mask[16];    //当前车厢子网掩码
	char gw[16];		//当前车厢网关
	uint16_t port;//ptu端口
//	char eth1_group1_ip[16];//车厢间通信组播地址1
//	char eth1_group2_ip[16];//车厢间通信组播地址2
//	uint16_t eth1_group1_port;//
//	uint16_t eth1_group2_port;//
};
*/
struct version_para ver_para;

/**
 * xxx note delete
 * sub group
 * 
 * @addtogroup group name 
 */

/**@{*/
/**@}*/

/** @formatter:off */
/**
 ****************<Private macro>****************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/**
 * macro define	- description 
 * @xxx:	description.
 * @xxx:	description.
 * @xxx:	description.
 */

/** @formatter:on */

/**
 ***************<Private define>****************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/**
 **************<Public IMPL functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/**
 * @brief  	解析16个字节的字符串IP地址为4字节的IP
 * @param  	char* ip_16 16字节的ip地址buffer
 * 			char* ip_4  4字节的ip地址buffer
 * @return
 * @note
**/
void app_paras_net_config(char* ip_16, char* ip_4)
{
	int i=0; //遍历16字节的ｉｐ地址
	int num=0; //4字节的ip地址中的数字顺序编号

	memset(ip_4, 0, 4);
	if (NULL==ip_16 || NULL==ip_4)
	{
		printf("app_paras_net_printf para error!\n");
		return;
	}
	for (i=0; i<16; i++)
	{
		if (ip_16[i]>='0' && ip_16[i]<='9')
		{
			ip_4[num] *=10;
			ip_4[num] += ip_16[i];
		}
		else if (ip_16[i]=='.')
		{
			num++;
			if (num>3)
			{
				printf("app_paras_net_config  ip error: %s !!!\n",ip_16);
			}
		}
		else	return;
	}
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
uint8_t app_paras_ip_check(uint8_t *ip)
{
	uint8_t result = 0;

	if((ip[0] != 192) || (ip[1] != 168) || (ip[2]>NET_CONFIG_TRAIN_TAIL) || (ip[2]<NET_CONFIG_TRAIN_HEAD) || (ip[3] != 14)) //192.168.xx.12
	{
		result = 1;
	}
    return result;
}
/**
 * @brief
 * @param
 * @return
 * @note
**/
uint8_t app_paras_gateway_check(uint8_t *gateway)
{
	uint8_t result = 0;

	if((gateway[0] != 192) || (gateway[1] != 168) || (gateway[2]>NET_CONFIG_TRAIN_TAIL) || (gateway[2]<NET_CONFIG_TRAIN_HEAD) || (gateway[3] != 1)) //192.168.xx.1
	{
		result = 1;
	}
    return result;
}
/**
 * @brief
 * @param
 * @return
 * @note
**/
uint8_t app_paras_mask_check(uint8_t *mask)
{
	uint8_t result = 0;

	if((mask[0] != 255) || (mask[1] != 255) || (mask[2] != 0) || (mask[3] != 0)) //255.255.xx.xx
	{
		result = 1;
	}
    return result;
}
/**
 * @brief Read  parse_ptu_para
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
 * @create time  : 2021年2月28日
 * *
 * Parameters
 * *
 * @param	struct comm_ptu_para *ptu_para [in] the pointer of the production fixed is used to obtain the key value
 *
 * @param	ini_entry_t * entry       [in] a structure containing the section, key, and value of INI.
 *
 * @retval
 *				- 0   program execution completed normally
 *				- -1  Program execution error
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		  kernel       create this class
 *
 */
static int parse_ptu_para(struct comm_ip_para *ptu_para,ini_entry_t * entry)
{
	if(!strcmp(KEY_TRAIN_ID,entry->key))
	{
		ptu_para->train_id=srt_to_uint32(entry->value);;
	}
	else if(!strcmp(KEY_IP,entry->key))
	{
		memcpy(ptu_para->ip,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_MASK,entry->key))
	{
		memcpy(ptu_para->mask,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_GW,entry->key))
	{
		memcpy(ptu_para->gw,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_PTU_PORT,entry->key))
	{
		ptu_para->port= srt_to_uint32(entry->value);
	}
//	else if(!strcmp(KEY_ETH1_GROUP1_IP,entry->key))
//	{
//		memcpy(ptu_para->eth1_group1_ip,entry->value,strlen(entry->value));
//	}
//	else if(!strcmp(KEY_ETH1_GROUP2_IP,entry->key))
//	{
//		memcpy(ptu_para->eth1_group2_ip,entry->value,strlen(entry->value));
//	}
//	else if(!strcmp(KEY_ETH1_GROUP1_PORT,entry->key))
//	{
//		ptu_para->eth1_group1_port= srt_to_uint32(entry->value);
//	}
//	else if(!strcmp(KEY_ETH1_GROUP2_PORT,entry->key))
//	{
//		ptu_para->eth1_group2_port= srt_to_uint32(entry->value);
//	}
	else
	{
		rt_kprintf("WARNING:%s should not contain %s\n",COMM_PTU_SECTION_NAME,entry->key);
		return -1;
	}

	return 0;
}


/**
 * @brief Read  parse_br_para
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
 * @create time  : 2021年2月28日
 * *
 * Parameters
 * *
 * @param	struct comm_br_para *br_para [in] the pointer of the production fixed is used to obtain the key value
 *
 * @param	ini_entry_t * entry       [in] a structure containing the section, key, and value of INI.
 *
 * @retval
 *				- 0   program execution completed normally
 *				- -1  Program execution error
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		  kernel       create this class
 *
 */
static int parse_br_para(struct comm_br_para *br_para,ini_entry_t * entry)
{

	if(!strcmp(KEY_BR_ENABLE,entry->key))
	{
		br_para->br_enable=srt_to_uint32(entry->value);
	}
	else if(!strcmp(KEY_BR_IP,entry->key))
	{
		memcpy(br_para->br_ip,entry->value,strlen(entry->value));
	}
	else
	{
		rt_kprintf("WARNING:%s should not contain %s\n",COMM_BR_SECTION_NAME,entry->key);
		return -1;
	}

	return 0;
}
/**
 * @brief  update_comm_para　更新ｉｐ参数
 * @param  无
 */
int update_comm_para(void)
{
	FILE * ini;

	if((ini = fopen(COMM_PARA_FILE_PATH, "w"))==NULL) {
		printf("ERROR: open %s failed!\n",COMM_PARA_FILE_PATH);
		return -1;
	}

	fseek(ini,0,SEEK_SET);
	fprintf(ini,
				"#\n"
				"# This is a comm of ini file\n"
				"#\n"
				"\n"
				"[%s]\n"
				"\n"
				"TRAIN_ID	= %u\n"
				"IP	= %s\n"
				"MASK      = %s\n"
				"GW      = %s\n"
				"PORT      = %u\n"
				"\n"
				"\n",COMM_PTU_SECTION_NAME,comm_para.train_id,
				comm_para.ip,comm_para.mask,
				comm_para.gw,comm_para.port);

	fclose(ini);
	return 0;
}
/**
 * @brief   write_default_comm_para
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
 * @create time  : 2021年2月28日
 * *
 * Parameters
 * *
 * @param	struct comm_ptu_para *ptu_para [in] the pointer of the production fixed is used to obtain the key value
 *
 * @param	ini_entry_t * entry       [in] a structure containing the section, key, and value of INI.
 *
 * @retval
 *				- 0   program execution completed normally
 *				- -1  Program execution error
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		  kernel       create this class
 *
 */
static int write_default_comm_para()
{
	FILE * ini;
	struct comm_ip_para ip_para;
//	struct comm_ini_para comm_para;
//	memset(&comm_para,0,sizeof(struct comm_ini_para));
	memset(&ip_para,0,sizeof(struct comm_ip_para));

	ip_para.train_id=1;
	memcpy(ip_para.ip,"192.168.1.14",16);
	memcpy(ip_para.mask,"255.255.0.0",16);
	memcpy(ip_para.gw,"192.168.1.1",16);
	ip_para.port=8000;
//	memcpy(comm_para.ptu_para.ptu_ip,"10.3.1.120",16);
	//memcpy(comm_para.ptu_para.ptu_ip,"192.168.30.250",16);
//	memcpy(comm_para.ptu_para.ptu_mask,"255.255.0.0",16);
//	memcpy(comm_para.ptu_para.ptu_gw,"10.3.1.1",16);
//	comm_para.ptu_para.ptu_port=9000;
//	memcpy(comm_para.ptu_para.eth1_group1_ip,"239.255.100.1",16);
//	memcpy(comm_para.ptu_para.eth1_group2_ip,"239.255.100.2",16);
//	comm_para.ptu_para.eth1_group1_port=9123;
//	comm_para.ptu_para.eth1_group2_port=9124;

//	comm_para.br_para.br_enable=1;
//	memcpy(comm_para.br_para.br_ip,"192.168.1.100",16);
	if((ini = fopen(COMM_PARA_FILE_PATH, "w"))==NULL) {
			rt_kprintf("ERROR: open %s failed!\n",COMM_PARA_FILE_PATH);
			return -1;
		}

	fseek(ini,0,SEEK_SET);
	fprintf(ini,
				"#\n"
				"# This is a comm of ini file\n"
				"#\n"
				"\n"
				"[%s]\n"
				"\n"
				"TRAIN_ID	= %u\n"
				"IP	= %s\n"
				"MASK      = %s\n"
				"GW      = %s\n"
				"PORT      = %u\n"
				"\n"
				"\n",COMM_PTU_SECTION_NAME,ip_para.train_id,ip_para.ip,ip_para.mask,
				ip_para.gw,ip_para.port);


//		fprintf(ini,
//				"#\n"
//				"# This is a comm of ini file\n"
//				"#\n"
//				"\n"
//				"[%s]\n"
//				"\n"
//				"TRAIN_ID	= %u\n"
//				"PTU_IP	= %s\n"
//				"PTU_MASK      = %s\n"
//				"PTU_GW      = %s\n"
//				"PTU_PORT      = %u\n"
//				"ETH1_GROUP1_IP      = %s\n"
//				"ETH1_GROUP2_IP      = %s\n"
//				"ETH1_GROUP1_PORT      = %u\n"
//				"ETH1_GROUP2_PORT      = %u\n"
//				"\n"
//				"[%s]\n"
//				"\n"
//				"BR_ENABLE      = %u\n"
//				"BR_IP      = %s\n"
//				"\n"
//				"\n",COMM_PTU_SECTION_NAME,comm_para.ptu_para.train_id,comm_para.ptu_para.ptu_ip,comm_para.ptu_para.ptu_mask,
//				comm_para.ptu_para.ptu_gw,comm_para.ptu_para.ptu_port,comm_para.ptu_para.eth1_group1_ip,
//				comm_para.ptu_para.eth1_group2_ip,comm_para.ptu_para.eth1_group1_port,comm_para.ptu_para.eth1_group2_port,
//				COMM_BR_SECTION_NAME,comm_para.br_para.br_enable,comm_para.br_para.br_ip);

			fclose(ini);

		return 0;

}

/**
 * @brief display display_comm_ini_para
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
 * @create time  : 2021年2月28日
 * *
 * Parameters
 * *
 * @param	struct comm_ini_para *comm_para [in] the pointer of the production fixed is used to transfer the key value
 *
 * @retval
 *				- 0   program execution completed normally
 *				- -1  Program execution error
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		  kernel       create this class
 *
 */
void display_comm_ini_para(struct comm_ip_para *comm_para)
{
	printf("[COMM-PARA-CONFIG]:\n");
	printf("\t->train_id = %u\n",comm_para->train_id);

	printf("\t->ptu_ip = %s \n",comm_para->ip);
	printf("\t->ptu_mask = %s \n",comm_para->mask);
	printf("\t->ptu_gw = %s\n",comm_para->gw);
	printf("\t->ptu_port = %u\n",comm_para->port);
//	printf("\t->eth1_group1_ip = %s\n",comm_para->ptu_para.eth1_group1_ip);
//	printf("\t->eth1_group2_ip = %s\n",comm_para->ptu_para.eth1_group2_ip);
//	printf("\t->eth1_group1_port = %u\n",comm_para->ptu_para.eth1_group1_port);
//	printf("\t->eth1_group2_port = %u\n",comm_para->ptu_para.eth1_group2_port);

//	printf("\t->br_enable = %u\n",comm_para->br_para.br_enable);
//	printf("\t->br_ip = %s\n",comm_para->br_para.br_ip);
	printf("\n");

}


/**
 * @brief Read and parse read_comm_ini
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
 * @create time  : 2021年2月28日
 * *
 * Parameters
 * *
 * @param	 path struct comm_ini_para
 *
 * @retval
 *				- 0   program execution completed normally
 *				- -1  Program execution error
 * *
 * @see  reference @file
 *
 * @note 			description
 *							
 * @warning			description 
 *	
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		  kernel       create this class
 *						
 */

int read_comm_ini(struct comm_ip_para* comm_para)
{
	int result;
	FILE *fp;
	ini_entry_t entry;

	if(NULL == comm_para)
	{
		rt_kprintf("ERROR: input point is NULL!\n");
		return -1;
	}

	fp = fopen(COMM_PARA_FILE_PATH, "r");
	if(fp == NULL)
	{
		rt_kprintf("ERROR: File open failed!\n");
		return -1;
	}

	/* initialize entry structure before reading first entry */
	entry.section = NULL;
	entry.key = NULL;
	entry.value = NULL;

	/* read one entry at a time */
	while((result = GetEntryFromFile(fp, &entry)) > 0)
	{
		if(!strcmp(COMM_PTU_SECTION_NAME,entry.section))
		{
			parse_ptu_para(comm_para,&entry);
		}
//		else if(!strcmp(COMM_BR_SECTION_NAME,entry.section))
//		{
//			parse_br_para(&comm_para->br_para,&entry);
//		}
		else
		{
			rt_kprintf("WARNING:%s should not contain %s\n",COMM_PARA_FILE_PATH,entry.section);
		}
	}

	fclose(fp);
	return 0;
}


/**
 * @brief Read  parse_version_para
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
 * @create time  : 2021年2月28日
 * *
 * Parameters
 * *
 * @param	the pointer of the production fixed is used to obtain the key value
 *
 * @param	ini_entry_t * entry       [in] a structure containing the section, key, and value of INI.
 *
 * @retval
 *				- 0   program execution completed normally
 *				- -1  Program execution error
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		  kernel       create this class
 *
 */
static int parse_version_para(struct version_para *ver_para,ini_entry_t * entry)
{

	if(!strcmp(KEY_TRAIN_ID_VERSION,entry->key))
	{
		ver_para->train_id=srt_to_uint32(entry->value);
	}
	//RECORD
	else if(!strcmp(KEY_RECORD_SOFTWARE_VERSION,entry->key))
	{
		memcpy(ver_para->record_version.software_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_RECORD_HARDWARE_VERSION,entry->key))
	{
		memcpy(ver_para->record_version.hard_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_RECORD_OLD_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->record_version.old_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_RECORD_CUR_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->record_version.cur_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_RECORD_PRODUCTION_SERIAL_NUMBER,entry->key))
	{
		memcpy(ver_para->record_version.production_serial_number,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_RECORD_PRODUCTION_TIME,entry->key))
	{
		memcpy(ver_para->record_version.production_time,entry->value,strlen(entry->value));
	}
	//TXB
	else if(!strcmp(KEY_TXB_SOFTWARE_VERSION,entry->key))
	{
		memcpy(ver_para->txb_version.software_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TXB_HARDWARE_VERSION,entry->key))
	{
		memcpy(ver_para->txb_version.hard_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TXB_OLD_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->txb_version.old_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TXB_CUR_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->txb_version.cur_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TXB_PRODUCTION_SERIAL_NUMBER,entry->key))
	{
		memcpy(ver_para->txb_version.production_serial_number,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TXB_PRODUCTION_TIME,entry->key))
	{
		memcpy(ver_para->txb_version.production_time,entry->value,strlen(entry->value));
	}
	//CLB
	else if(!strcmp(KEY_CLB_SOFTWARE_VERSION,entry->key))
	{
		memcpy(ver_para->clb_version.software_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_CLB_HARDWARE_VERSION,entry->key))
	{
		memcpy(ver_para->clb_version.hard_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_CLB_OLD_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->clb_version.old_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_CLB_CUR_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->clb_version.cur_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_CLB_PRODUCTION_SERIAL_NUMBER,entry->key))
	{
		memcpy(ver_para->clb_version.production_serial_number,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_CLB_PRODUCTION_TIME,entry->key))
	{
		memcpy(ver_para->clb_version.production_time,entry->value,strlen(entry->value));
	}

	//	QZQ1
	else if(!strcmp(KEY_QZQ1_SOFTWARE_VERSION,entry->key))
	{
		memcpy(ver_para->qzq1_version.software_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ1_HARDWARE_VERSION,entry->key))
	{
		memcpy(ver_para->qzq1_version.hard_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ1_OLD_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->qzq1_version.old_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ1_CUR_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->qzq1_version.cur_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ1_PRODUCTION_SERIAL_NUMBER,entry->key))
	{
		memcpy(ver_para->qzq1_version.production_serial_number,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ1_PRODUCTION_TIME,entry->key))
	{
		memcpy(ver_para->qzq1_version.production_time,entry->value,strlen(entry->value));
	}

	//	QZQ2
	else if(!strcmp(KEY_QZQ2_SOFTWARE_VERSION,entry->key))
	{
		memcpy(ver_para->qzq2_version.software_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ2_HARDWARE_VERSION,entry->key))
	{
		memcpy(ver_para->qzq2_version.hard_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ2_OLD_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->qzq2_version.old_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ2_CUR_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->qzq2_version.cur_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ2_PRODUCTION_SERIAL_NUMBER,entry->key))
	{
		memcpy(ver_para->qzq2_version.production_serial_number,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ2_PRODUCTION_TIME,entry->key))
	{
		memcpy(ver_para->qzq2_version.production_time,entry->value,strlen(entry->value));
	}

	//	QZQ3
	else if(!strcmp(KEY_QZQ3_SOFTWARE_VERSION,entry->key))
	{
		memcpy(ver_para->qzq3_version.software_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ3_HARDWARE_VERSION,entry->key))
	{
		memcpy(ver_para->qzq3_version.hard_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ3_OLD_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->qzq3_version.old_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ3_CUR_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->qzq3_version.cur_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ3_PRODUCTION_SERIAL_NUMBER,entry->key))
	{
		memcpy(ver_para->qzq3_version.production_serial_number,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ3_PRODUCTION_TIME,entry->key))
	{
		memcpy(ver_para->qzq3_version.production_time,entry->value,strlen(entry->value));
	}

	//	QZQ4
	else if(!strcmp(KEY_QZQ4_SOFTWARE_VERSION,entry->key))
	{
		memcpy(ver_para->qzq4_version.software_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ4_HARDWARE_VERSION,entry->key))
	{
		memcpy(ver_para->qzq4_version.hard_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ4_OLD_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->qzq4_version.old_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ4_CUR_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->qzq4_version.cur_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ4_PRODUCTION_SERIAL_NUMBER,entry->key))
	{
		memcpy(ver_para->qzq4_version.production_serial_number,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_QZQ4_PRODUCTION_TIME,entry->key))
	{
		memcpy(ver_para->qzq4_version.production_time,entry->value,strlen(entry->value));
	}

	//	TCLB
	else if(!strcmp(KEY_TCLB_SOFTWARE_VERSION,entry->key))
	{
		memcpy(ver_para->tclb_version.software_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TCLB_HARDWARE_VERSION,entry->key))
	{
		memcpy(ver_para->tclb_version.hard_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TCLB_OLD_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->tclb_version.old_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TCLB_CUR_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->tclb_version.cur_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TCLB_PRODUCTION_SERIAL_NUMBER,entry->key))
	{
		memcpy(ver_para->tclb_version.production_serial_number,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TCLB_PRODUCTION_TIME,entry->key))
	{
		memcpy(ver_para->tclb_version.production_time,entry->value,strlen(entry->value));
	}

	//	TCLB_AD
	else if(!strcmp(KEY_TCLB_AD_SOFTWARE_VERSION,entry->key))
	{
		memcpy(ver_para->tclbad_version.software_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TCLB_AD_HARDWARE_VERSION,entry->key))
	{
		memcpy(ver_para->tclbad_version.hard_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TCLB_AD_OLD_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->tclbad_version.old_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TCLB_AD_CUR_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->tclbad_version.cur_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TCLB_AD_PRODUCTION_SERIAL_NUMBER,entry->key))
	{
		memcpy(ver_para->tclbad_version.production_serial_number,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_TCLB_AD_PRODUCTION_TIME,entry->key))
	{
		memcpy(ver_para->tclbad_version.production_time,entry->value,strlen(entry->value));
	}

	//	ZXB
	else if(!strcmp(KEY_ZXB_SOFTWARE_VERSION,entry->key))
	{
		memcpy(ver_para->zxb_version.software_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_ZXB_HARDWARE_VERSION,entry->key))
	{
		memcpy(ver_para->zxb_version.hard_version,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_ZXB_OLD_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->zxb_version.old_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_ZXB_CUR_VERSION_TIME,entry->key))
	{
		memcpy(ver_para->zxb_version.cur_version_time,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_ZXB_PRODUCTION_SERIAL_NUMBER,entry->key))
	{
		memcpy(ver_para->zxb_version.production_serial_number,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_ZXB_PRODUCTION_TIME,entry->key))
	{
		memcpy(ver_para->zxb_version.production_time,entry->value,strlen(entry->value));
	}
	else
	{
		printf("WARNING:%s should not contain %s\n",VERSION_SECTION_NAME,entry->key);
		return -1;
	}

	return 0;
}


/**
 * @brief Read version
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
 * @create time  : 2021年2月28日
 * *
 * Parameters
 * *
 * @param	 path struct comm_ini_para
 *
 * @retval
 *				- 0   program execution completed normally
 *				- -1  Program execution error
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		  kernel       create this class
 *
 */
int read_version_ini( struct version_para* ver_para)
{
	int result;
	FILE *fp;
	ini_entry_t entry;

	if(NULL == ver_para)
	{
		rt_kprintf("ERROR: input point is NULL!\n");
		return -1;
	}

	fp = fopen(VERSION_PARA_FILE_PATH, "r");
	if(fp == NULL)
	{
		rt_kprintf("ERROR: File open failed!\n");
		return -1;
	}

	/* initialize entry structure before reading first entry */
	entry.section = NULL;
	entry.key = NULL;
	entry.value = NULL;

	/* read one entry at a time */
	while((result = GetEntryFromFile(fp, &entry)) > 0)
	{
		if(!strcmp(VERSION_SECTION_NAME,entry.section))
		{
			parse_version_para(ver_para,&entry);
		}
		else
		{
			rt_kprintf("WARNING:%s should not contain %s\n",VERSION_PARA_FILE_PATH,entry.section);
		}
	}

	fclose(fp);

	//printf("[TFDSN-RECORD-SOFT-VERSION]:%s\n",ver_para->record_version.);
	return 0;
}


/**
 * @brief write _version_para parameters
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
 * @create time  : 2021年2月28日
 * *
 * Parameters
 * *
 * @retval
 *				- 0   program execution completed normally
 *				- -1  Program execution error
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		  kernel       create this class
 *
 */
int write_version_para(struct version_para *ver_para)
{
	FILE * ini;

	if((ini = fopen(VERSION_PARA_FILE_PATH, "w"))==NULL) {
		printf("ERROR: open %s failed!\n",VERSION_PARA_FILE_PATH);
		return -1;
	}

	fseek(ini,0,SEEK_SET);

		fprintf(ini,
			"#\n"
			"# This is a version of ini file\n"
			"#\n"
			"\n"
			"[%s]\n"
			"\n"
			"TRAIN_ID  =%u\n"
			"RECORD_SOFTWARE_VERSION      = %s\n"
			"RECORD_HARDWARE_VERSION      = %s\n"
			"RECORD_OLD_VERSION_TIME      = %s\n"
			"RECORD_CUR_VERSION_TIME      = %s\n"
			"RECORD_PRODUCTION_SERIAL_NUMBER      = %s\n"
			"RECORD_PRODUCTION_TIME      = %s\n"
			"\n"
			"TXB_SOFTWARE_VERSION      = %s\n"
			"TXB_HARDWARE_VERSION      = %s\n"
			"TXB_OLD_VERSION_TIME      = %s\n"
			"TXB_CUR_VERSION_TIME      = %s\n"
			"TXB_PRODUCTION_SERIAL_NUMBER      = %s\n"
			"TXB_PRODUCTION_TIME      = %s\n"
			"\n"
			"CLB_SOFTWARE_VERSION      = %s\n"
			"CLB_HARDWARE_VERSION      = %s\n"
			"CLB_OLD_VERSION_TIME      = %s\n"
			"CLB_CUR_VERSION_TIME      = %s\n"
			"CLB_PRODUCTION_SERIAL_NUMBER      = %s\n"
			"CLB_PRODUCTION_TIME      = %s\n"
			"\n"
			"QZQ1_SOFTWARE_VERSION      = %s\n"
			"QZQ1_HARDWARE_VERSION      = %s\n"
			"QZQ1_OLD_VERSION_TIME      = %s\n"
			"QZQ1_CUR_VERSION_TIME      = %s\n"
			"QZQ1_PRODUCTION_SERIAL_NUMBER      = %s\n"
			"QZQ1_PRODUCTION_TIME      = %s\n"
			"\n"
			"QZQ2_SOFTWARE_VERSION      = %s\n"
			"QZQ2_HARDWARE_VERSION      = %s\n"
			"QZQ2_OLD_VERSION_TIME      = %s\n"
			"QZQ2_CUR_VERSION_TIME      = %s\n"
			"QZQ2_PRODUCTION_SERIAL_NUMBER      = %s\n"
			"QZQ2_PRODUCTION_TIME      = %s\n"
			"\n"
			"QZQ3_SOFTWARE_VERSION      = %s\n"
			"QZQ3_HARDWARE_VERSION      = %s\n"
			"QZQ3_OLD_VERSION_TIME      = %s\n"
			"QZQ3_CUR_VERSION_TIME      = %s\n"
			"QZQ3_PRODUCTION_SERIAL_NUMBER      = %s\n"
			"QZQ3_PRODUCTION_TIME      = %s\n"
			"\n"
			"QZQ4_SOFTWARE_VERSION      = %s\n"
			"QZQ4_HARDWARE_VERSION      = %s\n"
			"QZQ4_OLD_VERSION_TIME      = %s\n"
			"QZQ4_CUR_VERSION_TIME      = %s\n"
			"QZQ4_PRODUCTION_SERIAL_NUMBER      = %s\n"
			"QZQ4_PRODUCTION_TIME      = %s\n"
			"\n"
			"TCLB_SOFTWARE_VERSION      = %s\n"
			"TCLB_HARDWARE_VERSION      = %s\n"
			"TCLB_OLD_VERSION_TIME      = %s\n"
			"TCLB_CUR_VERSION_TIME      = %s\n"
			"TCLB_PRODUCTION_SERIAL_NUMBER      = %s\n"
			"TCLB_PRODUCTION_TIME      = %s\n"
			"\n"
			"TCLB_AD_SOFTWARE_VERSION      = %s\n"
			"TCLB_AD_HARDWARE_VERSION      = %s\n"
			"TCLB_AD_OLD_VERSION_TIME      = %s\n"
			"TCLB_AD_CUR_VERSION_TIME      = %s\n"
			"TCLB_AD_PRODUCTION_SERIAL_NUMBER      = %s\n"
			"TCLB_AD_PRODUCTION_TIME      = %s\n"
			"\n"
			"ZXB_SOFTWARE_VERSION      = %s\n"
			"ZXB_HARDWARE_VERSION      = %s\n"
			"ZXB_OLD_VERSION_TIME      = %s\n"
			"ZXB_CUR_VERSION_TIME      = %s\n"
			"ZXB_PRODUCTION_SERIAL_NUMBER      = %s\n"
			"ZXB_PRODUCTION_TIME      = %s\n"
			"\n"
			"\n",VERSION_SECTION_NAME,ver_para->train_id,
			ver_para->record_version.software_version,ver_para->record_version.hard_version,ver_para->record_version.old_version_time,
			ver_para->record_version.cur_version_time,ver_para->record_version.production_serial_number,ver_para->record_version.production_time,
			ver_para->txb_version.software_version,ver_para->txb_version.hard_version,ver_para->txb_version.old_version_time,
			ver_para->txb_version.cur_version_time,ver_para->txb_version.production_serial_number,ver_para->txb_version.production_time,
			ver_para->clb_version.software_version,ver_para->clb_version.hard_version,ver_para->clb_version.old_version_time,
			ver_para->clb_version.cur_version_time,ver_para->clb_version.production_serial_number,ver_para->clb_version.production_time,
			ver_para->qzq1_version.software_version,ver_para->qzq1_version.hard_version,ver_para->qzq1_version.old_version_time,
			ver_para->qzq1_version.cur_version_time,ver_para->qzq1_version.production_serial_number,ver_para->qzq1_version.production_time,
			ver_para->qzq2_version.software_version,ver_para->qzq2_version.hard_version,ver_para->qzq2_version.old_version_time,
			ver_para->qzq2_version.cur_version_time,ver_para->qzq2_version.production_serial_number,ver_para->qzq2_version.production_time,
			ver_para->qzq3_version.software_version,ver_para->qzq3_version.hard_version,ver_para->qzq3_version.old_version_time,
			ver_para->qzq3_version.cur_version_time,ver_para->qzq3_version.production_serial_number,ver_para->qzq3_version.production_time,
			ver_para->qzq4_version.software_version,ver_para->qzq4_version.hard_version,ver_para->qzq4_version.old_version_time,
			ver_para->qzq4_version.cur_version_time,ver_para->qzq4_version.production_serial_number,ver_para->qzq4_version.production_time,
			ver_para->tclb_version.software_version,ver_para->tclb_version.hard_version,ver_para->tclb_version.old_version_time,
			ver_para->tclb_version.cur_version_time,ver_para->tclb_version.production_serial_number,ver_para->tclb_version.production_time,
			ver_para->tclbad_version.software_version,ver_para->tclbad_version.hard_version,ver_para->tclbad_version.old_version_time,
			ver_para->tclbad_version.cur_version_time,ver_para->tclbad_version.production_serial_number,ver_para->tclbad_version.production_time,
			ver_para->zxb_version.software_version,ver_para->zxb_version.hard_version,ver_para->zxb_version.old_version_time,
			ver_para->zxb_version.cur_version_time,ver_para->zxb_version.production_serial_number,ver_para->zxb_version.production_time);

		fclose(ini);
	return 0;
}

/**
 * @brief display display_local_version
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
 * @create time  : 2021年2月28日
 * *
 * Parameters
 * *
 * @param	 [in] the pointer of the production fixed is used to transfer the key value
 *
 * @retval
 *				- 0   program execution completed normally
 *				- -1  Program execution error
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		  kernel       create this class
 *
 */
static void display_local_version(struct version_para* ver_para)
{

	printf("\n[TFDSN-RECORD-VERSION]:\n");

//	if((strcmp(ver_para->record_version.software_version,SOFT_VERSION)!=0)||(strcmp(ver_para->record_version.cur_version_time,SOFT_UPDATE_TIME)!=0))
//	{
////		memcpy(ver_para->record_version.old_version_time,ver_para->record_version.cur_version_time,BUF_LEN);
////		memcpy(ver_para->record_version.cur_version_time,SOFT_UPDATE_TIME,BUF_LEN);
////		memcpy(ver_para->record_version.software_version,SOFT_VERSION,BUF_LEN);
////		memcpy(ver_para->record_version.hard_version,HARDWARE_VERSION,BUF_LEN);
//		write_version_para(ver_para);
//	}


	printf("\t->soft_version:%s\n",ver_para->record_version.software_version);
	printf("\t->hard_version:%s\n",ver_para->record_version.hard_version);
	printf("\t->old_version_time:%s\n",ver_para->record_version.old_version_time);
	printf("\t->cur_version_time:%s\n",ver_para->record_version.cur_version_time);
	printf("\n");

}

/**
 * @brief write default version parameters
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
 * @create time  : 2021年2月28日
 * *
 * Parameters
 * *
 * @retval
 *				- 0   program execution completed normally
 *				- -1  Program execution error
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		  kernel       create this class
 *
 */
static int write_default_version_para()
{
	int ret=-1;
	struct version_para default_version;
	memset(&default_version,0,sizeof(struct version_para));
	default_version.train_id=1;
//	memcpy(default_version.record_version.software_version,SOFT_VERSION,BUF_LEN);
//	memcpy(default_version.record_version.hard_version,HARDWARE_VERSION,BUF_LEN);
//	memcpy(default_version.record_version.old_version_time,SOFT_UPDATE_TIME,BUF_LEN);
//	memcpy(default_version.record_version.cur_version_time,SOFT_UPDATE_TIME,BUF_LEN);
//	memcpy(default_version.record_version.production_serial_number,"0",BUF_LEN);
	memcpy(default_version.record_version.production_time,"0",BUF_LEN);

	memcpy(default_version.txb_version.software_version,"v0.0",BUF_LEN);
	memcpy(default_version.txb_version.hard_version,"0",BUF_LEN);
	memcpy(default_version.txb_version.old_version_time,"0",BUF_LEN);
	memcpy(default_version.txb_version.cur_version_time,"0",BUF_LEN);
	memcpy(default_version.txb_version.production_serial_number,"0",BUF_LEN);
	memcpy(default_version.txb_version.production_time,"0",BUF_LEN);

	memcpy(default_version.clb_version.software_version,"v0.0",BUF_LEN);
	memcpy(default_version.clb_version.hard_version,"0",BUF_LEN);
	memcpy(default_version.clb_version.old_version_time,"0",BUF_LEN);
	memcpy(default_version.clb_version.cur_version_time,"0",BUF_LEN);
	memcpy(default_version.clb_version.production_serial_number,"0",BUF_LEN);
	memcpy(default_version.clb_version.production_time,"0",BUF_LEN);

	memcpy(default_version.qzq1_version.software_version,"v0.0",BUF_LEN);
	memcpy(default_version.qzq1_version.hard_version,"0",BUF_LEN);
	memcpy(default_version.qzq1_version.old_version_time,"0",BUF_LEN);
	memcpy(default_version.qzq1_version.cur_version_time,"0",BUF_LEN);
	memcpy(default_version.qzq1_version.production_serial_number,"0",BUF_LEN);
	memcpy(default_version.qzq1_version.production_time,"0",BUF_LEN);

	memcpy(default_version.qzq2_version.software_version,"v0.0",BUF_LEN);
	memcpy(default_version.qzq2_version.hard_version,"0",BUF_LEN);
	memcpy(default_version.qzq2_version.old_version_time,"0",BUF_LEN);
	memcpy(default_version.qzq2_version.cur_version_time,"0",BUF_LEN);
	memcpy(default_version.qzq2_version.production_serial_number,"0",BUF_LEN);
	memcpy(default_version.qzq2_version.production_time,"0",BUF_LEN);

	memcpy(default_version.qzq3_version.software_version,"v0.0",BUF_LEN);
	memcpy(default_version.qzq3_version.hard_version,"0",BUF_LEN);
	memcpy(default_version.qzq3_version.old_version_time,"0",BUF_LEN);
	memcpy(default_version.qzq3_version.cur_version_time,"0",BUF_LEN);
	memcpy(default_version.qzq3_version.production_serial_number,"0",BUF_LEN);
	memcpy(default_version.qzq3_version.production_time,"0",BUF_LEN);

	memcpy(default_version.qzq4_version.software_version,"v0.0",BUF_LEN);
	memcpy(default_version.qzq4_version.hard_version,"0",BUF_LEN);
	memcpy(default_version.qzq4_version.old_version_time,"0",BUF_LEN);
	memcpy(default_version.qzq4_version.cur_version_time,"0",BUF_LEN);
	memcpy(default_version.qzq4_version.production_serial_number,"0",BUF_LEN);
	memcpy(default_version.qzq4_version.production_time,"0",BUF_LEN);

	memcpy(default_version.tclb_version.software_version,"v0.0",BUF_LEN);
	memcpy(default_version.tclb_version.hard_version,"0",BUF_LEN);
	memcpy(default_version.tclb_version.old_version_time,"0",BUF_LEN);
	memcpy(default_version.tclb_version.cur_version_time,"0",BUF_LEN);
	memcpy(default_version.tclb_version.production_serial_number,"0",BUF_LEN);
	memcpy(default_version.tclb_version.production_time,"0",BUF_LEN);

	memcpy(default_version.tclbad_version.software_version,"v0.0",BUF_LEN);
	memcpy(default_version.tclbad_version.hard_version,"0",BUF_LEN);
	memcpy(default_version.tclbad_version.old_version_time,"0",BUF_LEN);
	memcpy(default_version.tclbad_version.cur_version_time,"0",BUF_LEN);
	memcpy(default_version.tclbad_version.production_serial_number,"0",BUF_LEN);
	memcpy(default_version.tclbad_version.production_time,"0",BUF_LEN);

	memcpy(default_version.zxb_version.software_version,"v0.0",BUF_LEN);
	memcpy(default_version.zxb_version.hard_version,"0",BUF_LEN);
	memcpy(default_version.zxb_version.old_version_time,"0",BUF_LEN);
	memcpy(default_version.zxb_version.cur_version_time,"0",BUF_LEN);
	memcpy(default_version.zxb_version.production_serial_number,"0",BUF_LEN);
	memcpy(default_version.zxb_version.production_time,"0",BUF_LEN);

	ret=write_version_para(&default_version);

	return ret;

}

/**
 * @brief update version parameters
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
 * @create time  : 2021年2月28日
 * *
 * Parameters
 * *
 * @retval
 *				- 0   program execution completed normally
 *				- -1  Program execution error
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		  kernel       create this class
 *
 */
int update_version_para(struct version_para *ver_para)
{
	int ret=-1;

	struct version_para para;
	memcpy(&para,ver_para,sizeof(struct version_para));
	ret=write_version_para(&para);
	return ret;
}

/*****************************configuration file initialization***********************************/
/**
 * @brief determine whether the configuration file exists,if not,create and write default configuration,
 * if it exists,do not process
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
 * @create time  : 2021年2月28日
 * *
 * Parameters
 * *
 * @retval
 *				- 0   program execution completed normally
 *				- -1  Program execution error
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date           author       notes
 * 2021年2月28日		kernel       create this class
 *
 */
int config_ini_file_init(void)
{

	memset(&comm_para,0,sizeof(struct comm_ini_para));
//	memset(ver_para,0,sizeof(struct version_para));

	//脱轨板不适用
//	if(access(VERSION_PARA_FILE_PATH, F_OK)!= 0)
//	{
//		write_default_version_para();
//	}

//	if(read_version_ini(ver_para)==0)
//		display_local_version(ver_para);

	if(access(COMM_PARA_FILE_PATH, F_OK)!= 0)
	{
		write_default_comm_para();
	}

	if(read_comm_ini(&comm_para)==0)
		display_comm_ini_para(&comm_para);

	return 0;
}

/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

//#endif //\end module API
