/*
 * spi_app.h
 *
 *  Created on: Jun 29, 2020
 *      Author: linux-ls
 */

#ifndef _SPI_APP_H_
#define _SPI_APP_H_

#include <stdint.h>
//#include "ad7606.h"

//#define UPDATE_FILE_NAME "/media/local_config/app/tfdsh-adb.bin"

/************* SPI1脱轨数据接收缓存定义 ******************/

//#define SAMPLE_CHANNEL_NUM 	8			//需要采集通道数
//#define SAMPLE_TIME         1           //采用时间-秒
//#define SAMPLE_FS           3000        //采样频率
//#define SAMPLE_SIZE         SAMPLE_TIME*SAMPLE_FS        //1个通道采样大小-u16
//
//#define HEAD_LEN 			100 	   //协议头大小-u16
//#define LOCAL_DEV_INOF      100        //本地设备信息大小
//#define MATCH_DATA			2000       //匹配数据
//
//#define RESERVE_SIZE        (MATCH_DATA -HEAD_LEN-(sizeof(struct AD_PUBLIC_INFO)/2)) //预留
//#define DAT_PACKAGE_SIZE    SAMPLE_SIZE*SAMPLE_CHANNEL_NUM+MATCH_DATA  //数据包总大小

#define SAMPLE_CHANNEL_NUM  8			//需要采集通道数
#define SAMPLE_TIME         1           //采用时间-秒
#define SAMPLE_FS           4096        //采样频率
#define SAMPLE_SIZE         SAMPLE_TIME*SAMPLE_FS        //1个通道采样大小-u16

#define HEAD_LEN 			100 	   //协议头大小-u16
#define MATCH_DATA			2000-768       //匹配数据

#define RESERVE_SIZE        (MATCH_DATA -HEAD_LEN-(sizeof(struct AD_PUBLIC_INFO)/2)) //预留
#define DAT_PACKAGE_SIZE    34000  //数据包总大小


/************* SPI2 CMD接收缓存定义 ******************/
#define SPI2_CMD_USING_SIZE  10000

/* SPI IAP通信命令类型 */
typedef enum SPI_IAP_CMD_TYPE{
	IAP_READY_TYPE = 0x01,
	START_CMD_TYPE = 0x02,
	END_CMD_TYPE = 0x03,
	ERR_CMD_TYPE=0x04
}SPI_IAP_TYPE;


extern uint16_t default_tx[2000]; 	//spi发送buffer，为了给从机提供时钟，必须要有发送数据的动作
extern uint16_t default_tx2[2000];  //spi发送buffer，为了给从机提供时钟，必须要有发送数据的动作
extern struct spi_flag_st spi_flag;
extern struct spi_derail *spi1_recv_derail;
extern uint16_t *temp_buffer;

int init_spi_thread(void);
int init_spi2_thread(void);
void spi_thread_entry(void);
uint8_t spi_update_func(uint8_t board_id);


#endif /* 2_INC_SPI_APP_H_ */
