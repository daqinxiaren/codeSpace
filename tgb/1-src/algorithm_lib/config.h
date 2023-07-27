/**
 * @file     fft_config.h
 * @details  配置算法参数，根据不同项目配置,郑州地铁10号线项目
 * @author
 * @date
 * @version
**/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* include */
//#define MCU_ARM //若在STM32上跑，找不到此定义时须自行定义
#ifdef MCU_ARM
#include "../../../lib/dsp/include/arm_math.h" //根据代码路径配置
#else
#include "math.h"
#define PI					3.14159265358979f
#endif
//当前配置文件参数因项目而异，不同项目编译脱轨板代码时，清在这里打开对应项目的定义，关闭其他项目
//#define TFDS_ZX         //郑许线
//#define TFDS_Z10        //郑州地铁10号线
//#define TFDS_Z12        //郑州地铁12号线
#define TFDS_Z7         //郑州7号线及西安10号线

/* macro */
/************************* IIR PARAM ****************************/
#define POLYGON_FILTER_NUM_1    0.0202110431311719
#define POLYGON_FILTER_NUM_2    -0.0618784014310059
#define POLYGON_FILTER_NUM_3    0.126361691958874
#define POLYGON_FILTER_NUM_4    -0.168293917341436
#define POLYGON_FILTER_NUM_5    0.191049889368274
#define POLYGON_FILTER_NUM_6    -0.168293917341436
#define POLYGON_FILTER_NUM_7    0.126361691958874
#define POLYGON_FILTER_NUM_8    -0.0618784014310059
#define POLYGON_FILTER_NUM_9    0.0202110431311719

#define POLYGON_FILTER_DEN_1    1
#define POLYGON_FILTER_DEN_2    -5.37851988464675
#define POLYGON_FILTER_DEN_3    13.9721707234815
#define POLYGON_FILTER_DEN_4    -22.3042602479832
#define POLYGON_FILTER_DEN_5    23.7373979438715
#define POLYGON_FILTER_DEN_6    -17.1538692718108
#define POLYGON_FILTER_DEN_7    8.20869031281336
#define POLYGON_FILTER_DEN_8    -2.37814731181059
#define POLYGON_FILTER_DEN_9    0.320944012772122

#define POLYGON_wheel_diameter          0.805		// 单位m	轮径，默认是0.85
#define POLYGON_Sample_rate             4096			//采样频率
#define POLYGON_threshold_prognosis     17
#define POLYGON_threshold_prewarning    22
#define POLYGON_threshold_warning       27
#define POLYGON_time_length             10			//堆栈空间的长度

/*********************** FFT INIT PARAM ****************************/
#define Diag_Num_ZXZC_Wheel     5     //轴箱轴承诊断与踏面诊断总数据长度
#define Diag_Num_DJZC           8     //电机诊断数据长度
#define Diag_Num_gear           1     //齿轮诊断数据长度
#define Diag_Num_CLXZC          8    //齿轮箱轴承诊断数据长度

/*阈值*/
#define threshold_NGB_prognosis    44
#define threshold_W_prognosis      49
#define threshold_Wheel_prognosis  54
#define threshold_gear_prognosis   54

#define threshold_NGB_prewarning   50
#define threshold_W_prewarning     55
#define threshold_Wheel_prewarning 60
#define threshold_gear_prewarning  60

#define threshold_NGB_warning      56
#define threshold_W_warning        61
#define threshold_Wheel_warning    65
#define threshold_gear_warning     65

#ifdef TFDS_Z10
/*轴箱轴承*/
#define ZXZC_low_limit_1           1.24
#define ZXZC_low_limit_2           0.9704
#define ZXZC_low_limit_3           0.83971
#define ZXZC_low_limit_4           0.046211
#define ZXZC_low_limit_5           0.1053
#define ZXZC_high_limit_1          1.3528
#define ZXZC_high_limit_2          1.0587
#define ZXZC_high_limit_3          0.91605
#define ZXZC_high_limit_4          0.050412
#define ZXZC_high_limit_5          0.1148
#define ZXZC_base_1           0.1
#define ZXZC_base_2           0.1
#define ZXZC_base_3           0.1
#define ZXZC_base_4           0.1
#define ZXZC_base_5           0.1
#define ZXZC_bia_1          (10)
#define ZXZC_bia_2          (10)
#define ZXZC_bia_3          (10)
#define ZXZC_bia_4          (10)
#define ZXZC_bia_5          (10)

