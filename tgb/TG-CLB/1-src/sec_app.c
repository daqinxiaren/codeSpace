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
#include <semaphore.h>
#include <linux/can.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can/raw.h>
#include "user_data.h"
#include "hdd_save.h"
#include "bearing_diagnosis.h"
#include "ploygon_diagnosis.h"
#include "vibr_diagnosis.h"
#include "pthread_policy.h"

#include "sec_app.h"
#include "spi_app.h"
#include "watchdog.h"
#include "gpio_app.h"

/*************************************************
Function:    sec_thread_entry
Description: 自检和秒线程
Input:
Output:
Return:
Others:
*************************************************/
void sec_thread_entry()
{

   uint32_t delay_i=0,count = 0;
	uint8_t val;
	uint8_t v1_val=0;
	uint8_t v2_val=0;
	uint8_t bjb_val = 0;

	gpio_app_init();

	while(1)
	{
		delay_i++;
		count++;
		//feed_dog();//喂狗
		sleep(1);
		led_sys(count);

		if(speed_check(&val)==0)
		{
			//这里把检测状态取反，方便记录板判断状态
			if(val)
				sysdev.bits.speed=0;
			else
				sysdev.bits.speed=1;
			printf("gpio_read   speed_check val= %d\n",sysdev.bits.speed);
		}


		if(v1_check(&v1_val)==0)
			sysdev.bits.v1_err=v1_val;
		if(v2_check(&v2_val)==0)
			sysdev.bits.v2_err=v2_val;
		bjb_check(&bjb_val);//检查脱轨切除状态
		printf("gpio_read   bjb_check val= %d\n",bjb_val);
		if (bjb_val)
		{
			sysdev.bits.derail_del = 1;
		}
		else
		{
			sysdev.bits.derail_del = 0;
		}

		if(delay_i>5)
		{
			local_st.sys.byte=sysdev.byte;
//			tz_msg_send(&local_st);
		}

		//printf("v1-%d v2-%d ad-%d---\n",sysdev.bits.v1_err,sysdev.bits.v2_err,sysdev.bits.ad_err);

		syncflag.comm_counter++;
		if(syncflag.comm_counter>10)
			syncflag.public_comm_err=1;
		else
			syncflag.public_comm_err=0;

		//检测系统内存状态
		if((delay_i%600==0)&&(delay_i>0))
		{
			check_emmc_size();
			get_memoccupy();
		}

	}

}

/*************************************************
Function:    init_sec_thread
Description: 初始化自检和秒线程
Input:
Output:
Return:成功：0
	　　失败:非0
Others:
*************************************************/
 int init_sec_thread()
 {
 	pthread_t sec_thread_id;
 	int ret;
 	ret=pthread_create(&sec_thread_id,NULL,(void *)sec_thread_entry,NULL);
 	if(ret!=0)
 	{
 		DEBUG ("Create self check thread error!\n");
 		return ret;
 	}
 	return 0;
 }

/*************************************************
Function:    timer_handler
Description: 定时器回调函数，用于需要定时的任务
Input:
Output:
Return:
Others:
*************************************************/
void timer_handler(void)
{

	alarm(1);
}


/*************************************************
Function:    init_timer
Description: 初始化定时器
Input:
Output:
Return:
Others:
*************************************************/
void init_timer()
{
  	signal(SIGALRM, (void*)timer_handler);

  	alarm(1);
}


