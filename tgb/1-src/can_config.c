
/********************************************
 * 此文件配置的CAN用作内部处理板和通信板通信
 * ******************/
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <signal.h>
#include <sys/time.h>
#include <linux/can.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/if.h>
#include <linux/can/raw.h>
#include <semaphore.h>
#include "can_config.h"
#include "user_data.h"

#include "pthread_policy.h"

#define PRO_HEAD_LEN2   12
#define CAN_RX_BUF_SIZE2			256
#define CAN_TX_BUF_SIZE2			256
#define CAN_RX_RD_SIZE2 1024
#define CAN_RX_SIZE2 1024
#define RX_LEN_MAX2  1024
#define AF_CAN       PF_CAN
#define CAN_EFF_FLAG2 0x80000000U //扩展帧的标识
#define CAN_RTR_FLAG2 0x40000000U //远程帧的标识
#define CAN_ERR_FLAG2 0x20000000U //错误帧的标识，用于错误检查

#define CAN_LEN2      0x08
#define CAN_AND2      0x07
#define CAN_R_BITS2   0x03
//#define CAN_DEAL_FUN  recv_deal


int can_socket2 = -1;  //can套接字描述符
enum BOOL can_send_init_flag2 = FALSE;
enum BOOL RX_data_deal2 = TRUE;

struct can_frame  frame2;
struct can_tx_queue  //can的发送队列，用于发送数据时组包
{
    sem_t tx_sem;
	uint32_t tx_save;
	uint32_t tx_read;
	uint32_t size;
	struct can_frame msg_buf[CAN_TX_BUF_SIZE2]; //一次最多发送256帧数据
};
typedef struct can_tx_queue *CAN_tx_queue_t;


struct can_rx_queue
{
	sem_t    rx_sem;
	uint32_t rx_save;
	uint32_t rx_read;
	uint32_t size;
	struct can_frame msg_buf[CAN_RX_BUF_SIZE2];
};
typedef struct can_rx_queue *CAN_rx_queue_t;


struct can_tx_queue can_send_queue2; //实例化发送队列
struct can_rx_queue can_recv_queue2; //实例化接收队列

list_t frame_list2;

void can_tx_init2(void);

/*************************************************
Function:  can_send_data
Description: can消息发送函数
Input:  要发送的数据指针:data  数据的长度:len
Output: 无
Return: 无
Others:
*************************************************/
void can_send_data2(uint8_t *data, uint16_t len)
{
	struct can_frame *msg;
	CAN_ID_t can_rx_id;

	uint32_t send_group = 0, send_remain = 0, send_cnt = 0, len_tmp = 0;
	uint32_t i = 0, j = 0, r_index = 0;
	uint32_t multi_flag; //TRUE即为多帧，FALSE即为单帧
    uint32_t send_flag;

	static uint32_t cnt_save=0;

	if (data==NULL || len==0 || can_send_init_flag2==FALSE)
	{
		return;
	}

	can_rx_id.WORD = 0;
	can_rx_id.BITS.souc_id = data[6];
	//can_rx_id.BITS.type = ;
	can_rx_id.BITS.tag_id = data[5];
	can_rx_id.BITS.ack = 0x0;
	//can_rx_id.BITS.cnt = cnt_save++;
	can_rx_id.BITS.reserve = 0x0;
	can_rx_id.BITS.priority = 0x7;

	send_group = len>>CAN_R_BITS2;  //计算多少完整帧数据
	send_remain = len&CAN_AND2;    //计算剩余不足一帧的有多少数据

	if (send_group>1 || (send_group==1 && send_remain>0)) //两帧以上就判断为多帧数据
	{
	    multi_flag = TRUE;
		send_cnt = send_group;
		if (send_remain > 0)
		{
			send_cnt++;
		}
	}
	else
	{
		send_cnt = 1;  //单帧数据，只需要发送一次
		multi_flag = FALSE;
	}
	for (i=0; i<send_cnt; i++)
	{

		msg = &(can_send_queue2.msg_buf[can_send_queue2.tx_save]);
		if (multi_flag)//多帧
		{
			if (i == 0x0)//头帧
			{
			  can_rx_id.BITS.type = MULTI_BEGIN;
			  len_tmp = CAN_LEN2;
			}
			else if (i == send_cnt-1)//尾帧
			{
				can_rx_id.BITS.type = MULTI_END;
				if (send_remain>0)
				{
				  len_tmp = send_remain;
				}
				else
				{
					len_tmp = CAN_LEN2;
				}
			}
			else//中间帧
			{
				can_rx_id.BITS.type = MULTI_MID;
				len_tmp = CAN_LEN2;
			}
		}
		else//单帧
		{
			can_rx_id.BITS.type = SINGLE;
			if (send_group == 1)
			{
				len_tmp = CAN_LEN2;
			}
			else
			{
				len_tmp = send_remain;
			}
		}

		can_rx_id.BITS.cnt = cnt_save++;
		msg->can_id = can_rx_id.WORD | CAN_EFF_FLAG; //对应到stm32上的msg->ExtId，将把每帧的标识传输到通信对方
  	    msg->can_dlc = len_tmp;

  	    for (j=0; j<len_tmp; j++)
  	    {
  	    	msg->data[j] = data[r_index++];
  	    }

		can_send_queue2.tx_save++;
		if(can_send_queue2.tx_save >= can_send_queue2.size)
		{
			can_send_queue2.tx_save = 0;
		}
		send_flag = TRUE;

		if (can_send_queue2.tx_save == can_send_queue2.tx_read)
		{
			can_send_queue2.tx_read ++;
			if (can_send_queue2.tx_read >= can_send_queue2.size)
			{
				can_send_queue2.tx_read = 0;
			}
			send_flag = FALSE;
		}
		if (send_flag)
		{
			sem_post(&can_send_queue2.tx_sem);
		}
	}

}