/*电机轴承*/
#define DJ_low_limit_1           4.3444
#define DJ_low_limit_2           3.0796
#define DJ_low_limit_3           3.8467
#define DJ_low_limit_4           0.27997
#define DJ_low_limit_5           6.9474
#define DJ_low_limit_6           5.201
#define DJ_low_limit_7           4.5978
#define DJ_low_limit_8           0.28894
#define DJ_high_limit_1          4.7393
#define DJ_high_limit_2          3.3596
#define DJ_high_limit_3          4.1964
#define DJ_high_limit_4          0.30542
#define DJ_high_limit_5          7.579
#define DJ_high_limit_6          5.6738
#define DJ_high_limit_7          5.0158
#define DJ_high_limit_8          0.31521
#define DJ_base_1           0.1
#define DJ_base_2           0.1
#define DJ_base_3           0.1
#define DJ_base_4           0.1
#define DJ_base_5           0.1
#define DJ_base_6           0.1
#define DJ_base_7           0.1
#define DJ_base_8           0.1
#define DJ_bia_1          (10)
#define DJ_bia_2          (10)
#define DJ_bia_3          (10)
#define DJ_bia_4          (10)
#define DJ_bia_5          (10)
#define DJ_bia_6          (10)
#define DJ_bia_7          (10)
#define DJ_bia_8          (10)

/*齿轮*/
#define gear_low_limit   9.8946
#define gear_high_limit  10.7941

#define gear_base  0.1
#define gear_bia   (10)

/*齿轮箱轴承*/
#define CLX_low_limit_1           6.9474
#define CLX_low_limit_2           5.201
#define CLX_low_limit_3           4.5978
#define CLX_low_limit_4           0.28894
#define CLX_low_limit_5           6.1652
#define CLX_low_limit_6           4.6333
#define CLX_low_limit_7           3.8187
#define CLX_low_limit_8           0.28958
#define CLX_high_limit_1          7.579
#define CLX_high_limit_2          5.6738
#define CLX_high_limit_3          5.0158
#define CLX_high_limit_4          0.31521
#define CLX_high_limit_5          6.7257
#define CLX_high_limit_6          5.0545
#define CLX_high_limit_7          4.1658
#define CLX_high_limit_8          0.31591
#define CLX_base_1           0.1
#define CLX_base_2           0.1
#define CLX_base_3           0.1
#define CLX_base_4           0.1
#define CLX_base_5           0.1
#define CLX_base_6           0.1
#define CLX_base_7           0.1
#define CLX_base_8           0.1
#define CLX_bia_1          (10)
#define CLX_bia_2          (10)
#define CLX_bia_3          (10)
#define CLX_bia_4          (10)
#define CLX_bia_5          (10)
#define CLX_bia_6          (10)
#define CLX_bia_7          (10)
#define CLX_bia_8          (10)
#endif

#ifdef TFDS_ZX
/*轴箱轴承*/
#define ZXZC_low_limit_1           0.9728
#define ZXZC_low_limit_2           0.71138
#define ZXZC_low_limit_3           0.66179
#define ZXZC_low_limit_4           0.044461
#define ZXZC_low_limit_5           0.1053
#define ZXZC_high_limit_1          1.0612
#define ZXZC_high_limit_2          0.77605
#define ZXZC_high_limit_3          0.72195
#define ZXZC_high_limit_4          0.048503
#define ZXZC_high_limit_5          0.1148
#define ZXZC_base_1           0.1
#define ZXZC_base_2           0.1
#define ZXZC_base_3           0.1
#define ZXZC_base_4           0.1
#define ZXZC_base_5           0.1
#define ZXZC_bia_1          (10)
#define ZXZC_bia_2          (10)
#define ZXZC_bia_3          (10)
#define ZXZC_bia_4          (10)
#define ZXZC_bia_5          (10)

/*电机轴承*/
#define DJ_low_limit_1           3.3532
#define DJ_low_limit_2           2.3752
#define DJ_low_limit_3           2.9613
#define DJ_low_limit_4           0.21593
#define DJ_low_limit_5           5.3686
#define DJ_low_limit_6           4.0052
#define DJ_low_limit_7           3.5045
#define DJ_low_limit_8           0.22251
#define DJ_high_limit_1          3.6581
#define DJ_high_limit_2          2.5911
#define DJ_high_limit_3          3.2305
#define DJ_high_limit_4          0.23556
#define DJ_high_limit_5          5.8567
#define DJ_high_limit_6          4.3693
#define DJ_high_limit_7          3.8231
#define DJ_high_limit_8          0.24274
#define DJ_base_1           0.1
#define DJ_base_2           0.1
#define DJ_base_3           0.1
#define DJ_base_4           0.1
#define DJ_base_5           0.1
#define DJ_base_6           0.1
#define DJ_base_7           0.1
#define DJ_base_8           0.1
#define DJ_bia_1          (10)
#define DJ_bia_2          (10)
#define DJ_bia_3          (10)
#define DJ_bia_4          (10)
#define DJ_bia_5          (10)
#define DJ_bia_6          (10)
#define DJ_bia_7          (10)
#define DJ_bia_8          (10)

