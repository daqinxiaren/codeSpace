
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

#include "sys/time.h"

#include "crc_table.h"
#include "user_data.h"
#include "hdd_save.h"
#include "can_config.h"
#include "udp_client.h"
#include "app_paras_config.h"
#include "spi_app.h"
#include "ad7606.h"

/*----------------- pubilc para --------------*/
struct sockaddr_in inside_mcast1_addr;//time speed
struct sockaddr_in inside_mcast2_addr;//tz

struct sockaddr_in record_sockaddr;//
struct sockaddr_in txb_sockaddr;//
struct sockaddr_in clb_sockaddr;//
struct sockaddr_in tclb1_sockaddr;//
struct sockaddr_in tclb2_sockaddr;//
struct sockaddr_in zxb_sockaddr;//

struct sockaddr_in local_addr;
static char local_ip[15];

int inside_send_fd_socket;

union sys_dev sysdev;
struct TCMS_PUBLIC_INFO tcms_pub_info;

/*************************************************
Function:  sum_check_u16
Description: u16和校验
Input:	crc_len data
Output:
Return: check_sum
Others:
*************************************************/
uint16_t sum_check_u16(uint8_t *data,uint16_t crclen)
{
	int i;
	uint16_t check_sum=0;
	for(i=0;i<crclen;i++)
	{
		check_sum+=	data[i];
	}
	 return check_sum;
}


/**
 * function     :udp_send
 * description  :udp发送函数，可设置超时时间和重发次数
 * input        :buff     -要发送的数据
 *               buff_size-要发送的数据长度
 *               _addr    -要发送的目标地址，可以是单播地址，也可以是组播地址
 *               _addr_len-目标地址的长度
 *               time_out -超时时间，若为sec和usec为0表示没有超时判断，不重发
 *              resend_cnt-超时重发次数
 * output       :none
 * return       :0-ok 非0-err
 * others       :none
 */
static void udp_send(uint8_t *buff, uint16_t buff_size, struct sockaddr *__addr, socklen_t __addr_len)
{
//	sendto(send_fd_socket, &tcms_msg1, sizeof(struct TCMS_PUBLIC_INFO), 0, (struct sockaddr *)&train_info_group_addr, sizeof(train_info_group_addr));
    sendto(inside_send_fd_socket, buff, buff_size, 0, __addr, __addr_len);
}

void ucast_send(uint8_t *buff, uint16_t buff_size,uint8_t target)
{
	static uint16_t counter=0;
	uint16_t len=PROT_HEAD_LEN+buff_size;
	uint8_t sendbuf[len];
	struct SEND_DATA_HEAD heads;
	heads.head[0]=0xaa;
	heads.head[1]=0x50;
	heads.len[0]=(uint8_t)(len>>8);
	heads.len[1]=(uint8_t)(len);

	heads.company_id = 0x4;
	heads.board_id = LOCAL_BOARD;

	counter++;
	heads.life_signal[0]= (uint8_t)(counter>>8);
	heads.life_signal[1]= (uint8_t)(counter);

	memmove(sendbuf,&heads,sizeof(struct SEND_DATA_HEAD));
	memmove(&sendbuf[sizeof(struct SEND_DATA_HEAD)],buff,buff_size);
	uint16_t check_sum=sum_check_u16(sendbuf,len-2);

	sendbuf[len-2]=(uint8_t)(check_sum>>8);
	sendbuf[len-1]=(uint8_t)(check_sum);

	udp_send(sendbuf,len,(struct sockaddr *)&record_sockaddr,sizeof(record_sockaddr));

}

void udp_cmd_ack(uint8_t cmd,uint8_t flag)
{
	uint8_t buf[3];
	buf[0]=cmd;
	buf[1]=LOCAL_BOARD;
	buf[2]=flag;
	ucast_send(buf, 3,0);
}



void inside_mcast_send(uint8_t *buff, uint16_t buff_size,struct sockaddr_in target)
{
	static uint16_t counter=0;
	uint16_t len=26+buff_size;
	uint8_t sendbuf[len];
	struct SEND_DATA_HEAD heads;
	heads.head[0]=0xaa;
	heads.head[1]=0x50;
	heads.len[0]=(uint8_t)(len>>8);
	heads.len[1]=(uint8_t)(len);

	heads.company_id = 0x4;
	heads.board_id = LOCAL_BOARD;

	counter++;
	heads.life_signal[0]= (uint8_t)(counter>>8);
	heads.life_signal[1]= (uint8_t)(counter);

	memmove(sendbuf,&heads,sizeof(struct SEND_DATA_HEAD));
	memmove(&sendbuf[sizeof(struct SEND_DATA_HEAD)],buff,buff_size);
	uint16_t check_sum=sum_check_u16(sendbuf,len-2);

	sendbuf[len-2]=(uint8_t)(check_sum>>8);
	sendbuf[len-1]=(uint8_t)(check_sum);

	udp_send(sendbuf,len,(struct sockaddr *)&target,sizeof(struct sockaddr_in));
//	printf("tz --->send sucess!\n");

}

