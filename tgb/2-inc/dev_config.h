/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the system_para.h API or library.
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
 
#ifndef MODULE_CONFIG_DEV_CONFIG_H_
#define MODULE_CONFIG_DEV_CONFIG_H_

/**
 ***************** include file ***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

//#include <drv_config.h>
//#include <rtconfig.h>

//#ifdef ENABLE_INIPARSER

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/** @formatter:off  */
/**
 ****************<Public macro>****************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/**vesion
*20210526
*20210603
*20210712
 */

//#define XXX_VERSION	 		1 			/**< major version number */
#define TCLB_SOFT_VERSION        0x0100    //
#define TCLB_UPDATE_TIME    20210909    //软件发布时间
#define TCLB_HARDWARE_VERSION_H    'A'
#define TCLB_HARDWARE_VERSION_L    '1'

#define SYSTEM_CONFIG_FILE_PATH					"/media/LH_DATA/update/update_system/tclb_profile"
#define SYSTEM_CONFIG_SH_PATH_MKDIR			   "/media/LH_DATA/update/update_system/mkdir.sh"
#define SYSTEM_CONFIG_DTB_PATH	   "/media/LH_DATA/update/update_system/am335x-boneblack.dtb"


/***********************************COMM_PARA_FILE ***********************************/
#define BUF_LEN 16
//filename path
#define COMM_PARA_FILE_PATH 				"/media/LH_DATA/local_config/config/comm.ini"
//section name
#define COMM_PTU_SECTION_NAME     		"ETH"
#define COMM_BR_SECTION_NAME     		"BR"
#define COMM_TRDP_SECTION_NAME     		"TRDP"
#define COMM_MVB_SECTION_NAME     		"MVB"
//parameters key name
#define KEY_TRAIN_ID       				"TRAIN_ID"
#define KEY_PTU_IP       					"PTU_IP"
#define KEY_PTU_MASK    				 	"PTU_MASK"
#define KEY_PTU_GW       					"PTU_GW"
#define KEY_PTU_PORT    					"PTU_PORT"
#define KEY_IP       						"IP"
#define KEY_MASK    				 			"MASK"
#define KEY_GW       						"GW"
#define KEY_PORT    							"PORT"
#define KEY_ETH1_GROUP1_IP    			"ETH1_GROUP1_IP"
#define KEY_ETH1_GROUP2_IP   				"ETH1_GROUP2_IP"
#define KEY_ETH1_GROUP1_PORT   			"ETH1_GROUP1_PORT"
#define KEY_ETH1_GROUP2_PORT   			"ETH1_GROUP2_PORT"
#define KEY_BR_ENABLE   					"BR_ENABLE"
#define KEY_BR_IP   							"BR_IP"

/*******************************VERSION_PARA_FILE *****************************/
//filename path
#define VERSION_PARA_FILE_PATH					"/media/LH_DATA/local_config/config/version.ini"
//section name
#define VERSION_SECTION_NAME     		"VERSION"
//parameters key name--RECORD
#define KEY_TRAIN_ID_VERSION       						"TRAIN_ID"
#define KEY_RECORD_SOFTWARE_VERSION       			"RECORD_SOFTWARE_VERSION"
#define KEY_RECORD_HARDWARE_VERSION    				"RECORD_HARDWARE_VERSION"
#define KEY_RECORD_OLD_VERSION_TIME       			"RECORD_OLD_VERSION_TIME"
#define KEY_RECORD_CUR_VERSION_TIME    				"RECORD_CUR_VERSION_TIME"
#define KEY_RECORD_PRODUCTION_SERIAL_NUMBER    		"RECORD_PRODUCTION_SERIAL_NUMBER"
#define KEY_RECORD_PRODUCTION_TIME   					"RECORD_PRODUCTION_TIME"

#define KEY_TXB_SOFTWARE_VERSION				"TXB_SOFTWARE_VERSION"
#define KEY_TXB_HARDWARE_VERSION  			"TXB_HARDWARE_VERSION"
#define KEY_TXB_OLD_VERSION_TIME				"TXB_OLD_VERSION_TIME"
#define KEY_TXB_CUR_VERSION_TIME				"TXB_CUR_VERSION_TIME"
#define KEY_TXB_PRODUCTION_SERIAL_NUMBER	"TXB_PRODUCTION_SERIAL_NUMBER"
#define KEY_TXB_PRODUCTION_TIME				"TXB_PRODUCTION_TIME"