/*齿轮*/
#define gear_low_limit   9.8946
#define gear_high_limit  10.7941

#define gear_base  0.1
#define gear_bia   (10)

/*齿轮箱轴承*/
#define CLX_low_limit_1           5.3607
#define CLX_low_limit_2           4.0131
#define CLX_low_limit_3           3.5477
#define CLX_low_limit_4           0.22295
#define CLX_low_limit_5           4.7571
#define CLX_low_limit_6           3.5751
#define CLX_low_limit_7           2.9465
#define CLX_low_limit_8           0.22344
#define CLX_high_limit_1          5.848
#define CLX_high_limit_2          4.378
#define CLX_high_limit_3          3.8702
#define CLX_high_limit_4          0.24322
#define CLX_high_limit_5          5.1896
#define CLX_high_limit_6          3.9001
#define CLX_high_limit_7          3.2144
#define CLX_high_limit_8          0.24376
#define CLX_base_1           0.1
#define CLX_base_2           0.1
#define CLX_base_3           0.1
#define CLX_base_4           0.1
#define CLX_base_5           0.1
#define CLX_base_6           0.1
#define CLX_base_7           0.1
#define CLX_base_8           0.1
#define CLX_bia_1          (10)
#define CLX_bia_2          (10)
#define CLX_bia_3          (10)
#define CLX_bia_4          (10)
#define CLX_bia_5          (10)
#define CLX_bia_6          (10)
#define CLX_bia_7          (10)
#define CLX_bia_8          (10)
#endif

#ifdef TFDS_Z12
/*轴箱轴承*/
#define ZXZC_low_limit_1           0.97783
#define ZXZC_low_limit_2           0.70635
#define ZXZC_low_limit_3           0.63604
#define ZXZC_low_limit_4           0.044147
#define ZXZC_low_limit_5           0.1053
#define ZXZC_high_limit_1          1.0667
#define ZXZC_high_limit_2          0.77056
#define ZXZC_high_limit_3          0.69386
#define ZXZC_high_limit_4          0.04816
#define ZXZC_high_limit_5          0.1148
#define ZXZC_base_1           0.1
#define ZXZC_base_2           0.1
#define ZXZC_base_3           0.1
#define ZXZC_base_4           0.1
#define ZXZC_base_5           0.1
#define ZXZC_bia_1          (10)
#define ZXZC_bia_2          (10)
#define ZXZC_bia_3          (10)
#define ZXZC_bia_4          (10)
#define ZXZC_bia_5          (10)

/*电机轴承*/
#define DJ_low_limit_1           4.2647  //1-4为电机非驱动端轴承参数
#define DJ_low_limit_2           3.0231
#define DJ_low_limit_3           3.7761
#define DJ_low_limit_4           0.27483
#define DJ_low_limit_5           6.8199  //5-8为电机驱动端轴承参数
#define DJ_low_limit_6           5.1056
#define DJ_low_limit_7           4.5135
#define DJ_low_limit_8           0.28364
#define DJ_high_limit_1          4.6524
#define DJ_high_limit_2          3.2979
#define DJ_high_limit_3          4.1194
#define DJ_high_limit_4          0.29981
#define DJ_high_limit_5          7.4399
#define DJ_high_limit_6          5.5697
#define DJ_high_limit_7          4.9238
#define DJ_high_limit_8          0.30943
#define DJ_base_1           0.1
#define DJ_base_2           0.1
#define DJ_base_3           0.1
#define DJ_base_4           0.1
#define DJ_base_5           0.1
#define DJ_base_6           0.1
#define DJ_base_7           0.1
#define DJ_base_8           0.1
#define DJ_bia_1          (10)
#define DJ_bia_2          (10)
#define DJ_bia_3          (10)
#define DJ_bia_4          (10)
#define DJ_bia_5          (10)
#define DJ_bia_6          (10)
#define DJ_bia_7          (10)
#define DJ_bia_8          (10)

/*齿轮*/
#define gear_low_limit   11.2671
#define gear_high_limit  12.2836

#define gear_base  0.1
#define gear_bia   (10)

