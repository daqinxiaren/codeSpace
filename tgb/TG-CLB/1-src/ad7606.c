
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "user_data.h"
#include "ad7606.h"
//#include "spi_app.h"
#include "sec_app.h"
#include "bearing_diagnosis.h"
#include "derail_app.h"
#include "vibr_diagnosis.h"
#include "udp_client.h"
#include "hdd_save.h"

#define SPI_DEBUG

#ifdef SPI_DEBUG
#define SPI_PRINT printf
#else
#define SPI_PRINT(...)
#endif

#define  SPI_DEVICE   "/dev/spidev1.0"   //用于传输ad采样数据的spi设备
#define  SPI_DEVICE2  "/dev/spidev2.0"   //用于控制ad采样数据的通信
#define PRO_HEAD_LEN 11


static uint32_t mode = 0;  //SPI模式
static uint32_t mode2 = 0;  //SPI模式
static uint8_t bits = 16;  //数据位长
static uint32_t speed =10000000; //SPI传输时钟频率,20M
static uint16_t delay = 100;  //传输延时

//extern struct send_local_st local_st;
extern uint16_t default_tx[2000]; 	//spi发送buffer，为了给从机提供时钟，必须要有发送数据的动作
extern uint16_t default_tx2[2000];  //spi发送buffer，为了给从机提供时钟，必须要有发送数据的动作

struct qzq_config qzqconfig;
struct spi_flag_st spi_flag;

int spi_fd = -1;
int spi_fd2 = -1;
struct AD_PUBLIC_INFO public_msg={
		.head[0]=0x66,
		.head[1]=0xbb,
		.channel=0,
};

extern struct DERAIL_DAT derail_dat;
static void spi_transfer_data(uint16_t *tx_buf,uint16_t *rx_buf,uint16_t len);
static void spi1_recev_data_deal(uint16_t *data);

void spi1_send_data(SPI_CMD_TYPE type,uint16_t *data,uint16_t len);
void spi2_send_data(SPI_CMD_TYPE type,uint16_t *data,uint16_t len);

/*************************************************
Function:  init_spi_deviceinit_spi_device
Description:  初始化设置SPI设备
Input: 　无
Output: 无
Return: 成功０
Others:
*************************************************/
int init_spi_device(void)
{
	int ret = 0;

    spi_fd = open(SPI_DEVICE , O_RDWR);
	if (spi_fd < 0)
	{
		DEBUG("can't open SPI_SPI_DEVICE");
	}
	/*
	 * spi mode
	 */

	ret = ioctl(spi_fd, SPI_IOC_WR_MODE32, &mode);
	if (ret == -1)
	{
		DEBUG("can't set spi mode");
	}
	ret = ioctl(spi_fd, SPI_IOC_RD_MODE32, &mode);
	if (ret == -1)
	{
		DEBUG("can't get spi mode");
	}
	/*
	 * bits per word
	 */
	ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
	{
		DEBUG("can't set bits per word");
	}
	ret = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
	{
		DEBUG("can't get bits per word");
	}
	/*
	 * max speed hz
	 */
	ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		DEBUG("can't set max speed hz");
	}
	ret = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		DEBUG("can't get max speed hz");
	}

	printf("-\t->spi1 init: host bit%d\n", bits);

	close(spi_fd);
	return ret;
}



/*************************************************
Function:  spi_transfer_data
Description:  SPI主机发起数据传输的函数
Input:  发送缓存tx_buf ,接收缓存rx_buf,传输长度len
Output: 无
Return: 无
Others:
*************************************************/
static void spi_transfer_data(uint16_t *tx_buf,uint16_t *rx_buf,uint16_t len)
{
	int ret;

	spi_fd= open(SPI_DEVICE , O_RDWR);
	if (spi_fd < 0)
	{
		DEBUG("can't open SPI_SPI_DEVICE");
		system("reboot -nf");
	}
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx_buf,
		.rx_buf = (unsigned long)rx_buf,
		.len = len,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
	ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
	{
		DEBUG("can't send spi message\r\n");
	}
	close(spi_fd);
}


/*************************************************
Function:  spi1_read_data
Description: 开始从spi读取数据
Input:
Output: 无
Return: 无
Others:
*************************************************/
void spi1_read_data(void)
{

	uint16_t tmp_buf[2000] = {0};
	uint16_t tmp_data[DAT_PACKAGE_SIZE] = {0};

	int send_cnt=(DAT_PACKAGE_SIZE)/2000;
	//printf("send_cnt=%d\n",send_cnt);
	//连续读回40k的数据，10K*2

	for(int i=0; i<send_cnt; i++)
	{
//		printf("send_cnt = %d,  default_tx = 0x%02x%02x\n",send_cnt,default_tx[0],default_tx[1]);
		spi_transfer_data(default_tx,tmp_buf,4000); //全双工读写，也就是写与读同时进行

		if(i==1)
			memset(default_tx,0xcc,4000);
		memmove(&tmp_data[i*2000],tmp_buf,4000);
		memset(tmp_buf,0,sizeof(tmp_buf));
		usleep(100);
	}
	//判断是否获取正确通道号及通道数据
	spi1_recev_data_deal(tmp_data);

	//清除buf
	memset(tmp_data,0,DAT_PACKAGE_SIZE*2);
}

