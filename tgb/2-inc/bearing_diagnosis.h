#ifndef _BEARING_H_
#define _BEARING_H_
#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>
#include "ad7606.h"


#define BEAR_DIAG_STEP 3
#define DIAG_ACC_STEP 4096
#define FS_ACC_B 3000
#define FFT_POINT 4096

/**
 * 轴承算法配置
 */
struct BEAR_DIAG_CONFIG_PARA
{
	float bear_acc_sensitivity; //轴承灵敏度，默认0.01v/g
	float wheel_diameter;       //车轮直径，默认0.85m，这个值是从列车MVB协议中获取的

	int bearing_roller_num; //轴承滚动体的个数，默认13，需要和甲方确认实际个数
	float para_inner_ring;   	//内环故障频率与车速的比例 默认7.5075
	float para_outer_ring;   	//外环故障频率与车速的比例 默认5.4743
	float para_rolling_body; 	//滚动体故障频率与车速的比例 默认5.3465
	float para_holder;		 	//保持架故障频率与车速的比例 默认0.4225

	//内环故障
	float threshold_value_inner_ring_alarm1; //内环故障1级报警阈值  默认100km/h 0.6067
	float threshold_value_inner_ring_alarm2; //内环故障2级报警阈值  默认100km/h 0.91

	//外环故障
	float threshold_value_outer_ring_alarm1; //外环故障1级报警阈值  默认100km/h 0.6067
	float threshold_value_outer_ring_alarm2; //外环故障2级报警阈值  默认100km/h 0.91

	//滚动体
	float threshold_value_rolling_body_alarm1; //滚动体故障1级报警阈值  默认100km/h 3.7696
	float threshold_value_rolling_body_alarm2; //滚动体故障2级报警阈值  默认100km/h 4.08

	//保持架
	float threshold_value_holder_alarm1; //保持架故障1级报警阈值  默认100km/h 0.4061
	float threshold_value_holder_alarm2; //保持架故障2级报警阈值  默认100km/h 0.5414

	uint32_t alarm_en;                          //使能开关，默认是1，1表示打开使能 0表示关闭使能开关
};


/**
 * 报警类型
 */
enum ALARM_TYPE
{
	RUNNING_OK = 0x00,
	TEMP_WARN = 0x01,
	TEMP_ALARM = 0x02,
	ACC_ALARM_FIRST_GRADE = 0x03,
	ACC_ALARM_SECOND_GRADE = 0x04
};
/**
 *  轴承诊断的结果
 */
struct BEAR_ALARM_VAL
{
	uint8_t alarm_inner_ring;   //内环故障 0：正常   1：一级报警   2：二级报警
	uint8_t alarm_outter_ring;  //外环故障 0：正常   1：一级报警   2：二级报警
	uint8_t alarm_rolling_body; //滚动体故障 0：正常   1：一级报警   2：二级报警
	uint8_t alarm_holder;		//保持架故障 0：正常   1：一级报警   2：二级报警
	uint32_t alarm_bearing;		//轴承故障   0：正常   1：一级报警   其余：二级报警
	enum ALARM_TYPE alarm_type; //轴承故障   1: 正常   4: 一级报警   5：二级报警
};

//轴承相关数据
struct BEARING_DATA
{
//	pthread_mutex_t bearing_lock; //= PTHREAD_MUTEX_INITIALIZER;
//	pthread_cond_t cond_bearing_ready; // = PTHREAD_COND_INITIALIZER;//轴承诊断线程条件变量
	struct AD_PUBLIC_INFO info;
	sem_t bear_sem;  //轴承线程信号量
	uint8_t pthread_connect;
	uint8_t ch_num; //通道编号
	uint8_t pre1_sensor_status;
	uint8_t pre2_sensor_status;
	uint8_t bear_full_flag; //2s数据采满标志

	uint16_t *bear_buf; //一次的轴承原始数据，６Ｋ

};


/**
 * 轴承算法存储的数据
 */
struct BEAR_SAVE
{
	struct AD_PUBLIC_INFO pub_info;
	unsigned char channel_id;								//通道编号0-15
	unsigned char self_test;								//传感器自检结果 1:err 0:ok
	unsigned char temp_alarm_status;						//获取当时的温度预报警状态
	unsigned char speed_valid_flag;
	float diag_speed;							//4096个加速度所对应速度的平均值
	float temp_env;								//当时的环温，只做监测，与算法无关。环温测点是否会受到机械摩擦或者油温的影响
	float temp_running_gear_temp;				//当时的走行部温度，只做监测，与算法无关
	float amplitude_inner_ring;					//内环故障特征数据
	float amplitude_outer_ring;					//外环故障特征数据
	float amplitude_rolling_body;				//滚动体故障特征数据
	float amplitude_holder;						//保持架故障特征数据
	float amplitude_wheel_rotation;				//轮对轴转频幅值
	float rms_envelope;							//包络信号有效值
	struct BEAR_ALARM_VAL bearing_alarm_status; //轴承诊断的预报警结果
};

/**
 * 轴承算法相关特征结构体
 */
struct BEAR_DIAG_VAL
{
	float *acc_buff;
	float *speed_buff;
	int acc_buff_len;
	int speed_buff_len;
	struct BEAR_SAVE bear_save;

};

/*************************************************
 Function:    diagnosis_temp_thread_entry
 Description: 温度诊断线程
 Input:  无
 Output: 无
 Return: 无
 Others:
 *************************************************/
void diagnosis_temp_thread_entry(void*paramter);


/*************************************************
 Function:   　diagnosis_bearing_thread_entry
 Description: 轴承诊断线程
 Input:  无
 Output: 无
 Return: 无
 Others:
 *************************************************/
void diagnosis_bearing_thread_entry();



/*************************************************
 Function:    init_bearing_diagnosis_thread
 Description: 轴承诊断线程初始化
 Input:
 Output:
 Return:成功:0
 失败:非0
 Others:
 *************************************************/
int init_bearing_diagnosis_thread();

#endif