#define KEY_CLB_SOFTWARE_VERSION				"CLB_SOFTWARE_VERSION"
#define KEY_CLB_HARDWARE_VERSION				"CLB_HARDWARE_VERSION"
#define KEY_CLB_OLD_VERSION_TIME				"CLB_OLD_VERSION_TIME"
#define KEY_CLB_CUR_VERSION_TIME				"CLB_CUR_VERSION_TIME"
#define KEY_CLB_PRODUCTION_SERIAL_NUMBER	"CLB_PRODUCTION_SERIAL_NUMBER"
#define KEY_CLB_PRODUCTION_TIME				"CLB_PRODUCTION_TIME"

#define KEY_QZQ1_SOFTWARE_VERSION			"QZQ1_SOFTWARE_VERSION"
#define KEY_QZQ1_HARDWARE_VERSION			"QZQ1_HARDWARE_VERSION"
#define KEY_QZQ1_OLD_VERSION_TIME			"QZQ1_OLD_VERSION_TIME"
#define KEY_QZQ1_CUR_VERSION_TIME			"QZQ1_CUR_VERSION_TIME"
#define KEY_QZQ1_PRODUCTION_SERIAL_NUMBER	"QZQ1_PRODUCTION_SERIAL_NUMBER"
#define KEY_QZQ1_PRODUCTION_TIME				"QZQ1_PRODUCTION_TIME"

#define KEY_QZQ2_SOFTWARE_VERSION			"QZQ2_SOFTWARE_VERSION"
#define KEY_QZQ2_HARDWARE_VERSION			"QZQ2_HARDWARE_VERSION"
#define KEY_QZQ2_OLD_VERSION_TIME			"QZQ2_OLD_VERSION_TIME"
#define KEY_QZQ2_CUR_VERSION_TIME			"QZQ2_CUR_VERSION_TIME"
#define KEY_QZQ2_PRODUCTION_SERIAL_NUMBER	"QZQ2_PRODUCTION_SERIAL_NUMBER"
#define KEY_QZQ2_PRODUCTION_TIME				"QZQ2_PRODUCTION_TIME"

#define KEY_QZQ3_SOFTWARE_VERSION			"QZQ3_SOFTWARE_VERSION"
#define KEY_QZQ3_HARDWARE_VERSION			"QZQ3_HARDWARE_VERSION"
#define KEY_QZQ3_OLD_VERSION_TIME			"QZQ3_OLD_VERSION_TIME"
#define KEY_QZQ3_CUR_VERSION_TIME			"QZQ3_CUR_VERSION_TIME"
#define KEY_QZQ3_PRODUCTION_SERIAL_NUMBER	"QZQ3_PRODUCTION_SERIAL_NUMBER"
#define KEY_QZQ3_PRODUCTION_TIME				"QZQ3_PRODUCTION_TIME"

#define KEY_QZQ4_SOFTWARE_VERSION			"QZQ4_SOFTWARE_VERSION"
#define KEY_QZQ4_HARDWARE_VERSION			"QZQ4_HARDWARE_VERSION"
#define KEY_QZQ4_OLD_VERSION_TIME			"QZQ4_OLD_VERSION_TIME"
#define KEY_QZQ4_CUR_VERSION_TIME			"QZQ4_CUR_VERSION_TIME"
#define KEY_QZQ4_PRODUCTION_SERIAL_NUMBER	"QZQ4_PRODUCTION_SERIAL_NUMBER"
#define KEY_QZQ4_PRODUCTION_TIME				"QZQ4_PRODUCTION_TIME"

#define KEY_TCLB_SOFTWARE_VERSION			"TCLB_SOFTWARE_VERSION"
#define KEY_TCLB_HARDWARE_VERSION			"TCLB_HARDWARE_VERSION"
#define KEY_TCLB_OLD_VERSION_TIME			"TCLB_OLD_VERSION_TIME"
#define KEY_TCLB_CUR_VERSION_TIME			"TCLB_CUR_VERSION_TIME"
#define KEY_TCLB_PRODUCTION_SERIAL_NUMBER	"TCLB_PRODUCTION_SERIAL_NUMBER"
#define KEY_TCLB_PRODUCTION_TIME				"TCLB_PRODUCTION_TIME"

