//
//#include <math.h>
//#include <stdio.h>
//#include <unistd.h>
//#include <string.h>
//#include <stdlib.h>
//#include <pthread.h>
//#include <stdint.h>
//#include <getopt.h>
//#include <fcntl.h>
//#include <sys/ioctl.h>
//#include <linux/types.h>
//#include <signal.h>
//#include <sys/time.h>
//#include <stdint.h>
//#include <sys/socket.h>
//#include <net/if.h>
//#include <semaphore.h>
//
//#include "user_data.h"
//#include "fftw3.h"
//#include "lh_math.h"
//#include "pthread_policy.h"
//#include "watchdog.h"
//#include "temp_diagnosis.h"
//#include "bearing_diagnosis.h"
//#include "hdd_save.h"
//
////#define BEARING_DEBUG
//#ifdef BEARING_DEBUG
//#define BEARING_PRINT printf
//#else
//#define BEARING_PRINT(...)
//#endif
//
//
//extern struct _TRAIN_ALARM_ST train_alarm_stat; //列车报警状态
//extern union _SEND_ALARM_STATUS mvb_send_alarm_status;
//extern struct MASTER_CLB_CONFIG_PARA *master_config_paras;
//
//
//extern struct BEARING_DATA *bearing_data;
//extern struct spi_flag_st spi_flag;
//
///**
// * 初始化轴承算法特征值
// */
//static void init_bearing_diag_val(struct BEAR_DIAG_VAL *bear_diag_val)
//{
//	memset(bear_diag_val, 0, sizeof(struct BEAR_DIAG_VAL));
//	if (!bear_diag_val->acc_buff)
//	{
//		bear_diag_val->acc_buff = (float *)malloc(
//			DIAG_ACC_STEP * sizeof(float));
//		if (bear_diag_val->acc_buff)
//		{
//			memset(bear_diag_val->acc_buff, 0,
//				   DIAG_ACC_STEP * sizeof(float));
//			BEARING_PRINT("bear_diag_val->acc_buff ok\n");
//		}
//		else
//		{
//			BEARING_PRINT("bear_diag_val->acc_buff err\n");
//		}
//	}
//
//	if (!bear_diag_val->speed_buff)
//	{
//		bear_diag_val->speed_buff = (float *)malloc(
//			FS_SPEED * 2 * sizeof(float));
//		if (bear_diag_val->speed_buff)
//		{
//			memset(bear_diag_val->speed_buff, 0,
//				   FS_SPEED * 2 * sizeof(float));
//			BEARING_PRINT("bear_diag_val->speed_buff ok\n");
//		}
//		else
//		{
//			BEARING_PRINT("bear_diag_val->speed_buff err\n");
//		}
//	}
//
//	bear_diag_val->acc_buff_len = DIAG_ACC_STEP;
//}
//
///**
// * 释放轴承算法缓存
// */
// static void destroy_bearing_diag_val(struct BEAR_DIAG_VAL *bear_diag_val)
//{
//	if (bear_diag_val->acc_buff)
//	{
//		free(bear_diag_val->acc_buff);
//		bear_diag_val->acc_buff = NULL;
//	}
//
//	if (bear_diag_val->speed_buff)
//	{
//		free(bear_diag_val->speed_buff);
//		bear_diag_val->speed_buff = NULL;
//	}
//
//	memset(bear_diag_val, 0, sizeof(struct BEAR_DIAG_VAL));
//}
//
//
// /**
//  * 轴承AD转加速度公式
//  */
//static float convert_ad_bearing(unsigned short ad)
//{
// 	//return (ad - 32768) / 65536.0 * 20 / master_config_paras->bear_diag_para.bear_acc_sensitivity/0.416;
//}
//
//
// /**
//  * 更新轴承MVB算法结果*/
//static void update_mvb_send_bearing_alarm_status(uint8_t ch, int diag_res)
//{
//
//}
//
//
//
// /**
//  * fft的C语言实现
//  */
//
//static void fft(float *buff, int size)
//{
// #ifdef MCU_ARM
// 	static float32_t fft_temp[4096 * 2];
// 	uint16_t i;
// 	arm_cfft_radix4_instance_f32 scfft;
// 	memset(fft_temp, 0, sizeof(fft_temp));
// 	for (i = 0; i < size; i++)
// 	{
// 		fft_temp[2 * i] = buff[i];
// 	}
// 	arm_cfft_radix4_init_f32(&scfft, size, 0, 1);
// 	arm_cfft_radix4_f32(&scfft, fft_temp);
// 	arm_cmplx_mag_f32(fft_temp, buff, size);
//
// 	for (i = 0; i < size; i++)
// 	{
// 		buff[i] = buff[i] / (size / 2);
// 	}
// #else
// 	fftw_complex *din, *out;
// 	fftw_plan plan;
// 	uint16_t i;
// 	din = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * FFT_POINT);
// 	out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * FFT_POINT);
//
// 	for (i = 0; i < FFT_POINT; i++)
// 	{
// 		din[i][0] = buff[i];
// 		din[i][1] = 0;
// 	}
// 	plan = fftw_plan_dft_1d(FFT_POINT, din, out, FFTW_FORWARD, FFTW_ESTIMATE); //建立一个fft转换计划
// 	fftw_execute(plan);														   //执行转换
// 	fftw_destroy_plan(plan);												   //释放转换plan
// 	for (i = 0; i < FFT_POINT / 2; i++)
// 	{
// 		buff[i] = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) / 4096 * 2;
// 	}
// 	fftw_free(din);
// 	fftw_free(out);
// #endif
//}
//
//
// /**
//  * 轴承诊断
//  */
//static void diag_bearing(float *acc_buff, uint32_t acc_buff_size, const struct BEAR_DIAG_CONFIG_PARA bear_para, struct BEAR_DIAG_VAL *bear_val)
//{
//
//     char debug_buff[32];
//
//     bear_val->bear_save.diag_speed = get_bearing_diag_speed(&bear_val->speed_save);
////     bear_val->bear_save.temp_env = get_bearing_diag_env_temp(bear_val->bear_save.channel_id);
////     bear_val->bear_save.temp_running_gear_temp = get_bearing_diag_running_gear_temp(bear_val->bear_save.channel_id);
////     bear_val->bear_save.temp_alarm_status=get_temp_alarm_status(bear_val->bear_save.channel_id);
//     float wheel_rotation_fre =bear_val->bear_save.diag_speed/ 3.6f / PI /bear_para.wheel_diameter; //轮对轴转频
//
//     float rolling_body_revolution_fre = wheel_rotation_fre * bear_para.para_holder;                   //滚动体的公转频率,与保持架的旋转频率相同
//     float fft_resolution = FS_ACC_B * 1.0f / acc_buff_size;                                             //频率分辨率
//
//     float f_inner_ring = wheel_rotation_fre *bear_para.para_inner_ring;  //内环故障频率
//     float f_outer_ring = wheel_rotation_fre *bear_para.para_outer_ring;  //外环的故障频率 //
//     float f_rolling_body = wheel_rotation_fre * bear_para.para_rolling_body; //滚动体的故障频率//
//     float f_holder = rolling_body_revolution_fre;                    //保持架的故障频率
// 	sprintf(debug_buff, "%f", f_holder);
// 	BEARING_PRINT("f_holder=%s\n", debug_buff);
// 	sprintf(debug_buff, "%f", f_inner_ring);
// 	BEARING_PRINT("f_inner_ring=%s\n", debug_buff);
// 	sprintf(debug_buff, "%f", f_outer_ring);
// 	BEARING_PRINT("f_outer_ring=%s\n", debug_buff);
// 	sprintf(debug_buff, "%f", f_rolling_body);
// 	BEARING_PRINT("f_rolling_body=%s\n", debug_buff);
//
//
//     float outter_1order_fre_amp = 0;                                                        //外环故障幅值
//     float inner_1order_fre_amp = 0;                                                         //内环故障频率
//     float roller_1order_fre_amp = 0;                                                        //滚动体故障频率
//     float holder_1order_fre_amp = 0;                                                            //保持架故障频率
//
//     float outter_1order_fre_hz = 0;
//     float inner_1order_fre_hz = 0;
//     float roller_1order_fre_hz = 0;
//     float holder_1order_fre_hz = 0;
//
//
//     //包络有效值计算
//     bear_val->bear_save.rms_envelope = float_std_rms(acc_buff, acc_buff_size);
//     //进行fft运算
//     fft(acc_buff, acc_buff_size);
//
//
// #if 0
//     float max_amp = 0;
//     for (i = 0; i < 4096; i++)
//     {
//     //  sprintf(debug_buff, "%f", acc_buff[i]);
//     //  BEARING_PRINT("    %s\n", debug_buff);
//     }
//     max_amp = float_get_max(acc_buff + 1, 2047);
//     sprintf(debug_buff, "%f", max_amp);
//     BEARING_PRINT(" max_amp=%s\n", debug_buff);
// #endif
//
//
//     //内环诊断
//     fre_get_fft_max(acc_buff,f_inner_ring - 5,f_inner_ring + 5,&fft_resolution,&inner_1order_fre_amp,&inner_1order_fre_hz);
//     bear_val->bear_save.amplitude_inner_ring= inner_1order_fre_amp;
//
//     //外环诊断
//     fre_get_fft_max(acc_buff,f_outer_ring - 5,f_outer_ring + 5,&fft_resolution,&outter_1order_fre_amp,&outter_1order_fre_hz);
//     bear_val->bear_save.amplitude_outer_ring=outter_1order_fre_amp;
//
//
//     //保持架诊断
//     fre_get_fft_max(acc_buff,f_holder - 5,f_holder + 5,&fft_resolution,&holder_1order_fre_amp,&holder_1order_fre_hz);
//     bear_val->bear_save.amplitude_holder=holder_1order_fre_amp;
//
//     //滚动体诊断
//     fre_get_fft_max(acc_buff,f_rolling_body - 5,f_rolling_body + 5,&fft_resolution,&roller_1order_fre_amp,&roller_1order_fre_hz);
//     bear_val->bear_save.amplitude_rolling_body=roller_1order_fre_amp;
//
// //	sprintf(debug_buff, "%f", inner_1order_fre_hz);
// //	BEARING_PRINT("inner_1order_fre_hz=%s\n", debug_buff);
// //
// //	sprintf(debug_buff, "%f", outter_1order_fre_hz);
// //	BEARING_PRINT("outter_1order_fre_hz=%s\n", debug_buff);
// //
// //	sprintf(debug_buff, "%f", holder_1order_fre_hz);
// //	BEARING_PRINT("holder_1order_fre_hz=%s\n",debug_buff);
// //
// //	sprintf(debug_buff, "%f", roller_1order_fre_hz);
// //	BEARING_PRINT("roller_1order_fre_hz=%s\n", debug_buff);
//
// #ifdef DEV_ALARM_EN
// 	sprintf(debug_buff, "%f", inner_1order_fre_amp);
// 	BEARING_PRINT("inner_1order_fre_amp=%s (0.6067,0.91)\n", debug_buff);
//
// 	sprintf(debug_buff, "%f", outter_1order_fre_amp);
// 	BEARING_PRINT("outter_1order_fre_amp=%s (0.6067,0.91)\n", debug_buff);
//
// 	sprintf(debug_buff, "%f", holder_1order_fre_amp);
// 	BEARING_PRINT("holder_1order_fre_amp=%s (0.4061,0.5414)\n",debug_buff);
//
// 	sprintf(debug_buff, "%f", roller_1order_fre_amp);
// 	BEARING_PRINT("roller_1order_fre_amp=%s (3.7696,4.08)\n", debug_buff);
//
//     //外环
//     if (bear_val->bear_save.amplitude_outer_ring > bear_para.threshold_value_outer_ring_alarm2)
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_outter_ring = 2;
//     }
//     else if (bear_val->bear_save.amplitude_outer_ring > bear_para.threshold_value_outer_ring_alarm1)
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_outter_ring = 1;
//     }
//     else
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_outter_ring = 0;
//     }
//
//     //内环
//     if (bear_val->bear_save.amplitude_inner_ring > bear_para.threshold_value_inner_ring_alarm2)
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_inner_ring = 2;
//     }
//     else if (bear_val->bear_save.amplitude_inner_ring > bear_para.threshold_value_inner_ring_alarm1)
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_inner_ring = 1;
//     }
//     else
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_inner_ring = 0;
//     }
//
//     //滚动体
//     if (bear_val->bear_save.amplitude_rolling_body > bear_para.threshold_value_rolling_body_alarm2)
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_rolling_body = 2;
//     }
//     else if (bear_val->bear_save.amplitude_rolling_body > bear_para.threshold_value_rolling_body_alarm1)
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_rolling_body = 1;
//     }
//     else
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_rolling_body = 0;
//     }
//
//     //保持架
//     if (bear_val->bear_save.amplitude_holder > bear_para.threshold_value_holder_alarm2)
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_holder = 2;
//     }
//     else if (bear_val->bear_save.amplitude_holder > bear_para.threshold_value_holder_alarm1)
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_holder = 1;
//     }
//     else
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_holder = 0;
//     }
//
//     //综合诊断
//     bear_val->bear_save.bearing_alarm_status.alarm_bearing = bear_val->bear_save.bearing_alarm_status.alarm_holder | bear_val->bear_save.bearing_alarm_status.alarm_inner_ring | bear_val->bear_save.bearing_alarm_status.alarm_outter_ring | bear_val->bear_save.bearing_alarm_status.alarm_rolling_body;
//
//     if (bear_val->bear_save.bearing_alarm_status.alarm_bearing == 0)
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_type = RUNNING_OK;
//     }
//     else if (bear_val->bear_save.bearing_alarm_status.alarm_bearing == 1)
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_type = ACC_ALARM_FIRST_GRADE;
// 		//printf("bear_val->ACC_ALARM_FIRST_GRADE----\n");
//     }
//     else
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_type = ACC_ALARM_SECOND_GRADE;
// 		//printf("bear_val->ACC_ALARM_SECOND_GRADE----\n");
//     }
//
//
//     //这个时刻只需要拿温度的报警状态来二次判断，只要温度至少预警就可以上报振动预报警
//     if (get_temp_alarm_status(bear_val->bear_save.channel_id) == RUNNING_OK)
//     {
//         bear_val->bear_save.bearing_alarm_status.alarm_type = RUNNING_OK;
//         bear_val->bear_save.bearing_alarm_status.alarm_bearing=0;
//         bear_val->bear_save.bearing_alarm_status.alarm_holder = 0;
//         bear_val->bear_save.bearing_alarm_status.alarm_rolling_body = 0;
//         bear_val->bear_save.bearing_alarm_status.alarm_inner_ring = 0;
//         bear_val->bear_save.bearing_alarm_status.alarm_outter_ring = 0;
//     }
//#endif
//}
//
///*************************************************
// Function:   　diagnosis_bearing_thread_entry
// Description: 轴承诊断线程
// Input:  无
// Output: 无
// Return: 无
// Others:
// *************************************************/
//void diagnosis_bearing_thread_entry()
//{
//
//	int cnt;
//	uint8_t ch;
//	uint8_t *bearing_send_buff=NULL;
//	struct BEAR_DIAG_VAL tz_bear;
//
//	init_bearing_diag_val(&tz_bear);
//
//#ifdef RT_LINUX
//	pthread_attr_t attr;	  // 线程属性
//	struct sched_param sched; // 调度策略
//	printf("set SCHED_RR policy\n");
//	api_set_thread_policy(&attr, SCHED_RR);
//#endif
//
//	while (1)
//	{
//		sem_wait(&bearing_data->bear_sem);
//		{
////			feed_dog();
////			/************诊断算法*************************/
////			memset(&tz_bear.bear_save, 0, sizeof(struct BEAR_SAVE));
////			memset(&tz_bear.speed_save,0,sizeof(struct SPEED_SAVE));
////			memmove(&tz_bear.bear_save.pub_info,&bearing_data->info,sizeof(struct AD_PUBLIC_INFO));
////			tz_bear.bear_save.channel_id = bearing_data->ch_num - 1;//获取当前诊断通道号  前置1： 0 1 2 3 //前置2： 8 9 10 11
////			if(tz_bear.bear_save.channel_id<PROC_CHANNEL_NUM)
////			{
////				tz_bear.bear_save.pub_info.proc_id=1;
////				ch=tz_bear.bear_save.channel_id;
////			}
////			else
////			{
////				tz_bear.bear_save.pub_info.proc_id=2;
////				ch=tz_bear.bear_save.channel_id-PROC_CHANNEL_NUM;
////			}
////
////			tz_bear.bear_save.self_test=((tz_bear.bear_save.pub_info.proc_id==1)?((bearing_data->pre1_sensor_status>>ch)&0x1):((bearing_data->pre2_sensor_status>>ch)&0x1));
////			tz_bear.bear_save.pub_info.next_station_id=*(uint16_t*)&mvbmsg.station_next[0];
////			tz_bear.bear_save.pub_info.wheel=(int)(master_config_paras->bear_diag_para.wheel_diameter*1000);
////
////			BEARING_PRINT("\r\n<--------diag_bearing_ch%d----->\r\n", tz_bear.bear_save.channel_id + 1);
////			for (cnt = 0; cnt < tz_bear.acc_buff_len; cnt++)
////			{
////				tz_bear.acc_buff[cnt] = convert_ad_bearing(bearing_data->bear_buf[cnt + 4096]);
////			}
////
////			/*轴承算法*/
////			if(master_config_paras->bear_diag_para.wheel_diameter>0)
////			{
////				BEARING_PRINT("\r\n<--------diag_bearing_ch%d----->\r\n", tz_bear.bear_save.channel_id + 1);
////				diag_bearing(tz_bear.acc_buff, tz_bear.acc_buff_len, master_config_paras->bear_diag_para, &tz_bear);
////			}
////			update_mvb_send_bearing_alarm_status(tz_bear.bear_save.channel_id, tz_bear.bear_save.bearing_alarm_status.alarm_type);
////
////
////			if (tz_bear.bear_save.bearing_alarm_status.alarm_type == RUNNING_OK)
////			{
////				BEARING_PRINT("bear: ch%d diag ok...\n", tz_bear.bear_save.channel_id);
////			}
////			else if (tz_bear.bear_save.bearing_alarm_status.alarm_type == ACC_ALARM_FIRST_GRADE)
////			{
////				BEARING_PRINT("bear: ch%d diag acc_alarm_first_grade!!!!!\n", tz_bear.bear_save.channel_id);
////			}
////			else
////			{
////				BEARING_PRINT("bear: ch%d diag acc_alarm_second_grade!!!!!!!!\n", tz_bear.bear_save.channel_id);
////			}
////
////			//判断如果此车位8车，那么需要将数据发往1车，再存储
////			if(master_config_paras->device_num==8)
////			{
////				if (!bearing_send_buff)
////				{
////					uint32_t send_len;
////					send_len = sizeof(struct BEAR_SAVE) + (FS_ACC_B*BEAR_DIAG_STEP* 2) + sizeof(struct SPEED_SAVE); //原始数据字节数
////					bearing_send_buff = (uint8_t *)malloc(send_len);
////					if (bearing_send_buff)
////					{
////						memmove(bearing_send_buff, &tz_bear.bear_save, sizeof(struct BEAR_SAVE));
////						//发送轴承特征数据
////						//client_net_send(UDP,0, TRAIN1_MCU, BEARING_TZ_DATA_TYPE, bearing_send_buff, sizeof(struct BEAR_SAVE));
////
////						//发送轴承特征数据 + 原始数据 + 速度原始数据
////						memmove(&bearing_send_buff[sizeof(struct BEAR_SAVE)], bearing_data->bear_buf, (FS_ACC_B*BEAR_DIAG_STEP * 2));
////						memmove(&bearing_send_buff[sizeof(struct BEAR_SAVE) +(FS_ACC_B*BEAR_DIAG_STEP * 2)], &tz_bear.speed_save, sizeof(struct SPEED_SAVE));
////						//client_net_send(UDP,0, TRAIN1_MCU, BEARING_RAW_DATA_TYPE, &bearing_send_buff[0], send_len);
////						free(bearing_send_buff);
////						bearing_send_buff = NULL;
////					}
////				}
////			}
////			else if(master_config_paras->device_num==1)
////			{
////				if (!bearing_send_buff)
////				{
////					uint32_t send_len;
////					send_len = sizeof(struct BEAR_SAVE) + (FS_ACC_B*FS_TIME* 2) + sizeof(struct SPEED_SAVE); //原始数据字节数
////					bearing_send_buff = (uint8_t *)malloc(send_len);
////					if (bearing_send_buff)
////					{
////
////						memmove(bearing_send_buff, &tz_bear.bear_save, sizeof(struct BEAR_SAVE));
////						//发送轴承特征数据
////						//client_net_send(UDP,0, TRAIN8_MCU, BEARING_TZ_DATA_TYPE, bearing_send_buff, sizeof(struct BEAR_SAVE));
////
////						//发送轴承特征数据 + 原始数据 + 速度原始数据
////						memmove(&bearing_send_buff[sizeof(struct BEAR_SAVE)], bearing_data->bear_buf, (FS_ACC_B*BEAR_DIAG_STEP * 2));
////						memmove(&bearing_send_buff[sizeof(struct BEAR_SAVE) +(FS_ACC_B*BEAR_DIAG_STEP * 2)], &tz_bear.speed_save, sizeof(struct SPEED_SAVE));
////						//client_net_send(UDP,0, TRAIN8_MCU, BEARING_RAW_DATA_TYPE, &bearing_send_buff[0], send_len);
////						free(bearing_send_buff);
////						bearing_send_buff = NULL;
////					}
////				}
////			}
////			/*********************** SAVE DATA TO HDD **********************/
////
////			//train_data_save_func(BEARING_RAW_DATA_TYPE,(uint8_t *)&tz_bear, sizeof(struct BEAR_DIAG_VAL));
////			//train_data_save_func(BEARING_TZ_DATA_TYPE,(uint8_t *)&tz_bear, sizeof(struct BEAR_DIAG_VAL));
////
////
////			spi_flag.spi2_bearing_deal=0;
////			usleep(1000);
//		}
//	}
//}
//
//
///*************************************************
// Function:    init_bearing_diagnosis_thread
// Description: 轴承诊断线程初始化
// Input:
// Output:
// Return:成功:0
// 失败:非0
// Others:
// *************************************************/
//int init_bearing_diagnosis_thread()
//{
//	pthread_t acc_id;
//	int ret;
//	sem_init(&bearing_data->bear_sem, 0, 0); //信号量初始化
//	ret = pthread_create(&acc_id, NULL, (void *)diagnosis_bearing_thread_entry,NULL);
//	if (ret != 0)
//	{
//		DEBUG("Create acc thread error!\n");
//		return ret;
//	}
//	return 0;
//}
//
//