void tz_msg_send(struct send_local_st *local)
{
	uint8_t buff[160];
	local->sys.byte=sysdev.byte;
	memmove(&buff[2],(uint8_t *)local,sizeof(struct send_local_st));
	inside_mcast_send(buff, sizeof(struct send_local_st)+2,inside_mcast2_addr);
}

/**
 * 单播消息接收处理
 * */
static void ucast_recv_msg_deal(uint8_t *data, int len, struct sockaddr_in client_addr)
{
	char ip[16];
	strcpy(ip,inet_ntoa(client_addr.sin_addr));
	if(strcmp(ip,local_ip)==0)
		return;
	if((*(uint16_t*)&data[0]!=0x50aa))
		return;

	uint16_t host_sum = sum_check_u16(data, len-2);
	uint16_t net_sum = (data[len-2]<<8)+data[len-1];
	if ((host_sum == net_sum)&&(data[25]==LOCAL_BOARD))
	{
		switch(data[24])
		{
		  case APP_UPDATE_CMD:
			  //同步数据
			  system(RSYNC_UPDATE_GET);
			  //拷贝数据
			  if(data[26]==0)
			  {
				  printf("tclb iap---\n");
				  system("cp /media/LH_DATA/update/update_app/TFDSN-TCLB  /media/LH_DATA/local_config/app -rf");
				  udp_cmd_ack(APP_UPDATE_CMD,0);
			  	  sleep(2);
			  	  system("reboot -nf");
			  }
			  else if(data[26]==1)
			  {
				  printf("ad iap---\n");
				  system("cp /media/LH_DATA/update/update_app/adb.bin  /media/LH_DATA/local_config/app -rf");
				  if(spi_update_func(AD_BOARD))
					  udp_cmd_ack(APP_UPDATE_CMD,1);
				  else
					  udp_cmd_ack(APP_UPDATE_CMD,0xff);
			  }
			  break;

		  case SYSTEM_REBOOT_CMD:
			     printf(" SYSTEM_REBOOT_CMD---\n");
			     system(RSYNC_UPDATE_GET);

	    		  if(access(SYSTEM_CONFIG_SH_PATH_MKDIR, F_OK)== 0)//system config
		    	  {
		    			  system("cp /media/LH_DATA/update/update_system/mkdir.sh /usr/local/ -rf");
		    			  system("rm -rf /media/LH_DATA/update/update_system/mkdir.sh ");
		    			  system("chmod 777 /usr/local/mkdir.sh ");
		    			  printf("update /usr/local/mkdir.sh file success!\n");
		    	  }

	    		  if(access(SYSTEM_CONFIG_FILE_PATH, F_OK)== 0)//system config
		    	   {
		    			  system("cp /media/LH_DATA/update/update_system/tclb_profile /etc/ -rf");
		    			  system("mv /etc/tclb_profile /etc/profile ");
		    			  system("rm -rf /media/LH_DATA/update/update_system/tclb_profile ");
		    			  printf("update /etc/profile file success!\n");
		    	   }

	    		  if(access(SYSTEM_CONFIG_DTB_PATH, F_OK)== 0)//system config
		    	   {
		    			  system("cp /media/LH_DATA/update/update_system/am335x-boneblack.dtb /boot/ -rf");
		    			  system("chmod 777 /boot/am335x-boneblack.dtb ");
		    			  system("rm -rf /media/LH_DATA/update/update_system/am335x-boneblack.dtb ");
		    			  printf("update /boot/am335x-boneblack.dtb success!\n");
		    	   }

	    		  sleep(2);
				  system("reboot -nf");

			  break;

		  case CONFIG_PARA_SET_CMD:
			  //同步数据
			  system(RSYNC_UPDATE_GET);
			  system("cp /media/LH_DATA/update/updat_config/*  /media/LH_DATA/local_config/config/ -rf");
			  udp_cmd_ack(CONFIG_PARA_SET_CMD,0);
		  	  sleep(1);
		  	  system("reboot -nf");
			  break;

		  case CONFIG_PARA_READ_CMD:
			  system("cp /media/LH_DATA/local_config/config/*  /media/LH_DATA/update/updat_config/ -rf");
			  //同步数据
			  system(RSYNC_UPDATE_PUT);
			  udp_cmd_ack(CONFIG_PARA_SET_CMD,0);
			  break;
		  case CONFIG_DERAIL_CMD:
			  printf("----  CONFIG_DERAIL_CMD  alarm state %d !!!\n",data[26]);
			  if (1 == data[26])
			  {
				  bjb_ctr(1);//控制报警板报警
			  }
			  else if (0 == data[26])
			  {
				  bjb_ctr(0);//控制报警板清除报警
			  }
			  break;
		  case CONFIG_SAFE_CMD:
			  printf("----  CONFIG_SAFE_CMD  safe state %d !!!\n",data[26]);
			  bjb_safe_ctr(1);
		  	  break;
//		  case CONFIG_SPEED_CMD:
//			  printf("----  CONFIG_SPEED_CMD  safe state %d !!!\n",data[26]);
//			  if (1 == data[26])
//			  {
//				  speed_set(1);//0速灯亮
//			  }
//			  else if (0 == data[26])
//			  {
//				  speed_set(0);//0速灯灭
//			  }
		  default:
			  break;
		}
	}
}

