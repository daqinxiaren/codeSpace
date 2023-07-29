#ifndef _PIS_H_
#define _PIS_H_

#include <stdint.h>
#include <semaphore.h>
#include <sys/socket.h>

//PIS网络参数
struct PIS_NET_ADDR
{
	int server_net_port;
	uint8_t server_ip[4];
	uint8_t self_ip[4];
	uint8_t self_gwaddr[4];
	uint8_t self_maskaddr[4];
};

//pis数据结构体
struct PIS_DAT_ST
{
	sem_t pis_sem;
	uint8_t pis_allow_flag;
	uint8_t pis_connect_flag;
	int pis_commit_socket;
};
enum PIS_MSG_TYPE
{
    RESET_FTP_TYPE,							//重启ftp命令
    DATA_VALID_TYPE							//数据有效命令
};
enum PIS_BOARD_TYPE
{
    PIS,
    TRAIN_MCU,								//车号
};


void pis_thread_entry(void*para);
int init_pis_thread(void);
void pis_client_init(void);
void pis_sem_init(void);
void pis_data_deal_func(uint8_t *data,uint16_t len);


#endif
