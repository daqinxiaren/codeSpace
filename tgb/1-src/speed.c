//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "speed.h"
//#include "lh_math.h"
//#include "ringbuffer.h"
//#include <stdint.h>
//static struct SPEED_PARA polygon_speed_para;
//static struct SPEED_PARA bearing_speed_para;
//static struct rt_ringbuffer polygon_speed_rb;
//static struct rt_ringbuffer bearing_speed_rb;
///**
// * 速度缓存初始化
// */
//void init_speed_para(void)
//{
//	memset(&polygon_speed_para, 0, sizeof(struct SPEED_PARA));
//	memset(&bearing_speed_para, 0, sizeof(struct SPEED_PARA));
//	rt_ringbuffer_init(&polygon_speed_rb, (uint8_t *)polygon_speed_para.speed_buff, SPEED_BUFF_SIZE * sizeof(uint16_t));
//	rt_ringbuffer_init(&bearing_speed_rb, (uint8_t *)bearing_speed_para.speed_buff, SPEED_BUFF_SIZE * sizeof(uint16_t));
//};
//
///**
// * 获取轴承诊断的速度,
// * 返回speed_save结构体和平均速度
// */
//float get_bearing_diag_speed(struct SPEED_SAVE *speed_save)
//{
//	int i;
//	uint16_t recv_buff[15*10];
//	float s = 0;
//	//读取环形缓存区的大小
//	int len = rt_ringbuffer_data_len(&bearing_speed_rb);
//	if (len == 0)
//	{
//		speed_save->speed_len=0;
//		return 0;
//	}
//	//判断环形缓冲区的速度个数是否达到算法速度的最低要求,速度每秒15个
//	if (len >= FS_MIN_SPEED * sizeof(uint16_t)*FS_TIME)  //
//	{
//		//从环形缓存区读取所有的速度
//		rt_ringbuffer_get(&bearing_speed_rb, (uint8_t *)recv_buff, len);
//		speed_save->speed_len=FS_MIN_SPEED*FS_TIME;//
//        memmove(speed_save->speed_buff,&recv_buff,speed_save->speed_len*2);
//        rt_ringbuffer_reset(&bearing_speed_rb);
//		for (i = 0; i < speed_save->speed_len; i++)
//		{
//			s += recv_buff[i] * 0.01;
//		}
//		s = s / i;
//		return s;
//	}
//	else if (len >= FS_MIN_SPEED * sizeof(uint16_t))  //主机轴承切换一次
//	{
//		//从环形缓存区读取所有的速度
//		rt_ringbuffer_get(&bearing_speed_rb, (uint8_t *)recv_buff, len);
//		speed_save->speed_len=FS_MIN_SPEED;//
//        memmove(speed_save->speed_buff,&recv_buff,speed_save->speed_len*2);
//        rt_ringbuffer_reset(&bearing_speed_rb);
//		for (i = 0; i < speed_save->speed_len; i++)
//		{
//			s += recv_buff[i] * 0.01;
//		}
//
//		s = s / i;
//		return s;
//	}
//	else
//	{
//		speed_save->speed_len=0;
//		//如果未达到速度缓存区的算法个数要求，则强制清空缓存区
//		rt_ringbuffer_reset(&bearing_speed_rb);
//		return 0;
//	}
//}
//
///**
// * 获取多边形诊断的速度缓存和个数
// * 如果中间发生时间和速度中断，则判定为速度失效
// */
//int get_polygon_diag_speed(float speed_buff[],struct SPEED_SAVE *speed_save)
//{
//	int i;
//	uint16_t recv_buff[15*120];
//
//	//读取环形缓存区的大小
//	int len = rt_ringbuffer_data_len(&polygon_speed_rb);
//	if (len == 0)
//	{
//		return 0;
//	}
//
//	//判断环形缓冲区的速度个数是否达到算法速度的最低要求
//	if (len >= FS_MIN_SPEED * sizeof(uint16_t) * FS_TIME)
//	{
//		rt_ringbuffer_get(&polygon_speed_rb, (uint8_t *)recv_buff, len);
//
//		speed_save->speed_len=FS_MIN_SPEED*FS_TIME;
//
//        memmove(speed_save->speed_buff,recv_buff,speed_save->speed_len*2);
//
//        rt_ringbuffer_reset(&polygon_speed_rb);
//		for (i = 0; i < speed_save->speed_len; i++)
//		{
//			speed_buff[i] = recv_buff[i] * 0.01;
//		}
//		return i;
//	}
//	else if(len >= FS_MIN_SPEED * sizeof(uint16_t) )
//	{
//		rt_ringbuffer_get(&polygon_speed_rb, (uint8_t *)recv_buff, len);
//
//		speed_save->speed_len=len/sizeof(uint16_t);
//
//        memmove(speed_save->speed_buff,recv_buff,len);
//
//        rt_ringbuffer_reset(&polygon_speed_rb);
//		for (i = 0; i < speed_save->speed_len; i++)
//		{
//			speed_buff[i] = recv_buff[i] * 0.01;
//		}
//		return i;
//	}
//	else
//	{
//		speed_save->speed_len=0;
//		//如果未达到速度缓存区的算法个数要求，则强制清空缓存区
//		rt_ringbuffer_reset(&polygon_speed_rb);
//		return 0;
//	}
//}
//
///**
// * 填充速度缓存
//*/
//void write_speed_buff(uint16_t speed_buff[], int size)
//{
//	int  i;
//	float s=0;
//	rt_ringbuffer_put(&bearing_speed_rb, (uint8_t *)speed_buff, size * sizeof(uint16_t));
//	rt_ringbuffer_put(&polygon_speed_rb, (uint8_t *)speed_buff, size * sizeof(uint16_t));
//
//	for(i=0;i<size;i++)
//	{
//       s+=speed_buff[i];
//	}
//
//	s=s/size;
//	bearing_speed_para.mean_speed=s*0.01;//0.01速度的分辨率
//	polygon_speed_para.mean_speed=s*0.01;//0.01速度的分辨率
//
////	printf("polygon_rb size=%d\n",rt_ringbuffer_data_len(&polygon_speed_rb));
////	printf("bearing_speed_para.mean_speed=%f\n",bearing_speed_para.mean_speed);
////	printf("polygon_speed_para.mean_speed=%f\n",polygon_speed_para.mean_speed);
//
//}
//
//
///**
// * 轴承平均速度
// * */
//float get_mean_speed(void)
//{
//	return bearing_speed_para.mean_speed;
//}
//
///**
// * 清除速度缓存
// * */
//void reset_bear_speed_rb(void)
//{
//	rt_ringbuffer_reset(&bearing_speed_rb);
//}
///**
// * 清除速度缓存
// * */
//void reset_ploygon_speed_rb(void)
//{
//	rt_ringbuffer_reset(&polygon_speed_rb);
//}
//
//
//
//