#define KEY_TCLB_AD_SOFTWARE_VERSION			"TCLB_AD_SOFTWARE_VERSION"
#define KEY_TCLB_AD_HARDWARE_VERSION			"TCLB_AD_HARDWARE_VERSION"
#define KEY_TCLB_AD_OLD_VERSION_TIME			"TCLB_AD_OLD_VERSION_TIME"
#define KEY_TCLB_AD_CUR_VERSION_TIME			"TCLB_AD_CUR_VERSION_TIME"
#define KEY_TCLB_AD_PRODUCTION_SERIAL_NUMBER	"TCLB_AD_PRODUCTION_SERIAL_NUMBER"
#define KEY_TCLB_AD_PRODUCTION_TIME				"TCLB_AD_PRODUCTION_TIME"

#define KEY_ZXB_SOFTWARE_VERSION				"ZXB_SOFTWARE_VERSION"
#define KEY_ZXB_HARDWARE_VERSION				"ZXB_HARDWARE_VERSION"
#define KEY_ZXB_OLD_VERSION_TIME				"ZXB_OLD_VERSION_TIME"
#define KEY_ZXB_CUR_VERSION_TIME				"ZXB_CUR_VERSION_TIME"
#define KEY_ZXB_PRODUCTION_SERIAL_NUMBER	"ZXB_PRODUCTION_SERIAL_NUMBER"
#define KEY_ZXB_PRODUCTION_TIME				"ZXB_PRODUCTION_TIME"



/** @formatter:on */

/**
 ***************<Public typedef>*************** */
/***********************************COMM_PARA_FILE STRUCT parameters***********************************/
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

struct comm_ptu_para
{
	uint8_t train_id;//车厢号
	char ptu_ip[16];//ptu ip地址
	char ptu_mask[16];
	char ptu_gw[16];
	uint16_t ptu_port;//ptu端口
	char eth1_group1_ip[16];//车厢间通信组播地址1
	char eth1_group2_ip[16];//车厢间通信组播地址2
	uint16_t eth1_group1_port;//
	uint16_t eth1_group2_port;//

};

struct comm_br_para
{
	uint8_t br_enable;//桥接使能开关
	char br_ip[16];
};

struct comm_ini_para
{
	struct comm_ptu_para ptu_para;
	struct comm_br_para br_para;
	struct comm_ip_para cur_ip;
};

/*******************************version parameters*****************************/
/**
 * @description		:
 * @note			: null 
 * @note			: null 
 */
struct version_board
{
	 char software_version[BUF_LEN];
	 char hard_version[BUF_LEN];
	 char old_version_time[BUF_LEN];
	 char cur_version_time[BUF_LEN];
	 char production_serial_number[BUF_LEN];
	 char production_time[BUF_LEN];
};


struct version_para
{
	 uint8_t train_id;

	 struct version_board record_version;
	 struct version_board txb_version;
	 struct version_board clb_version;
	 struct version_board qzq1_version;
	 struct version_board qzq2_version;
	 struct version_board qzq3_version;
	 struct version_board qzq4_version;

	 struct version_board tclb_version;
	 struct version_board tclbad_version;
	 struct version_board zxb_version;

};

/**
 **************<Public variables>**************
 * @description		: null 
 * @note			: null 
 * @note			: null  
 */
extern struct comm_ip_para comm_para;
//extern struct version_para ver_para;
int update_comm_para(void);
uint8_t app_paras_ip_check(uint8_t *ip);
uint8_t app_paras_mask_check(uint8_t *mask);
uint8_t app_paras_gateway_check(uint8_t *gateway);
void display_comm_ini_para(struct comm_ip_para *comm_para);

/**
 **************<Public functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/


/**
 **************<Public functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

//int config_ini_file_init(struct comm_ini_para *comm_para,struct version_para *ver_para);
//int update_version_para(struct version_para *ver_para);

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

//#endif /* ENABLE_INIPARSER */

#endif

