/*
 * spi_app.c
 *
 *  Created on: Mar 22, 2019
 *      Author: www
 */
#include <pthread.h>
#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>
#include <semaphore.h>
#include "ad7606.h"
#include "spi_app.h"
#include "user_data.h"
#include "udp_client.h"
#include "sec_app.h"
#include "vibr_diagnosis.h"

/*-------- external para ------------*/
uint16_t default_tx[2000]; 	//spi发送buffer，为了给从机提供时钟，必须要有发送数据的动作
uint16_t default_tx2[2000];  //spi发送buffer，为了给从机提供时钟，必须要有发送数据的动作

/*--------------- public para ----------------*/
struct spi_derail *spi1_recv_derail;

uint16_t *temp_buffer=NULL;

static void spi1_data_init(void)
{

    spi1_recv_derail=(struct spi_derail*)malloc(sizeof(struct spi_derail));
    if(spi1_recv_derail==NULL)
        printf("spi1_recv_derail malloc fail\r\n");
    else
        memset(spi1_recv_derail,0,sizeof(struct spi_derail));

    temp_buffer=( uint16_t*)malloc(DAT_PACKAGE_SIZE*2);
}

/*************************************************
Function:    spi1_thread_entry
Description: spi1处理线程，接收采集的振动数据
Input:
Output:
Return:
Others:
*************************************************/
void spi1_thread_entry(void)
{

	spi1_data_init();

	spi_flag_init();

    init_spi_device();

    while(1)
    {
    	sleep(1);
   	    //如果算法完成,请求振动数据
    	if((spi_flag.update_flag == 0))
    	{
    		//printf("spi send dat\n");
    		spi1_headmsg_change();
			spi1_read_data();
    	}
    }
}
/*************************************************
Function:    spi2_thread_entry
Description: spi处理线程
Input:
Output:
Return:
Others:
*************************************************/
void spi2_thread_entry(void)
{
	uint32_t delay_i=0;

    init_spi2_device();

    while(1)
    {
    	sleep(1);
    	if((spi_flag.update_flag == 0))
    	{
    		if((spi_flag.spi1_err_reset_flag>60)||(spi_flag.spi2_err_reset_flag>60))
    		{
    			ad_board_reset(AD_BOARD,0xff);
    			printf("spi:--->reset ad board....\n");
    			spi_flag.spi_comm_reset_flag++;
    			if(spi_flag.spi_comm_reset_flag>5)
    			{
    				spi_flag.spi1_err_reset_flag=0;
					spi_flag.spi2_err_reset_flag=0;
					spi_flag.spi_comm_reset_flag=0;
    			}
    			spi2_read_data();
    		}
    		//printf("delay_i:%d-%d\n",delay_i,spi_flag.spi1_err_reset_flag);
    	}
    	delay_i++;
    }
}
/*************************************************
Function:    init_spi_thread
Description: 初始化创建spi线程
Input:
Output:0代表成功,否则失败
Return:
Others:
*************************************************/
int init_spi1_thread(void)
{
	int ret;
	pthread_t spi1_thread_id;

	ret = pthread_create(&spi1_thread_id, NULL, (void *)spi1_thread_entry, NULL);
	if(ret!=0)
	{
		printf("spi2 pthread_create error\r\n");
	}
	return ret;
}
/*************************************************
Function:    init_spi_thread
Description: 初始化创建spi线程
Input:
Output:0代表成功,否则失败
Return:
Others:
*************************************************/
int init_spi2_thread(void)
{
	int ret;
	pthread_t spi2_thread_id;

	ret = pthread_create(&spi2_thread_id, NULL, (void *)spi2_thread_entry, NULL);
	if(ret!=0)
	{
		printf("spi2 pthread_create error\r\n");
	}
	return ret;
}



