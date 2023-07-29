#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include "fftw3.h"
#include "user_data.h"
#include "spi_app.h"
#include "ploygon_diagnosis.h"
#include "hdd_save.h"

#define LOCAL_CONFIG_FILE   "/media/LH_DATA/local_config/config/train.dat" //本地配置文件，可从SD卡或者U盘里面导入来更新
#define LOCAL_PISTZ_FILE   "/media/LH_DATA/hdd/PIS/" 					  //本地pis特征值文件，下面目录依次放置每列车的特征值（轴承、轮对、温度）
#define LOCAL_TRAIN_FILE   "/media/LH_DATA/hdd/"						  //本地列车数据文件，下面目录依次放置每列车的轴承、轮对、温度数据文件夹，文件夹下又分原始和特征值



/*************************************************
Function:  get_para_from_local
Description:  从本地emmc获取配置参数
Input: 　无
Output: 无
Return: 无
Others:
*************************************************/
void get_para_from_local()
{

}


/*************************************************
Function:  get_local_time
Description: 获取本机当前时间
Input:  时间结构体指针
Output: 无
Return: 无
Others:
*************************************************/
void get_local_time(struct LOCAL_TIME *timel)
{
	time_t timep;
	struct tm *ptime;
	time(&timep);
	ptime=gmtime(&timep);
	timel->year = 1900+ptime->tm_year;
	timel->mon  = 1+ptime->tm_mon;
	timel->day  = ptime->tm_mday;
	timel->hour = ptime->tm_hour;
	timel->min  = ptime->tm_min;
	timel->sec  = ptime->tm_sec;
	//printf("year:%d-%d-%d-%d-%d-%d\n",timel->year,timel->mon,timel->day,timel->hour ,timel->min ,timel->sec);
}


/*************************************************
Function:  set_local_time
Description: 设置本机当前时间
Input:  时间结构体指针
Output: 无
Return: 无
Others:
*************************************************/
void set_local_time(struct LOCAL_TIME *timel)
{
	char date_str[50],time_str[50];
	char str[100] = {'d','a','t','e',' ','-','s'};
	char str1[100] = {'d','a','t','e',' ','-','s'};

	system("date");
	sprintf(date_str,"%d-%d-%d",timel->year,timel->mon,timel->day);
	//printf("date_str = %s \r\n",date_str);
	strcat(str,date_str);
	system(str);

	sprintf(time_str,"%d:%d:%d",timel->hour,timel->min,timel->sec);
	strcat(str1,time_str);
	system(str1);

	//system("date");
}

/*************************************************
Function:  set_start_time
Description: 设置本机当前时间
Input:  时间结构体指针
Output: 无
Return: 无
Others:
*************************************************/
void set_start_time(void)
{
	struct LOCAL_TIME timel;
	char date_str[50],time_str[50];
	char str[100] = {'d','a','t','e',' ','-','s'};
	char str1[100] = {'d','a','t','e',' ','-','s'};

	timel.year=2020;
	timel.mon=1;
	timel.day=1;
	timel.hour=0;
	timel.min=0;
	timel.sec=0;

	//system("date");
	sprintf(date_str,"%d-%d-%d",timel.year,timel.mon,timel.day);
	//printf("date_str = %s \r\n",date_str);
	strcat(str,date_str);
	system(str);

	sprintf(time_str,"%d:%d:%d",timel.hour,timel.min,timel.sec);
	strcat(str1,time_str);
	system(str1);
	//system("date");
}

