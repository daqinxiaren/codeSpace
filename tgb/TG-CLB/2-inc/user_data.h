#ifndef _USER_DATA_H_
#define _USER_DATA_H_
#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>
#include "dev_config.h"


//#define TEST_USING

#define SENSOR_DELAY_DEAL //传感器延时处理，装车时使用
#define TEST_VIBR
#define USING_CAN_DEV


#ifndef __DEBUG__

//#define __DEBUG__
//#define DEBUG(format,...) printf("File: "__FILE__", Line: %05d: "format"/n", __LINE__, ##__VA_ARGS__)
#define DEBUG(format,...)  printf(format,##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif

#define BOARD_NUMB  MAIN_BOARD
#define SOFT_VERSION       0x2000     //
#define SOFT_UPDATE_TIME    20190819     //软件发布时间
#define UDP 1
#define TCP 2
/*
 * 板卡号
 */

enum BOOL
{
	FALSE,
	TRUE
};

////Board ID,用于can通信 spi
enum BOARD_TYPE
{
	  MAIN_BOARD = 0x0f,   // 处理板
	  PRE_BOARD1 = 0x01,   // 接口板1
	  PRE_BOARD2 = 0x02,   //接口板2
	  AD_BOARD =0xa,
};

//自定义时间结构体
struct LOCAL_TIME
{
	uint32_t year;
	uint32_t mon;
	uint32_t day;
	uint32_t hour;
	uint32_t min;
	uint32_t sec;
};

extern struct send_local_st local_st;

void init_sys_err_para(void);
/*************************************************
Function:  check_dir_exits
Description:  检查一个目录是否存在
Input: 　目录绝对路径 dir_path
Output: 无
Return: 存在0，不存在-１
Others:
*************************************************/
int check_dir_exits(const char* dir_path);


/*************************************************
Function:  get_local_time
Description: 获取本机当前时间
Input:  时间结构体指针
Output: 无
Return: 无
Others:
*************************************************/
void get_local_time(struct LOCAL_TIME *timel);

/*************************************************
Function:  init_data
Description: 读取配置参数，创建存储文件
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
void init_data(void);

/*************************************************
Function:  print_system_para
Description: 打印系统配置的相关参数
Input:
Output:
Return:
Others:
*************************************************/
void print_system_para(void);


void cmd_input_check(int argc,char** argv);
/*************************************************
Function:  set_local_time
Description: 设置本机当前时间
Input:  时间结构体指针
Output: 无
Return: 无
Others:
*************************************************/
void set_local_time(struct LOCAL_TIME *timel);

int time_msg_valid_deal(uint8_t *timebuf);

void set_start_time(void);


int init_sec_thread();

int init_file_del_thread();

int init_spi1_thread(void);

int init_derail_thread(void);

#endif

