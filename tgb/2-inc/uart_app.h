#ifndef _UART_APP_H
#define _UART_APP_H

enum UART4_CMD
{
    SAMPLE_ASK = 0x00, //采样请求
    SAMPLE_ACK = 0x01, //２s数据采集完成回复
    RTC_TIME = 0x02,
    READ_BEAR_DATA_FLAG = 0x03,
    READ_POLYGOM_DATA_FLAG = 0x04,
	PTU_LED = 0x05,

	ADB_IAP_MESSAGE = 0x60,
	UART_WAIT_MESSAGE = 0x61,
	UART_RESET_MESSAGE = 0x62,
	SYSTEM_RESET_MESSAGE = 0x63

};



//升级时内部命令标识
enum UART_UPDATE_CMD
{
	UART_START_IAP,//准备升级
	UART_SEND_IAP,//数据发送
	UART_ACK_OK,//正确应答
	UART_ACK_ERR,//错误应答
	UART_END_IAP//完成数据发送
};


//内部ad板卡升级相关标志
struct UART_UPDATE_DATA
{
    uint8_t update_message;  //升级时的命令符标识
    uint8_t recv_normal_data;
    uint8_t recv_error_data;
    uint8_t open_count;
    uint32_t send_group_count; //发送的组数计数

};


/****************************************
Function:  set_speed
Description: 设置串口波特率
Input: 　串口文件：fd
		波特率:speed
Output: 无
Return: 无
Others:
****************************************/
void set_speed(int fd ,int speed);


/****************************************
Function:  set_parity
Description: 设置串口极性
Input: 　串口文件：fd
Output: 无
Return: 无
Others:
****************************************/
int set_parity(int fd);



/****************************************
Function:  serial_init
Description: 串口初始化
Input: 　串口文件路径：dev_path
		波特率：speed
		阻塞标志：is_block
Output: 无
Return: 无
Others:
****************************************/
int serial_init(char *dev_path,int speed,int is_block);



/*************************************************
Function:    uart4_send
Description: 组帧发送uart消息
Input:  目标ID:taget
		命令类型:cmd
		发送数据指针:data
		数据长度:len
Output: 无
Return: 无
Others:
*************************************************/
void uart4_send(uint8_t taget, uint8_t cmd, uint8_t *data, uint16_t len);


/****************************************
Function:  start_ad_sample
Description: 开始ＡＤ采样
Input: 　无
Output: 无
Return: 无
Others:
****************************************/
void start_ad_sample(void);



/****************************************
Function:  start_read_bear_data
Description: 开始填充轴承数据采样buf
Input: 　无
Output: 无
Return: 无
Others:
****************************************/
void start_read_bear_data(void);



/****************************************
Function:  start_read_polygon_data
Description: 开始填充轮对数据采样buf
Input: 　无
Output: 无
Return: 无
Others:
****************************************/
void start_read_polygon_data(void);



/****************************************
Function:  uart4_data_deal
Description: 串口接受数据处理
Input: 　待处理数据：data
		数据长度：len
Output: 无
Return: 无
Others:
****************************************/
void uart4_data_deal(uint8_t *data, uint16_t len);





/****************************************
Function:  send_ptu_connect_message
Description: 网口3连接状态
Input: 　无
Output: 无
Return: 无
Others:
****************************************/
void send_ptu_connect_message(uint8_t connect_status);


/****************************************
Function:  uart4_recv_thread_entry
Description: 串口接收线程函数
Input: 　无
Output: 无
Return: 无
Others:
****************************************/
void uart4_recv_thread_entry(void);



/****************************************
Function:  init_uart4_thread
Description: 串口初始化函数
Input: 　无
Output: 无
Return: 成功:0
		失败:非0
Others:
****************************************/
int init_uart4_thread(void);



/*************************************************
Function:  uart_iap_reset_message
Description: 通知相关板卡跳转到底包
Input:	目标板卡地址
Output:
Return:
Others:
*************************************************/
void uart_send_iap_reset_message(uint8_t taget);

/*************************************************
Function:    uart_send_bin_adb
Description: 发送bin数据给ad采集板
Input:  无
Output: 无
Return: 无
Others:
*************************************************/
void uart_send_bin_adb(uint8_t *bin_data,uint16_t len);

#endif