/*齿轮箱轴承*/
#define CLX_low_limit_1           6.8199  //1-4为输入端电机侧轴承参数
#define CLX_low_limit_2           5.1056
#define CLX_low_limit_3           4.5135
#define CLX_low_limit_4           0.28364
#define CLX_low_limit_5           6.0208  //5-8为输入端车轮侧轴承参数
#define CLX_low_limit_6           4.5796
#define CLX_low_limit_7           3.9181
#define CLX_low_limit_8           0.28623
#define CLX_high_limit_1          7.4399
#define CLX_high_limit_2          5.5697
#define CLX_high_limit_3          4.9238
#define CLX_high_limit_4          0.30943
#define CLX_high_limit_5          6.5681
#define CLX_high_limit_6          4.996
#define CLX_high_limit_7          4.2743
#define CLX_high_limit_8          0.31225
#define CLX_base_1           0.1
#define CLX_base_2           0.1
#define CLX_base_3           0.1
#define CLX_base_4           0.1
#define CLX_base_5           0.1
#define CLX_base_6           0.1
#define CLX_base_7           0.1
#define CLX_base_8           0.1
#define CLX_bia_1          (10)
#define CLX_bia_2          (10)
#define CLX_bia_3          (10)
#define CLX_bia_4          (10)
#define CLX_bia_5          (10)
#define CLX_bia_6          (10)
#define CLX_bia_7          (10)
#define CLX_bia_8          (10)
#endif

#ifdef TFDS_Z7
/*轴箱轴承*/
#define ZXZC_low_limit_1           1.24
#define ZXZC_low_limit_2           0.9704
#define ZXZC_low_limit_3           0.83971
#define ZXZC_low_limit_4           0.046211
#define ZXZC_low_limit_5           0.1053
#define ZXZC_high_limit_1          1.3528
#define ZXZC_high_limit_2          1.0587
#define ZXZC_high_limit_3          0.91605
#define ZXZC_high_limit_4          0.050412
#define ZXZC_high_limit_5          0.1148
#define ZXZC_base_1           0.1
#define ZXZC_base_2           0.1
#define ZXZC_base_3           0.1
#define ZXZC_base_4           0.1
#define ZXZC_base_5           0.1
#define ZXZC_bia_1          (10)
#define ZXZC_bia_2          (10)
#define ZXZC_bia_3          (10)
#define ZXZC_bia_4          (10)
#define ZXZC_bia_5          (10)

/*电机轴承*/
#define DJ_low_limit_1           4.3444  //1-4为电机非驱动端轴承参数
#define DJ_low_limit_2           3.0796
#define DJ_low_limit_3           3.8467
#define DJ_low_limit_4           0.27997
#define DJ_low_limit_5           6.9474  //5-8为电机驱动端轴承参数
#define DJ_low_limit_6           5.201
#define DJ_low_limit_7           4.5978
#define DJ_low_limit_8           0.28894
#define DJ_high_limit_1          4.7393
#define DJ_high_limit_2          3.3596
#define DJ_high_limit_3          4.1964
#define DJ_high_limit_4          0.30542
#define DJ_high_limit_5          7.579
#define DJ_high_limit_6          5.6738
#define DJ_high_limit_7          5.0158
#define DJ_high_limit_8          0.31521
#define DJ_base_1           0.1
#define DJ_base_2           0.1
#define DJ_base_3           0.1
#define DJ_base_4           0.1
#define DJ_base_5           0.1
#define DJ_base_6           0.1
#define DJ_base_7           0.1
#define DJ_base_8           0.1
#define DJ_bia_1          (10)
#define DJ_bia_2          (10)
#define DJ_bia_3          (10)
#define DJ_bia_4          (10)
#define DJ_bia_5          (10)
#define DJ_bia_6          (10)
#define DJ_bia_7          (10)
#define DJ_bia_8          (10)

/*齿轮*/
#define gear_low_limit   11.4777
#define gear_high_limit  12.5132

#define gear_base  0.1
#define gear_bia   (10)

/*齿轮箱轴承*/ 
#define CLX_low_limit_1           6.9474  //1-4为电机侧轴承参数
#define CLX_low_limit_2           5.201
#define CLX_low_limit_3           4.5978
#define CLX_low_limit_4           0.28894
#define CLX_low_limit_5           6.1652  //5-8为车轮侧轴承参数
#define CLX_low_limit_6           4.6333
#define CLX_low_limit_7           3.8187
#define CLX_low_limit_8           0.28958
#define CLX_high_limit_1          7.579
#define CLX_high_limit_2          5.6738
#define CLX_high_limit_3          5.0158
#define CLX_high_limit_4          0.31521
#define CLX_high_limit_5          6.7257
#define CLX_high_limit_6          5.0545
#define CLX_high_limit_7          4.1658
#define CLX_high_limit_8          0.3159
#define CLX_base_1           0.1
#define CLX_base_2           0.1
#define CLX_base_3           0.1
#define CLX_base_4           0.1
#define CLX_base_5           0.1
#define CLX_base_6           0.1
#define CLX_base_7           0.1
#define CLX_base_8           0.1
#define CLX_bia_1          (10)
#define CLX_bia_2          (10)
#define CLX_bia_3          (10)
#define CLX_bia_4          (10)
#define CLX_bia_5          (10)
#define CLX_bia_6          (10)
#define CLX_bia_7          (10)
#define CLX_bia_8          (10)
#endif
/* type declaration */

#endif /*__CONFIG_H__*/