/*************************************************
Function:    spi_update_func
Description: spi升级函数
Input:	1代表允许升级,0代表禁止升级
Output: 1代表升级成功,0代表升级失败
Return:
Others:
*************************************************/
uint8_t spi_update_func(uint8_t board_id)
{
	uint32_t update_err_cnt=0;
	uint16_t tmp_buf[2000] = {0},buffer[10]={0};
	uint16_t tmp_data[10000] = {0};
	uint16_t read_file_data[10000] = {0};
	int file_fd = -1,ret=0;
	spi_flag.update_count=0;
	spi_flag.spi2_recv_reset_flag = 0;									//spi接收到重启消息标志位
	spi_flag.spi2_recv_update_flag = 0;									//spi接收到正常的升级数据标志位
	spi_flag.spi2_update_over_flag = 0;									//spi升级完成标志位
	spi_flag.update_flag=1;//升级标志
	spi_flag.spi_send_cnt=0;

	char update_name[128];

	sleep(1);

	printf("********* start spi2_update_func... *********\r\n");
	while(spi_flag.spi2_recv_reset_flag==0)				//循环请求复位,直至接收到复位消息,500ms请求一次
	{
		ad_board_update_reset(0x01);					//请求复位
		spi2_read_data();

		sleep(1);
		update_err_cnt++;
		if(update_err_cnt==20)
		{
			printf("AD_BOARD reset update error\r\n");
			spi_flag.update_flag=0;//升级标志
			return 0;//err
		}
	}

	spi_flag.update_count=0;

	if(spi_flag.spi2_recv_reset_flag)
	{
		spi_flag.spi2_recv_reset_flag=0;
		sleep(1);
		printf("AD_BOARD reset update CMD\r\n");

		ad_board_update_reset(0xff);					//开始复位
		spi2_read_data();

		sleep(6);										//延时5s,等待重启

		{
			if(board_id==AD_BOARD)
				strcpy(update_name,"/media/LH_DATA/local_config/app/adb.bin");
		}

		file_fd = open(update_name,O_RDWR);		//打开升级文件
		if(file_fd<0)
		{
			printf("open %s faild\r\n",update_name);

			spi_flag.update_flag=0;//升级标志
			spi_flag.update_count=0;
			return 0;
		}
		printf("open %s success\r\n",update_name);

		buffer[0] = IAP_READY_TYPE;
		spi2_send_data(UPDATE_CMD_TYPE,buffer,1);//ready
		spi2_read_data();
		sleep(1);
		spi_flag.spi_send_cnt=0;
		//20k循环读取发送文件
		while((ret=read(file_fd,read_file_data,sizeof(uint16_t)*8000))>0)	//收到复位消息后,开始发送升级包
		{
			spi_flag.spi2_recv_update_flag = 0;
			spi_flag.spi_send_cnt++;
			printf("spi_send_cnt = %d\r\n",spi_flag.spi_send_cnt);
			buffer[0] = START_CMD_TYPE;
			buffer[1] = spi_flag.spi_send_cnt;
			buffer[2] = ret;
			spi_flag.update_count=0;

			while(spi_flag.spi2_recv_update_flag == 0)
			{
				spi2_send_data(UPDATE_CMD_TYPE,buffer,3);					//组包发送协议头
				spi2_transfer_data(default_tx2,tmp_buf,4000);
				memmove(&tmp_data[0],tmp_buf,4000);							//获取反馈数据
				memset(tmp_buf,0,sizeof(tmp_buf));							//清除buf

				for(int i=0;i<4;i++)											//发送其他额外数据
				{
					memset(default_tx2,0,sizeof(default_tx2));
					memmove(default_tx2,read_file_data+2000*i,4000);

					spi2_transfer_data(default_tx2,tmp_buf,4000);
					memmove(&tmp_data[i*2000+2000],tmp_buf,4000);
					memset(tmp_buf,0,sizeof(tmp_buf));						//清除buf
				}
				spi2_recev_data_deal(tmp_data);								//判断是否获取正确通道号及通道数据
				sleep(1);

				update_err_cnt++;
				if(update_err_cnt==15)
				{
					printf("update error11111\r\n");
					spi_flag.update_flag=0;
					return 0;
				}
			}
			update_err_cnt=0;
			memset(read_file_data,0,sizeof(read_file_data));							//清除buf
			spi_flag.spi2_recv_update_flag = 0;
		}
		close(file_fd);

		update_err_cnt = 0;


		printf("upgrade data send ok!\n\r");

		while(spi_flag.spi2_update_over_flag==0)
		{
			//发送完成,结束升级命令
			buffer[0] = END_CMD_TYPE;
			buffer[1] = spi_flag.spi_send_cnt;
			buffer[2] = board_id;
			spi2_send_data(UPDATE_CMD_TYPE,buffer,3);

			spi2_read_data();

			update_err_cnt++;
			if(update_err_cnt==20)
			{
				printf("update error22222222\r\n");
				spi_flag.update_flag=0;
				return 0;
			}
			sleep(1);
		}

		if((board_id==PRE_BOARD2)||(board_id==PRE_BOARD1))
		{
			update_err_cnt=0;
			while(spi_flag.port_finish==0)
			{
				sleep(1);
				//发送完成,结束升级命令
				memset(default_tx2,0,sizeof(default_tx2));
				buffer[0] = ERR_CMD_TYPE;
				spi2_send_data(UPDATE_CMD_TYPE,buffer,1);
				spi2_read_data();
				//printf("update4444444444\r\n");
				update_err_cnt++;
				if(update_err_cnt==30)
				{
					printf("update error333333\r\n");
					spi_flag.update_flag=0;
					return 0;
				}

			}
			spi_flag.update_flag=0;
			if(spi_flag.port_finish==0xaa)
			{
				printf("update success\r\n");
				return 1;
			}
			else
			{
				printf("update failed\r\n");
				return 0;
			}
		}

		printf("update success\r\n");
		spi_flag.update_flag=0;
		return 1;
	}

	spi_flag.update_flag=0;
	return 0;

}