/*************************************************
Function:  can_tx_thread_entry
Description: can发送线程入口
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
void can_tx_thread_entry2(void *parameter )
{
	struct can_frame *msg;  //一帧can消息的结构体
	uint16_t send_cnt=0;
	uint32_t i;
	int size = 0;
	CAN_tx_queue_t tx_queue = (CAN_tx_queue_t)parameter;
    tx_queue = &can_send_queue2;
    tx_queue->tx_save = 0;
    tx_queue->tx_read = 0;
    tx_queue->size = CAN_TX_BUF_SIZE2;
   // can_tx_init();
    sem_init(&(tx_queue->tx_sem), 0, 0);  //信号量初始化
    can_send_init_flag2 = TRUE;

#ifdef RT_LINUX
    pthread_attr_t attr;       // 线程属性
    struct sched_param sched;  // 调度策略
    printf ("set SCHED_RR policy\n");
    api_set_thread_policy(&attr, SCHED_RR);
#endif

	//can_tx_init();
    while(1)
    {
      /* wait receive */
	  sem_wait(&can_send_queue2.tx_sem);  //一直等待信号量值不为0,并将信号量值减1
      {
		 //feed_dog();
         for (i=0;i<100000;i++); //延时一段时间
         if (tx_queue->tx_read == tx_queue->tx_save)
		 {
             continue;
		 }
         msg = &(tx_queue->msg_buf[tx_queue->tx_read]);
         size = sizeof(tx_queue->msg_buf[tx_queue->tx_read]);
         //printf("msg= %d \r\n",size);
         if(write(can_socket2, msg, size))
         {
    	    tx_queue->tx_read++;
            if(tx_queue->tx_read >= tx_queue->size)
            {
        	  tx_queue->tx_read = 0;
            }
			send_cnt = 0;
          //DEBUG("Can send ok\r\n");
        }
        else
        {
			 send_cnt++;
			 if (send_cnt > 10)
			 {
			   send_cnt = 0;
			   usleep(40);
			 }
            //rt_kDEBUG("no mb temp\r\n", trans_mb_no);
		    sem_post(&can_send_queue2.tx_sem);  //释放发送信号量
         }
      }
    }
}



/*************************************************
Function:  can_tx_init
Description: 初始化can的发送队列
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
void can_tx_init2(void)
{
    CAN_tx_queue_t tx_queue = &can_send_queue2;
    tx_queue->tx_save = 0;
    tx_queue->tx_read = 0;
    tx_queue->size = CAN_TX_BUF_SIZE2;
    sem_init(&(tx_queue->tx_sem), 0, 0);  //信号量初始化
	can_send_init_flag2 = TRUE;
}




/*************************************************
Function:    can_send
Description: 组帧发送can消息
Input:  目标ID:taget
		命令类型:cmd
		发送数据指针:data
		数据长度:len
Output: 无
Return: 无
Others:
*************************************************/
void can_send2(uint8_t taget, uint8_t cmd, uint8_t *data, uint16_t len)
{
    uint8_t can_data[1024];
    uint8_t crc=0;
    uint16_t i=0;
	can_data[0] = 0x55;
	can_data[1] = 0xAA;
	can_data[2] = len+PRO_HEAD_LEN2;
	can_data[3] = (uint8_t)((len+PRO_HEAD_LEN2) >> 8);
	can_data[4] = cmd;   //命令类型
	can_data[5] = taget;  //目标ID
	can_data[6] = BOARD_NUMB;  //源ID
	can_data[7] = 0x00;
	can_data[8] = 0x00;
	can_data[9] = 0x00;
	can_data[10] = 0x00;
	for(i=0; i<len; i++)
	{
		can_data[11+i] = data[i];
	}
	crc=0;
	for(i=0; i<PRO_HEAD_LEN2+len-1; i++)
	{
		crc += can_data[i];
	}
	can_data[PRO_HEAD_LEN2+len-1] = crc;
#ifdef USING_CAN_DEV

	can_send_data2(can_data, PRO_HEAD_LEN2+len);
#endif
}




