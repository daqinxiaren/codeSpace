#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>
#include "ad7606.h"


#define FS_ACC_P 3000
#define POLYGON_DIAG_STEP 3 //3s
#define FS_SPEED_INTERP 300 //1个速度对应10个加速度
#define TZ_POLYGON_VECTOR_MAX_SIZE 600

/**
 * 多边形算法需要的配置
 */
struct POLYGON_DIAG_CONFIG_PARA
{
    float polygon_acc_sensitivity;              //多边形加速度的灵敏度，默认是0.01，硬件可能具有放大倍数
    float wheel_diameter;                       // m    轮径，默认是0.85
    float threshold_speed_max;                  // km/h 多边形算法的速度阈值,默认是80
    float threshold_speed_delta;                // km/h 多边形算法的速度跳变阈值，默认是6
    float threshold_warn1;                      //多边形一级预警阈值    1
    float threshold_warn2;                      //多边形二级预警阈值    20
    float threshold_warn3;                      //多边形三级预警阈值    18
    float threshold_alarm1;                     //多边形一级报警阈值    30
    float threshold_alarm2;                     //多边形二级报警阈值    40
    float threshold_alarm3;                     //多边形三级报警阈值    24
    float calc_db1_para;                        //计算特征值里面的TZ3的DB值，需要这个参数，默认1500
    float calc_db2_para;                        //计算多边形阶次阈值，默认是1.5，暂时不使用
    uint32_t alarm_en;                          //使能开关，默认是1，1表示打开使能 0表示关闭使能开关
};

//轮对多边形相关数据
struct POLYGON_DATA
{
	struct AD_PUBLIC_INFO info;
	sem_t polygon_sem;  //多边形线程信号量
	uint8_t pthread_connect;
	uint8_t ch_num; //通道编号
	uint8_t pre1_sensor_status;
	uint8_t pre2_sensor_status;
	uint8_t polygon_full_flag; //１分数据采满标志
	uint16_t *polygon_buf; //一次的轮对数据，180k
};

/**
 * 多边形算法计算完之后的特征数据
 */
struct POLYGON_SAVE
{
	struct AD_PUBLIC_INFO pub_info;
	unsigned char channel_id;                               //通道编号0-15
	unsigned char self_test;                                //传感器自检结果:1:OK  0:ERR
	unsigned char diag_res;                                 //诊断的结果 RUNNING_OK=0x00,TEMP_WARN=0x01,TEMP_ALARM=0x02,ACC_ALARM_FIRST_GRADE=0x03,ACC_ALARM_SECOND_GRADE=0x04
	unsigned char speed_valid_flag;                    //速度是否有效的标志 1:有效 0:无效
	float speed_max;                              //本段时间内的最大速度 km/h
	float speed_min;                              //本段时间内的最小速度 km/h
	float TZ1_scalar;                             //特征数据1  特征向量的均方根
	float TZ2_scalar;                             //特征数据2  加速度均方根
	float TZ3_scalar;                             //特征数据3  DB值
	int TZ4_vector_length;                        //特征数据4的长度
	//float TZ4_vector[TZ_POLYGON_VECTOR_MAX_SIZE]; //特征数据4  特征向量
};

/**
 * 多边形计算过程
 */
struct POLYGON_CALC
{
    float *acc_buff;
    float *speed_buff;
    int acc_buff_len;
    int speed_buff_len;
    struct POLYGON_SAVE polygon_save;

};


struct CPLX_p
{
	double real;
	double imag;
};


/*************************************************
Function:    init_polygon_thread
Description: 初始化轮对多边形诊断线程
Input:
Output:
Return: 成功：０
		失败：非０
Others:
*************************************************/
int init_polygon_thread(void);



#endif
