#ifndef _CAN_CONFIG_H_
#define _CAN_CONFIG_H_

#include "user_data.h"

/***********下面是CAN相关定义**************************/
struct list_node  //引用了rtt下面的链表节点定义
{
    struct list_node *next;
    struct list_node *prev;
};
typedef struct list_node list_t;

typedef enum FRAME_TYPE
{
    SINGLE,
	MULTI_BEGIN,
    MULTI_MID,
    MULTI_END

}Frame_type;

struct FRAME
{
	  uint32_t cnt;
	  uint32_t size;
	  enum BOOL deal_flag;
      uint8_t *rx_buf;
};

struct FRAME_DEAL
{
    list_t	flist;
	uint32_t tag_id;
	uint32_t souc_id;

	struct FRAME *frame_tmp;
    struct FRAME *frame_rd;
    struct FRAME *frame_nor;
    void (*deal)(uint8_t *data, uint32_t len);
};

typedef union CAN_ID
{
  struct{
    uint32_t souc_id  :6;
    uint32_t type     :2;
    uint32_t tag_id   :6;
    uint32_t ack      :2;
    uint32_t cnt      :6;
    uint32_t reserve  :4;
	uint32_t priority :3;
    uint32_t nc       :3;
  }BITS;
	uint32_t WORD;
}CAN_ID_t;

#define rt_list_entry(node, type, member) \
    ((type *)((char *)(node) - (unsigned long)(&((type *)0)->member)))


/*************************************************
Function:  can_send_data
Description: can消息发送函数
Input:  要发送的数据指针:data  数据的长度:len
Output: 无
Return: 无
Others:
*************************************************/
void can_send_data(uint8_t *data, uint16_t len);


/*************************************************
Function:  can_tx_thread_entry
Description: can发送线程入口
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
void can_tx_thread_entry(void *parameter );

/*************************************************
Function:  can_tx_init
Description: 初始化can的发送队列
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
void can_tx_init(void);

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
void can_send(uint8_t taget, uint8_t cmd, uint8_t *data, uint16_t len);


/*************************************************
Function:    config_can
Description: 配置socket can的相关参数
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
void config_can();

/*************************************************
Function:    protocol_deal
Description: can数据协议解析
Input:  接受的can数据指针:data
		数据的长度:data_len
Output: 无
Return: 无
Others:
*************************************************/
void protocol_deal(uint8_t *data, uint8_t data_len);


/*************************************************
Function:    init_can_socket
Description: 配置和初始化can
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
int init_can_socket();


#endif