/*************************************************
Function:    config_can
Description: 配置socket can的相关参数
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
void config_can2()
{


	struct sockaddr_can addr;
	struct ifreq ifr;
	system("ifconfig can1 down");//使能can驱动设备
	//system("ifconfig can1 up");//使能can驱动设备
	system("ip link set can1 type can bitrate 200000 triple-sampling on");
	system("ifconfig can1 up");//使能can驱动设备


	can_socket2 = socket(PF_CAN, SOCK_RAW, CAN_RAW);//创建CAN套接字
	if(can_socket2 == -1)
	{
		printf("---------------------can 2 socket is err-----------------------------\r\n");
	}
	strcpy(ifr.ifr_name, "can1" );
	ioctl(can_socket2, SIOCGIFINDEX, &ifr); //指定 can0 设备
	int loopback = 0; /* 0 = disabled, 1 = enabled (default) */
    setsockopt(can_socket2, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback)); // Setting lookback options
	//DEBUG("set can0 sock loopback unable\n");


	int recv_own_msgs = 0; /* 0 = disabled (default), 1 = enabled */
   setsockopt(can_socket2, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &recv_own_msgs, sizeof(recv_own_msgs)); // Setting receiver own massages options
	//DEBUG("set can0 do not receiver own massage\n");

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind(can_socket2, (struct sockaddr *)&addr, sizeof(addr));//将套接字与 can0 绑定
}





/*************************************************
Function:    protocol_deal
Description: can数据协议解析
Input:  接受的can数据指针:data
		数据的长度:data_len
Output: 无
Return: 无
Others:
*************************************************/
void protocol_deal_mvb(uint8_t *data, uint8_t data_len)
{

	static uint8_t rx_buf[RX_LEN_MAX2];
	static uint32_t write_id=0;
	static uint8_t rx_i;
	static int32_t get_numb;
	static uint8_t crc;
//	static int i;
    if (data)
	{
		rx_i = 0;
		get_numb = data_len;
		while (get_numb--)
		{
			rx_buf[write_id++] = data[rx_i++];
			if (write_id >= RX_LEN_MAX2)
			{
				write_id = 0;
			}
			if (write_id >= 1)
			{
				if (rx_buf[0] == 0x55)
				{
				   if (write_id >= 2)
				   {
					  if (rx_buf[1] == 0xAA)
					  {
					    if (write_id >= 4)
					    {
				 	    	if (rx_buf[2] < RX_LEN_MAX2)
					    	{
						       if (write_id == rx_buf[2])
						       {
							       if (crc == rx_buf[write_id-1])
							       {
							    	  //printf("111111111111111111111111111111111111\r\n");
									  //recv_deal(rx_buf,write_id);
								      write_id = 0;	//重新接收
							       }
							       else
							       {
								      write_id = 0;
							       }
							  }
						      else
						      {
							     if (write_id != 4)
							     {
							    	 crc += rx_buf[write_id-1];
							     }
							     else
							     {
							    	 crc = rx_buf[0] + rx_buf[1] + rx_buf[2] + rx_buf[3];
							     }
						     }
						 }
						 else
						 {
								write_id = 0;
						 }
					  }
					}
					else
					{
					 write_id = 0;
					}
				  }
				}
				else
				{
					 write_id = 0;
				}

			}
		}
	}

}



/*************************************************
Function:    can_recv_thread_entery
Description: can接收线程入口
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void can_recv_thread_entery2(void *parameter)  //CAN接收线程入口
{
	    uint8_t can_data[8];
		CAN_ID_t can_rx_id;
#ifdef RT_LINUX
	    pthread_attr_t attr;       // 线程属性
	    struct sched_param sched;  // 调度策略
	    printf ("set SCHED_RR policy\n");
	    api_set_thread_policy(&attr, SCHED_RR);
#endif
		while(1)
		{
			int nbytes = read(can_socket2, &frame2, sizeof(frame2)); //接收报文
			can_rx_id.WORD = frame2.can_id;
			if(nbytes > 0 &&(can_rx_id.BITS.tag_id == MAIN_BOARD)) //只会接收到通信板的报文
			{
				memcpy(can_data,frame2.data,frame2.can_dlc);
				protocol_deal_mvb(can_data,frame2.can_dlc);
			}
			usleep(100);
		}
}




/*************************************************
Function:    init_can_thread
Description: 初始化创建can的发送和接收线程
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
int init_can_thread2(void) //can接收线程初始化
{
	pthread_t can_recv_id,can_tx_id;
	int ret;
	/****************创建CAN接收数据的线程****************/
	ret=pthread_create(&can_recv_id,NULL,(void *)can_recv_thread_entery2,NULL);
	if(ret!=0){
	DEBUG ("Create can recv thread error!\n");
	return ret;
	}

	/****************创建CAN发送数据的线程****************/
	ret=pthread_create(&can_tx_id,NULL,(void *)can_tx_thread_entry2,NULL);
	if(ret!=0){
	DEBUG ("Create can tx thread error!\n");
	return ret;
	}

	return 0;
}




/*************************************************
Function:    init_can_socket2
Description: 配置和初始化can
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
int init_can_socket()
{
	int ret = -1;
	//init_speed_para();

	config_can2();
	ret = init_can_thread2(); //can线程初始化


	return ret;
}
