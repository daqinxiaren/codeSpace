/*
 * public_func.h
 *
 *  Created on: 2021年2月22日
 *      Author: root
 */

#ifndef INC_PUBLIC_FUNC_H_
#define INC_PUBLIC_FUNC_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

typedef unsigned char uint8_t;

#define LOCAL_PARA_PATH_NAME "local_para.dat"

#define LOCAL_NET_ID 0XFF

struct threshold_para
{
	int amplitude_alarm_num;
	float amplitude_alarm_thteshold;
	int abs_alarm_num;
	float abs_alarm_thteshold;

	float uplift_alarm_thteshold;
};

struct save_para
{
	int fs_time;
	int risk_before_time;
	int risk_after_time;
};

struct config_para
{
	struct threshold_para threshold;
	struct save_para save;
};


//文件数据存储
extern int save_date_to_file(char* path,void* buf,int len);

//文件数据读取
extern int get_date_to_file(char* path,void* buf,int len);

//获取配在参数
extern int get_local_para(struct config_para* para );

//设置参数
extern int set_local_para(struct config_para* para );

//获取本地时间创建时间文件
extern void create_ymd_hms_dir_name(char *name);

//网络发送
extern void net_send(int sock_fd,struct sockaddr_in *send_addr, uint8_t cmd, uint8_t *data, int len);

#endif /* INC_PUBLIC_FUNC_H_ */
