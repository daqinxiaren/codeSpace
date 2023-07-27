#ifndef _AD7606_H_
#define _AD7606_H_
#include <stdint.h>

#include "udp_client.h"
#include "spi_app.h"

struct qzq_config
{
	uint16_t config_flag;
	uint16_t src_id;
	uint16_t tag_id;
	uint16_t CONFIG_CMD_TYPE;
	uint16_t CONFIG_OK_TYPE;
};

extern struct qzq_config qzqconfig;
/**
//ad 采集的公共信息：序列号、时间、站台等
*/

struct AD_PUBLIC_INFO
{
    unsigned char head[2]; //包头
    unsigned char lenth[2];//包数据长度
    unsigned char type;//数据类型

    unsigned char year;
    unsigned char mon;
    unsigned char day;
    unsigned char hour;
    unsigned char min;
    unsigned char sec;

    unsigned char counter[2];//包计数
    unsigned char curr_id[2];//当前站id
    unsigned char next_id[2];//下有站id
    unsigned char speed[2];//当前速度
    unsigned char wheel[2];//轮径信息
    unsigned char channel;//通道号
    struct version_t advdrsion;
    unsigned char rcc[34];
};
struct DERAIL_DAT
{
	//存储算法相关数据
	struct AD_PUBLIC_INFO info;
	sem_t derail_sem;
	int16_t data_buf[SAMPLE_CHANNEL_NUM][SAMPLE_SIZE]; //
};

/* spi1脱轨数据接收结构体 */
struct spi_derail{

    uint16_t head_buf[HEAD_LEN];                   //标志头数组
    struct AD_PUBLIC_INFO  info;                    //公共信息
    uint16_t reserv[RESERVE_SIZE];                  //预留
    uint16_t buffer[SAMPLE_CHANNEL_NUM][SAMPLE_SIZE];           //脱轨数据缓存
};
extern struct AD_PUBLIC_INFO public_msg;

/* SPI通信命令类型 */
typedef enum SPI_CMD_TYPE_EN{
	SWITCH_CH_CMD_TYPE = 0x01,
	RESET_CMD_TYPE = 0x02,
	UPDATE_CMD_TYPE = 0x03,
	UPDATE_RST_CMD_TYPE=0x04
}SPI_CMD_TYPE;

/*定义spi标志结构体*/
struct spi_flag_st{
	uint8_t cur_cmd_type;						//当前命令标志
	uint8_t update_flag;
	uint8_t update_count;
	uint8_t proc1_reset_flag;					//重启采集板标志位
	uint8_t proc2_reset_flag;					//重启采集板标志位
	uint8_t spi1_err_reset_flag;				//spi1故障,重启采集板标志位
	uint8_t spi2_err_reset_flag;				//spi1故障,重启采集板标志位
	uint8_t spi2_recv_reset_flag;				//spi接收到复位标志位
	uint8_t spi2_recv_update_flag;			//spi接收到正确升级数据标志位
	uint8_t spi2_update_over_flag;			//spi接收到升级完成标志位
	uint8_t spi2_update_start_flag;			//spi开始升级标志位
	uint8_t spi2_bearing_deal;
	uint8_t spi1_plougon_deal;
	uint8_t spi_send_cnt;
	uint8_t port_finish;
	uint8_t spi_comm_reset_flag;
	uint8_t spi_zd_sample_ch;
};

/*定义spi1振动1分钟接收处理结构体标志记录*/
struct spi_min_deal_st{
	uint32_t ad_beg_cnt;						//开始序列号
	uint32_t ad_cur_cnt;						//当前采集序列号
	uint8_t ad_count;
	uint8_t count_reset_flag;
};

/*************************************************
Function:  init_spi_device
Description:  初始化设置SPI设备
Input: 　无
Output: 无
Return: 成功０
Others:
*************************************************/
int init_spi_device(void);
int init_spi2_device(void);


/*************************************************
Function:  spi_read_data
Description:  开始从ＳＰＩ读取数据
Input:
Output: 无
Return: 无
Others:
*************************************************/
void spi1_read_data(void);
void spi2_read_data(void);

/*************************************************
Function:  outc_change_switch
Description:  开始从ＳＰＩ读取数据
Input:
Output: 无
Return: 无
Others:
*************************************************/
void outc_change_switch(void);
void spi1_headmsg_change(void);


void ad_board_reset(uint8_t tag,uint8_t cmd);
void ad_board_update_reset(uint8_t cmd);

void spi2_transfer_data(uint16_t *tx_buf,uint16_t *rx_buf,uint16_t len);
void spi2_recev_data_deal(uint16_t *data);

void spi_flag_init(void);
void spi2_send_data(SPI_CMD_TYPE type,uint16_t *data,uint16_t len);
void update_save_msg(struct AD_PUBLIC_INFO *msg,uint8_t type);

#endif