/*************************************************
Function:  time_msg_valid_deal
Description: 设置本机当前时间
Input:  时间结构体指针
Output: 无
Return: 无
Others:
*************************************************/
int time_msg_valid_deal(uint8_t *timebuf)
{
    static uint8_t time_cnt=0;
    static uint8_t set_time_flag=1;
    struct LOCAL_TIME time_t,loctime;
    int ret,return_t=0;

   if(set_time_flag)
    {
    	time_t.year = 2000+timebuf[0];
    	time_t.mon = timebuf[1];
    	time_t.day = timebuf[2];
    	time_t.hour= timebuf[3];
    	time_t.min = timebuf[4];
    	time_t.sec = timebuf[5];
		if (time_t.year>2099 || time_t.mon>12 || time_t.day>31 || time_t.hour>=60 || time_t.min>=60 || time_t.sec>=60)
		{
			printf("data&time error!!!: %04d-%02d-%02d %d:%d:%d\n",time_t.year,time_t.mon,time_t.day,time_t.hour,time_t.min,time_t.sec);
			return -1;
		}
		set_local_time(&time_t);
		printf("$$$$sync mvb time.....\n");
		set_time_flag=0;
        return_t=1;
    }
    else if(time_cnt==61)
    {
		get_local_time(&loctime);
		time_t.year=2000+timebuf[0];
		time_t.mon=timebuf[1];
		time_t.day=timebuf[2];
		time_t.hour=timebuf[3];
		time_t.min=timebuf[4];
		time_t.sec=timebuf[5];

		ret=memcmp(&time_t,&loctime,16);

		if(ret!=0)
		{
			if (time_t.year>2099 || time_t.mon>12 || time_t.day>31 || time_t.hour>=60 || time_t.min>=60 || time_t.sec>=60)
			{
				printf("data&time error!!!: %04d-%02d-%02d %d:%d:%d\n",time_t.year,time_t.mon,time_t.day,time_t.hour,time_t.min,time_t.sec);
				return -1;
			}
			printf("$$$$time reset....\n");
			set_local_time(&time_t);
		}
		time_cnt=0;
    }

    time_cnt++;
    return return_t;
}

/*************************************************
Function:  update_save_msg
Description:
Input:
Output: 无
Return: 无
Others:
*************************************************/
void update_save_msg(struct AD_PUBLIC_INFO *msg,uint8_t type)
{
	struct LOCAL_TIME time_t;
	uint16_t save_len=SAMPLE_FS*2+64;

	memcpy(msg,&public_msg,sizeof( struct AD_PUBLIC_INFO ));

	get_local_time(&time_t);
	msg->year=time_t.year-2000;
	msg->mon=time_t.mon;
	msg->day=time_t.day;
	msg->hour=time_t.hour;
	msg->min=time_t.min;
	msg->sec=time_t.sec;

	msg->type=type;
	msg->lenth[0]=(uint8_t)save_len;
	msg->lenth[1]=(uint8_t)(save_len>>8);

}

/*************************************************
Function:  init_data
Description: 读取配置参数，创建存储文件
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
void init_data(void)
{

	memset(&local_st,0,sizeof(struct send_local_st ));
	init_version(&local_st.derail_version);
	printf("\n[TFDSN-TCLB-VERSION]:\n");

	printf("\t->soft_version:v%d.%d\n",local_st.derail_version.soft_version[0],local_st.derail_version.soft_version[1]);
	printf("\t->hard_version:%c%c\n",local_st.derail_version.hard_version[0],local_st.derail_version.hard_version[1]);
	//printf("\t->old_version_time:%s\n",ver_para->record_version.old_version_time);
	printf("\t->cur_version_time:%d\n",TCLB_UPDATE_TIME);
	printf("\n");

	init_sys_err_para();		//初始化系统故障值


}


/*************************************************
Function:  init_sys_err_para
Description: 初始化系统默认故障参数
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
void init_sys_err_para(void)
{
	train_data.train_dat.raw.raw_fd=-1,
	train_data.train_dat.tz.tz_fd=-1,
	syncflag.comm_counter=0;
	syncflag.time_update_flag=0;
	syncflag.public_comm_err=0;
	system("ulimit -n 1048576");

}

/*************************************************
Function:  init_version
Description: version
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
void init_version(struct version_t *version)
{
	uint16_t version_soft=TCLB_SOFT_VERSION;
	uint32_t version_time= TCLB_UPDATE_TIME;
	memset(version,0,sizeof(struct version_t));

	version->soft_version[0]=(uint8_t)(version_soft>>8);
	version->soft_version[1]=(uint8_t)(version_soft);

	version->hard_version[0]=TCLB_HARDWARE_VERSION_H;
	version->hard_version[1]=TCLB_HARDWARE_VERSION_L;

	version->date[0]=(uint8_t)(version_time>>24);
	version->date[1]=(uint8_t)(version_time>>16);
	version->date[2]=(uint8_t)(version_time>>8);
	version->date[3]=(uint8_t)(version_time);

}