/*************************************************
Function:  spi1_recev_data_deal
Description: spi1接受数据处理函数
            STM32将1s的振动原始数据324000Byte，分成5包发送
Input:
Output: 无
Return: 无
Others:
*************************************************/
static void spi1_recev_data_deal(uint16_t *data)
{

	static uint32_t recv_cnt=0;

	memmove(spi1_recv_derail,data,sizeof(struct spi_derail));

	//校验头是否正确,判断获取到数据包号
	if((data[0]==0x55)&&(data[1]==0xaa))
	{
		//printf("***** vibra recv data pakg %d in %d******\r\n",data[4],data[5]);
		switch(data[3])
		{
			case SWITCH_CH_CMD_TYPE:

				update_save_msg(&derail_dat.info,0x11);

				memcpy(&local_st.ad_version,&spi1_recv_derail->info.advdrsion,sizeof(struct version_t));

				derail_dat.info.counter[0]=spi1_recv_derail->info.counter[0];
				derail_dat.info.counter[1]=spi1_recv_derail->info.counter[1];


				printf("***** vibra recv data pakg %d in %d******\r\n",derail_dat.info.counter[0]+(derail_dat.info.counter[1]<<8),data[5]);
				printf(" \n");
				for(int i=0;i<SAMPLE_CHANNEL_NUM;i++)
				{
					//printf(" %x",data[MATCH_DATA+i*SAMPLE_SIZE+5]);
					derail_dat.info.channel=i+1;
					save_sample_data((uint8_t*)&derail_dat.info,sizeof( struct AD_PUBLIC_INFO ));
					memcpy(derail_dat.data_buf[i],&data[MATCH_DATA+i*SAMPLE_SIZE],SAMPLE_SIZE*2);

					save_sample_data((uint8_t*)&data[MATCH_DATA+i*SAMPLE_SIZE],SAMPLE_SIZE*2);
				}
				recv_cnt++;

				if(recv_cnt%60==0)
					sync_file_work(syncflag.time_update_flag);
				sem_post(&derail_dat.derail_sem);
				break;

			default:
				break;
		}
		spi_flag.spi1_err_reset_flag=0;
		sysdev.bits.ad_err=0;
	}
	else
	{
		spi_flag.spi1_err_reset_flag++;
		if(spi_flag.spi1_err_reset_flag>60)
			sysdev.bits.ad_err=1;
	}
}

