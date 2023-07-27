/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the derail_diag.h API or library.
 *
 * @features			:
 *
 * @author       		: xuzhao <xuzhao@lhrtt.com>
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: Jan 25, 2021
 *
 * @environment  		: null
 *
 * @description  		: null 
 *
 * @module       		: null
 *
 * \n <b> @NOTE </b> 	: null
 *
 * \b @Warning      		: null
 * 
 * *
 * Change Logs:
 * Date           author       notes
 * Jan 25, 2021		  kernel       create this file
 * 
 * 
 */
 
/**
 * \addtogroup dev module
 * @{
 */


/**
 * \defgroup module API
 *
 * The xxx API module defines a set of functions that a xxx device
 * driver must implement.
 *
 * @{
 */
 
#ifndef _DERAIL_DIAG_H_
#define _DERAIL_DIAG_H_

/**
 ***************** include file ***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
#include <stdint.h>


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/** @formatter:off  */
/**
 ****************<Public macro>****************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
/**
 * macro define	- description 
 * @xxx:	description.
 * @xxx:	description.
 * @xxx:	description.
 */
#define DERAIL_DIAG_ENABLE
#define MAX_LENGHT_DATA 		600000
#define DERAIL_FILTER_ORDER 	4							/*滤波阶数*/
#define DERAIL_N				60							/*脱轨算法分段数*/
#define DERAIL_DATA_NUM 		50							/*每一个分段数据个数*/


/** @formatter:on */

/**
 ***************<Public typedef>***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
struct derail_diag_threshold
{
	float max_acc_threshold;
	float abs_acc_threshold;
	uint16_t max_acc_cnt_threshold;
	uint16_t abs_acc_cnt_threshold;
};


struct derail_filter_struct_double
{
	uint16_t  derail_filter_order;
	uint16_t  derail_window_size;
};


struct derail_diag_calculate_res
{
	/*加速度峰值分段*/
	float max_acc_value[DERAIL_DATA_NUM][DERAIL_N];
	/*加速度绝对值的和分段*/
	float abs_acc_value[DERAIL_DATA_NUM];

	float wheel_uplift[DERAIL_DATA_NUM];
};

struct derail_data_save_head
{
    uint8_t head[2];               /*"0xAA50：过程数据包0xAA51-0xAA54：原始数据包（根据实际增减）*/
    uint8_t len[2];                /*振动过程数据为360，失稳数据为256 平稳为256 原始为1024 轴温为256*/
    uint8_t company_id;            /*板卡供应商编号 参考供应商定义*/
    uint8_t board_id;              /*本身板卡编号 参考宏定义*/
    uint8_t life_signal[2];        /*生命信号，每秒加1*/
};



struct public_info
{
    uint8_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t marshalling[2];									/*编组编号*/
    uint8_t speed[2];										/*全车速度 1 = 0.01km/h*/
    uint8_t train_outer_temp;								/*车外温度*/
    uint8_t ctrl_train_mode;								/*控车模式*/


    struct gps
    {
    	uint8_t byte;										/*轮径值判断，空气压力值*/
        uint8_t air_spring_pressure1;
        uint8_t air_spring_pressure2;
        uint8_t longitude_down;
        uint8_t longitude_mid_down;
        uint8_t longitude_mid_up;
        uint8_t longitude_up;
        uint8_t longitude_dir;
        uint8_t latitude_dir;
        uint8_t latitude_down;
        uint8_t latitude_mid_down;
        uint8_t latitude_mid_up;
        uint8_t latitude_up;

    } gps_data;

    uint8_t wheel_dia1[2];					/*１架轮径值*/
    uint8_t wheel_dia2[2];					/*2架轮径值*/
    uint8_t train_style;					/*车型*/
    uint8_t carriage_number;				/*车厢号*/
    uint8_t motor_or_trailer_flag;			/*动拖车标识*/
    uint8_t byte;							/*速度值有效判断*/
};



struct save_date_inf
{
	struct derail_data_save_head save_date_head;
	struct public_info train_public_info;
	//数据类型和数据通道号

	uint8_t date_type_channel;      /*数据类型和通道号,数据类型：低4位;通道号高4位*/
	float factor_date;				/*加速度转换系数*/

	uint8_t res[2];
	uint8_t acc_sum[2];
};

/*数据类型,类型范围不能超过15*/
enum save_date_type
{
	DERAIL_Z_DATE = 0x01,				/*垂向加速度数据*/
	DERAIL_Y_DATE = 0x02,				/*横向加速度数据*/
	DERAIL_Z_MAX_DATE = 0x03,				/*垂向脱轨加速度峰值*/
	DERAIL_Z_ABS_DATE = 0x04,				/*垂向脱轨加速度数据绝对值*/
	DERAIL_Z_UPLIFT_DATE = 0x05,		 /*垂向脱轨加速度数据太升量计算*/
	DERAIL_Z_MVOE_DATE = 0x05,		/*横向脱轨加速度数据横移量计算*/
};


/*自定义时间结构体*/
struct local_time
{
	uint32_t year;
	uint32_t mon;
	uint32_t day;
	uint32_t hour;
	uint32_t min;
	uint32_t sec;
};

/**
 **************<Public variables>**************
 * @description		: null 
 * @note			: null 
 * @note			: null  
 */

/**
 **************<Public functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
/*从硬盘中读取脱轨原始数据*/
extern int dds_data_read(char *read_path,float *read_buff);

/*获取平均值*/
extern float get_average_value(float *acc_data,uint16_t data_len);

/*低通滤波*/
extern void derail_filter_deal(float *deal_buf,uint16_t data_len);

/*判断最大加速度值*/
extern void judge_max_acc_data(float *acc_buf,uint8_t **judge_res,uint16_t acc_data_len);

/*存储加速度峰值*/
extern int save_max_acc_data(char *save_path,float *acc_buf,uint16_t acc_data_len,uint8_t channl);

/*加速度值绝对值*/
extern void judge_abs_acc_data(float *acc_buf,uint8_t *judge_res,uint16_t acc_data_len);

/*存储加速度绝对值*/
extern int save_abs_acc_data(char *save_path,float *acc_buf,uint16_t acc_data_len,uint8_t channl);

/*计算车轮抬升量*/
extern int dds_wheel_uplift(float* buff,float* uplift,int fs,uint16_t count);

/*存储车轮抬升量*/
extern int save_uplift_acc_data(char *save_path,float *acc_buf,uint16_t acc_data_len,uint8_t channl);

/*计算车轮抬升量*/
extern int dds_wheel_move(float* buff,float* move,int fs,uint16_t count);

/*存储车轮抬升量*/
extern int save_move_acc_data(char *save_path,float *acc_buf,uint16_t acc_data_len,uint8_t channl);

/**
 **************<Private functions>**************
 * @description		: null
 * @note			: null
 * @note			: null
 */



#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/


/**
 **************<Public functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/**
 * @brief development description of class
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: Jan 25, 2021
 * @create time  : Jan 25, 2021
 * *
 * Parameters
 * *
 * @param	param	new node to be inserted
 * 
 * @param [ in or out ]	param	new node to be inserted
 *
 * * 
 * @return 
 *			- xxx   description or none
 *			- xxx   description ,the operation status or on successful
 * @retval
 *			- xxx   description
 * *
 * @see  reference @file
 *
 * @note 			description
 *							
 * @warning			description 
 *	
 * Change Logs:
 * Date           	  author       notes
 * Jan 25, 2021		  xuzhao       create this class
 *						
 */




/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* DDS_LIB_INC_DERAIL_DIAG_H_ */