/**
 * function     :udp_ucast_recv
 * description  :接收单播消息
 * input        :none
 * output       :none
 * return       :0-ok 非0-err
 * others       :none
 */
void udp_ucast_recv(void *ptr)
{
    struct sockaddr_in client_addr;
    struct sockaddr_in my_addr;
    socklen_t len = sizeof(client_addr);
    uint8_t udp_recv_buff[1024];

    int recv_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    /**socket复用 */
    int reuse = 1; //必须赋值为非零常数
    if (setsockopt(recv_sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
		perror("reusing socket failed");
		return;
    }

    /**绑定本地地址 */
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port =local_addr.sin_port;    //单播端口，表示主机接收的udp端口
//	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memcpy(&my_addr.sin_addr.s_addr,&local_addr.sin_addr.s_addr,sizeof(my_addr.sin_addr.s_addr));


	// 允许广播数据
	const int on = 1;
	if(setsockopt(inside_send_fd_socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
	{
		rt_kprintf("add broadcast group failed!\n");
		return ;
	}
	int err = bind(recv_sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr));
	if (err != 0)
    {
		perror("bind");

    }
	printf("\t->bind ucast recv socket success\n");

	while (1)
    {
		int r = recvfrom(recv_sock_fd, udp_recv_buff, sizeof(udp_recv_buff) - 1, 0, (struct sockaddr *)&client_addr, &len);
		if (r > 0)
        {
			printf("recv jlb  ucast cmd:\n");
			for(int i=0;i<r;i++)
			{
				if (i!=0 && i%20==0) printf("\n");
				printf("%x ",udp_recv_buff[i]);
			}
			printf("\n");

			ucast_recv_msg_deal(udp_recv_buff, r, client_addr);

			app_paras_config_get_data(udp_recv_buff, r, client_addr);
        }
    }
}

/**
 *  组播消息接收处理
 * */
void mcast_recv_msg_deal(uint8_t *data, int len, struct sockaddr_in addr)
{
	char ip[16];
	strcpy(ip,inet_ntoa(addr.sin_addr));
	if(strcmp(ip,local_ip)==0)
		return;
//	printf("%s \n",ip);
//	for(int i=0;i<50;i++)
//		printf("%x ",data[26+i]);
//	printf("\n");

	if((*(uint16_t*)&data[0]!=0x50aa))
		return;
	uint16_t host_sum = sum_check_u16(data, len-2);
	uint16_t net_sum = (data[len-2]<<8)+data[len-1];
	if ((host_sum == net_sum)&&(data[5]==REC_BOARD))
	{
		//公共信息获取
		memcpy((uint8_t*)&tcms_pub_info,&data[26],sizeof(struct TCMS_PUBLIC_INFO));
		printf("public msg :%d-%d-%d-%d:%d:%d\n",tcms_pub_info.year,tcms_pub_info.mon,tcms_pub_info.day,tcms_pub_info.hour,tcms_pub_info.min,tcms_pub_info.sec);
		//printf("valid_flag:%x\n",info.valid_flag.byte);
		if(tcms_pub_info.valid_flag.bits.time_valid)
		{
			//printf("valid_flag:%x\n",info.valid_flag.byte);
			if(time_msg_valid_deal(&tcms_pub_info.year)==1)
			{
				syncflag.time_update_flag=1;
				sync_file_work(syncflag.time_update_flag);
			}

		}

		if(tcms_pub_info.valid_flag.bits.wheel_valid)
		{
			public_msg.wheel[0]=tcms_pub_info.wheel[(LOCAL_TRAIN_NUM-1)*2];//轮径信息
			public_msg.wheel[1]=tcms_pub_info.wheel[(LOCAL_TRAIN_NUM-1)*2+1];
			//printf(" wheel: %d-%d  \n",public_msg.wheel[0],public_msg.wheel[1]);
		}

		if(tcms_pub_info.valid_flag.bits.speed_valid)
		{
			public_msg.speed[0]=tcms_pub_info.speed[0];//当前速度
			public_msg.speed[1]=tcms_pub_info.speed[1];
//			printf(" speed: %d-%d  \n",public_msg.speed[0],public_msg.speed[1]);
		}
		if (tcms_pub_info.valid_flag.bits.speed_0)
		{
			speed_set(1);
//			led_speed(1);
		}
		else
		{
			speed_set(0);
//			led_speed(0);
		}

		public_msg.curr_id[0]=tcms_pub_info.curr_id[0];//当前站id
		public_msg.curr_id[1]=tcms_pub_info.curr_id[1];

		public_msg.next_id[0]=tcms_pub_info.next_id[0];//下有站id
		public_msg.next_id[1]=tcms_pub_info.next_id[1];
		syncflag.comm_counter=0;
		//printf(" 1id: %d-%d  2id: %d-%d\n",public_msg.curr_id[0],public_msg.curr_id[1],public_msg.next_id[0],public_msg.next_id[1]);
	}
}



/**
 * function     :udp_mcast_recv
 * description  :接收组播消息
 * input        :none
 * output       :none
 * return       :none
 * others       :none
 */
void udp_mcast_recv1(void *ptr)
{
    struct sockaddr_in my_addr;
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    uint8_t udp_recv_buff[2048];
    int recv_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    /**socket复用 */
    int reuse = 1; //必须赋值为非零常数
    if (setsockopt(recv_sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        perror("reusing socket failed");
        return;
    }

    /**绑定本地地址 */
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = inside_mcast1_addr.sin_port; //组播端口
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);     //本地任意地址 inet_addr("192.168.40.100");INADDR_ANY

    int err = bind(recv_sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if (err != 0)
    {
        perror("bind");
    }

    /*设置回环许可:当接收者加入到一个多播组以后，再向这个多播组发送数据，这个字段的设置是否允许再返回到本身*/
    int loop = 0;
    err = setsockopt(recv_sock_fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
    if (err < 0)
    {
        perror("setsockopt():IP_MULTICAST_LOOP");
        return;
    }

    /**默认情况下，多播报文的ＴＴＬ被设置成了１，也就是说到这个报文在网络传送的时候，它只能在自己所在的网络传送，当要向外发送的时候，路由器把ＴＴＬ减１以后变成了０，这个报文就已经被Discard了*/
    unsigned char ttl = 1;
    err = setsockopt(recv_sock_fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
    if (err < 0)
    {
        perror("setsockopt():IP_MULTICAST_TTL");
        return;
    }

    /**参数addr是希望多播输出接口的IP地址，使用INADDR_ANY地址回送到默认接口。 */
    struct in_addr addr;
    addr.s_addr = htonl(INADDR_ANY);
    setsockopt(recv_sock_fd, IPPROTO_IP, IP_MULTICAST_IF, &addr, sizeof(addr));

    /*将本机加入多播组*/
    struct ip_mreq mreq;                                          /*加入多播组*/
    memcpy(&mreq.imr_multiaddr.s_addr,&inside_mcast1_addr.sin_addr.s_addr,sizeof(mreq.imr_multiaddr.s_addr));
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);     /*网络接口为默认INADDR_ANY*/
    err = setsockopt(recv_sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if (err < 0)
    {
        perror("setsockopt():IP_ADD_MEMBERSHIP");
        return;
    }
    printf("\t->bind mcast1 recv socket success\n");
   while (1)
    {
      int r = recvfrom(recv_sock_fd, udp_recv_buff, sizeof(udp_recv_buff) - 1, 0, (struct sockaddr *)&client_addr, &len);
      if (r > 0)
        {
           mcast_recv_msg_deal(udp_recv_buff, r, client_addr);
        }
    }
}


/**
 * function     :init_udp
 * description  :初始化udp，建立发送套接字，端口复用，板卡IP地址初始化，绑定本地端口，单播、组播接收线程初始化
 * input        :none
 * output       :none
 * return       :0-ok 非0-err
 * others       :none
 */
int init_udp(void)
{
	char str[256];
#if LOCAL_BOARD==TCLB1_BOARD
	memcpy(local_ip,comm_para.ip,16);
#else
	strcpy(local_ip,TCLB2_BOARD_ADDR);
#endif

	//内部配置IP和掩码
	sprintf(str,"%s %s %s %s","ifconfig eth0",comm_para.ip,"netmask",comm_para.mask);
	system(str);
	//printf("local_ip:%s\n",str);

	//设置内部组播
	sprintf(str, "%s %s %s %s", "route add -net ", INSIDE_MCAST1_ADDR, "netmask", "255.255.255.255 eth0");
	system(str);
	//printf("inside mcast-->1=%s\n",str);
	sprintf(str, "%s %s %s %s", "route add -net ", INSIDE_MCAST2_ADDR, "netmask", "255.255.255.255 eth0");
	system(str);
	//printf("inside mcast-->2=%s \r\n",str);
	sprintf(str,"%s %s %s","route add -net","255.255.255.255","netmask 255.255.255.255 dev eth0 metric 1");
	system(str);

	printf("\n[USING_PARA]:\n");
	printf("\t->eth0_local_ip:%s-%d\n",local_ip,INSIDE_NET_PORT);
	printf("\t->eth0_inside_mcast1:%s-%d\n",INSIDE_MCAST1_ADDR,INSIDE_MCAST1_PORT);
	printf("\t->eth0_inside_mcast2:%s-%d\n",INSIDE_MCAST2_ADDR,INSIDE_MCAST2_PORT);

	/*设置内部组播1*/
	memset(&inside_mcast1_addr, 0, sizeof(inside_mcast1_addr));         /*初始化IP多播地址为0*/
	inside_mcast1_addr.sin_family = AF_INET;                                   /*设置协议族类行为AF*/
	inside_mcast1_addr.sin_addr.s_addr = inet_addr(INSIDE_MCAST1_ADDR); /*设置多播IP地址*/
	inside_mcast1_addr.sin_port = htons(INSIDE_MCAST1_PORT);            /*设置多播端口*/
	/*设置内部组播2*/
	memset(&inside_mcast2_addr, 0, sizeof(inside_mcast2_addr));         /*初始化IP多播地址为0*/
	inside_mcast2_addr.sin_family = AF_INET;                                   /*设置协议族类行为AF*/
	inside_mcast2_addr.sin_addr.s_addr = inet_addr(INSIDE_MCAST2_ADDR); /*设置多播IP地址*/
	inside_mcast2_addr.sin_port = htons(INSIDE_MCAST2_PORT);            /*设置多播端口*/

	/*设置内部记录板单播端口*/
	memset(&record_sockaddr, 0, sizeof(record_sockaddr));         /*初始化IP单播地址为0*/
	record_sockaddr.sin_family = AF_INET;                                   /*设置协议族类行为AF*/
	record_sockaddr.sin_addr.s_addr = inet_addr(REC_BOARD_ADDR); /*设置单播IP地址*/
	record_sockaddr.sin_port = htons(INSIDE_NET_PORT);            /*设置单播端口*/

	/*设置本地单播端口*/
	memset(&local_addr, 0, sizeof(local_addr));         /*初始化IP单播地址为0*/
	local_addr.sin_family = AF_INET;                                   /*设置协议族类行为AF*/
	local_addr.sin_addr.s_addr = inet_addr(local_ip); /*设置单播IP地址 192.168.1.14 */
	local_addr.sin_port = htons(INSIDE_NET_PORT);            /*设置单播端口*/

	/*建立发送套接字*/
	inside_send_fd_socket = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
   if (inside_send_fd_socket == -1)
	{
		perror("socket()");
		return -1;
	}
   /**socket复用 */
   int reuse = 1; //必须赋值为非零常数复用才有效
   if (setsockopt(inside_send_fd_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
	{
		perror("reusing socket failed");
		return -1;
	}
	/**绑定本地地址 */
   int err = bind(inside_send_fd_socket, (struct sockaddr *)&local_addr, sizeof(local_addr));
   if (err != 0)
	{
		perror("bind faild\n");
		return -1;
	}

    //接收内部单播消息等
	pthread_t inside_udp_recv_ucast_thread;
	printf("\n[START_NET]:\n");
    //接收从机单播消息如原始数据等
   int ret = pthread_create(&inside_udp_recv_ucast_thread, NULL, (void *)&udp_ucast_recv, NULL);
   if (ret != 0)
    {
	   perror("pthread_create");
    }

	//接收列车公共消息
	pthread_t inside_udp_mcast1_thread;
	ret = pthread_create(&inside_udp_mcast1_thread, NULL, (void *)&udp_mcast_recv1, NULL);
	if (ret != 0)
	{
		perror("pthread_create");
	}

	//printf("init_udp success\r\n");

    return 0;
}