//spi2
/*************************************************
Function:  init_spi2_deviceinit_spi_device
Description:  初始化设置SPI设备
Input: 　无
Output: 无
Return: 成功０
Others:
*************************************************/
int init_spi2_device(void)
{
	int ret = 0;

    spi_fd2 = open(SPI_DEVICE2 , O_RDWR);
	if (spi_fd2 < 0)
	{
		DEBUG("can't open SPI_SPI_DEVICE2");
	}
	/*
	 * spi mode
	 */
	ret = ioctl(spi_fd2, SPI_IOC_WR_MODE32, &mode2);
	if (ret == -1)
	{
		DEBUG("can't set spi2 mode");
	}
	ret = ioctl(spi_fd2, SPI_IOC_RD_MODE32, &mode2);
	if (ret == -1)
	{
		DEBUG("can't get spi2 mode");
	}
	/*
	 * bits per word
	 */
	ret = ioctl(spi_fd2, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
	{
		DEBUG("can't set per word");
	}
	ret = ioctl(spi_fd2, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
	{
		DEBUG("can't get bits per word");
	}
	/*
	 * max speed hz
	 */
	ret = ioctl(spi_fd2, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		DEBUG("can't set max speed hz");
	}
	ret = ioctl(spi_fd2, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		DEBUG("can't get max speed hz");
	}
	printf("\t->spi2 init: host bit%d\n", bits);
	close(spi_fd2);
	return ret;
}
/*************************************************
Function:  spi2_transfer_data
Description:  SPI主机发起数据传输的函数
Input:  发送缓存tx_buf ,接收缓存rx_buf,传输长度len
Output: 无
Return: 无
Others:
*************************************************/
void spi2_transfer_data(uint16_t *tx_buf,uint16_t *rx_buf,uint16_t len)
{
	int ret;

	spi_fd2= open(SPI_DEVICE2 , O_RDWR);
	if (spi_fd2 < 0)
	{
		printf("can't open SPI_SPI_DEVICE2");
		system("reboot -nf");
	}
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx_buf,
		.rx_buf = (unsigned long)rx_buf,
		.len = len,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
	ret = ioctl(spi_fd2, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
	{
		DEBUG("can't send spi2 message\r\n");
	}
	close(spi_fd2);
}

/*************************************************
Function:  start_read_bear_data
Description: 开始从spi读取数据
Input:
Output: 无
Return: 无
Others:
*************************************************/
void spi2_read_data(void)
{
	uint16_t tmp_buf[2000] = {0};
	uint16_t tmp_data[SPI2_CMD_USING_SIZE] = {0};

	int send_cnt=SPI2_CMD_USING_SIZE/2000;
	//连续读回20k的数据，10K*2
	for(int i=0; i<send_cnt; i++)
	{
		spi2_transfer_data(default_tx2,tmp_buf,4000); //全双工读写，也就是写与读同时进行
		memmove(&tmp_data[i*2000],tmp_buf,4000);
		memset(tmp_buf,0,sizeof(tmp_buf));
	}
	//判断是否获取正确通道号及通道数据
	spi2_recev_data_deal(tmp_data);

	//清除buF
	memset(tmp_data,0,sizeof(tmp_data));
}



/*************************************************
Function:  spi2_recev_data_deal
Description: spi2接受数据处理函数
Input:
Output: 无
Return: 无
Others:can_send tz data
*************************************************/
void spi2_recev_data_deal(uint16_t *data)
{
	//校验头是否正确,判断是否获取到正确的通道号数据,否则继续请求
	if((data[0]==0x55)&&(data[1]==0xaa))
	{
		switch(data[3])
		{
			printf("****** recv spi2 cmd **********\r\n");
			case RESET_CMD_TYPE:

				if(data[10]==AD_BOARD)
				{
					printf("****** recv stm32 reset success cmd **********\r\n");
					spi_flag.spi2_recv_reset_flag = 1;
				}
				break;

			case UPDATE_CMD_TYPE:

				if(data[10]==START_CMD_TYPE)
				{
					if(spi_flag.spi_send_cnt==(data[11]+1))
					{
						printf("recv UPDATE_CMD_TYPE message pakg = %d\r\n",data[11]);
						spi_flag.spi2_recv_update_flag = 1;
					}
				}

				else if(data[10]==END_CMD_TYPE)
				{
					if(data[11]==0xff)
					{
						spi_flag.spi2_update_over_flag = 1;
					}
				}
				else if(data[10]==ERR_CMD_TYPE)
				{
					printf("recv ERR_CMD_TYPE message = %x\r\n",data[11]);
					if(data[11]==0xff ||data[11]==0xaa)
						spi_flag.port_finish=data[11];
				}

				break;
			default:
				break;
		}
		spi_flag.spi2_err_reset_flag=0;
	}
	else
	{
		spi_flag.spi2_err_reset_flag++;

	}
}

//控制指令

/*************************************************
Function:  outc_change_switch
Description: 改变轴承切换通道,更新spi1发送缓存
Input:
Output: 无
Return: 无
Others:
*************************************************/
void outc_change_switch(void)
{
	struct LOCAL_TIME time_now;
	get_local_time(&time_now);

	uint16_t tmp_buf[32]={0};
	tmp_buf[0] = 1;				//数据请求有效,1：有效，0：无效
	tmp_buf[1]=0;
	tmp_buf[2]=time_now.year-2000;
	tmp_buf[3]=time_now.mon;
	tmp_buf[4]=time_now.day;
	tmp_buf[5]=time_now.hour;
	tmp_buf[6]=time_now.min;
	tmp_buf[7]=time_now.sec;

	spi2_send_data(SWITCH_CH_CMD_TYPE,tmp_buf,11);
	spi_flag.cur_cmd_type=SWITCH_CH_CMD_TYPE;
}

/*************************************************
Function:  spi1_headmsg_change
Description: 改变head msg
Input:
Output: 无
Return: 无
Others:
*************************************************/
void spi1_headmsg_change(void)
{
	uint16_t tmp_buf[32]={0};
	tmp_buf[0] = 1;				//数据请求有效,1：有效，0：无效
	tmp_buf[1]=0;			//mvb接收数据
	tmp_buf[2]=1;			//振动信号采集通道，默认１通道，可通过上位机设置通道
	spi1_send_data(SWITCH_CH_CMD_TYPE,tmp_buf,3);
}

/*************************************************
Function:  ad_board_reset
Description: 采集板复位
Input:
Output: 无
Return: 无
Others:
*************************************************/
void ad_board_reset(uint8_t tag,uint8_t cmd)
{
	uint16_t tmp_buf[32]={0};
	tmp_buf[0] = tag;
	tmp_buf[1] = cmd;
	tmp_buf[2] = 1;
	spi2_send_data(RESET_CMD_TYPE,tmp_buf,3);
}

/*************************************************
Function:  ad_board_update_reset
Description: 采集板升级复位
Input:
Output: 无
Return: 无
Others:
*************************************************/
void ad_board_update_reset(uint8_t cmd)
{
	uint16_t tmp_buf[32]={0};
	tmp_buf[0] = AD_BOARD;
	tmp_buf[1] = cmd;
	spi2_send_data(RESET_CMD_TYPE,tmp_buf,2);
}

/*************************************************
Function:  spi1数据发送协议拟定
Description:
Input:
Output: 无
Return: 无
Others:
*************************************************/
void spi1_send_data(SPI_CMD_TYPE type,uint16_t *data,uint16_t len)
{
	uint16_t spi_switch_dat[100]={0};
	uint16_t crc = 0,i=0;
	spi_switch_dat[0] = 0x55;
	spi_switch_dat[1] = 0xaa;
	spi_switch_dat[2] = PRO_HEAD_LEN+len;	//数据长度
	spi_switch_dat[3] = type;				//命令类型
	spi_switch_dat[4] = 0;					//目标地址
	spi_switch_dat[5] = 0;					//源地址

	spi_switch_dat[6] = 0;
	spi_switch_dat[7] = 0;
	spi_switch_dat[8] = 0;
	spi_switch_dat[9] = 0;					//预留

	for(i=0; i<len; i++)
	{
		spi_switch_dat[10+i] = data[i];
	}
	crc=0;
	for(i=0; i<PRO_HEAD_LEN+len-1; i++)
	{
		crc += spi_switch_dat[i];
	}
	spi_switch_dat[PRO_HEAD_LEN+len-1] = crc;

	memmove(default_tx,spi_switch_dat,sizeof(spi_switch_dat));
//	for (i=0; i<20; i++)
//		printf(" 0x%02x",default_tx[i]);
//	printf("\n");
}

/*************************************************
Function:  spi1数据发送协议拟定
Description:
Input:
Output: 无
Return: 无
Others:
*************************************************/
void spi2_send_data(SPI_CMD_TYPE type,uint16_t *data,uint16_t len)
{
	uint16_t spi_switch_dat[100]={0};
	uint16_t crc = 0,i=0;
	spi_switch_dat[0] = 0x55;
	spi_switch_dat[1] = 0xaa;
	spi_switch_dat[2] = PRO_HEAD_LEN+len;	//数据长度
	spi_switch_dat[3] = type;				//命令类型
	spi_switch_dat[4] = 0;					//
	spi_switch_dat[5] = 0;					//

	spi_switch_dat[6] = 0;
	spi_switch_dat[7] = 0;
	spi_switch_dat[8] = 0;
	spi_switch_dat[9] = 0;					//预留

	for(i=0; i<len; i++)
	{
		spi_switch_dat[10+i] = data[i];
	}
	crc=0;
	for(i=0; i<PRO_HEAD_LEN+len-1; i++)
	{
		crc += spi_switch_dat[i];
	}
	spi_switch_dat[PRO_HEAD_LEN+len-1] = crc;

	memmove(default_tx2,spi_switch_dat,sizeof(spi_switch_dat));
}

/*************************************************
Function:  spi_flag_init
Description: 标志位初始化
Input:
Output: 无
Return: 无
Others:
*************************************************/
void spi_flag_init(void)
{

	spi_flag.update_count=0;
	spi_flag.update_flag=0;
	spi_flag.proc1_reset_flag=0;
	spi_flag.proc2_reset_flag=0;
	spi_flag.spi1_err_reset_flag = 0;		//spi异常重启标志
	spi_flag.spi2_err_reset_flag = 0;		//spi异常重启标志

	spi_flag.spi2_recv_reset_flag = 0;		//spi接收到重启消息标志位

	spi_flag.spi2_recv_update_flag = 0;		//spi接收到正常的升级数据标志位
	spi_flag.spi2_update_over_flag = 0;		//spi升级完成标志位

	spi_flag.spi2_update_start_flag = 0;		//spi升级标志位
	spi_flag.spi2_bearing_deal=0;
	spi_flag.spi1_plougon_deal=0;
	spi_flag.port_finish=0;
	spi_flag.spi_comm_reset_flag=0;

	spi_flag.spi_zd_sample_ch=1;//振动信号采集通道默认１，ＰＴＵ可配置采样通道

}



