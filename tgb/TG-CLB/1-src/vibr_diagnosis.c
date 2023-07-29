///*
// * vibr_diagnosis.c
// *
// *  Created on: Nov 2, 2020
// *      Author: linux-ls
// */
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
//#include <math.h>
//#include <signal.h>
//#include <sys/time.h>
//#include <semaphore.h>
//#include <linux/can.h>
//#include <sys/socket.h>
//#include <net/if.h>
//#include <linux/can/raw.h>
//#include "user_data.h"
//#include "vibr_diagnosis.h"
//#include "hdd_save.h"
//#include "lh_math.h"
//
//#include "pthread_policy.h"
//#include "sec_app.h"
//
//
//struct VIBR_CALC tz_vibr;
//
//extern struct spi_flag_st spi_flag;
//extern struct MASTER_CLB_CONFIG_PARA *master_config_paras;
//
///**
// * 初始参数和缓存
// */
//void init_vibr_diag(struct VIBR_CALC *tz_vibrs)
//{
//    memset(tz_vibrs, 0, sizeof(struct VIBR_CALC));
//
//    tz_vibrs->acc_buff_len = FS_ACC_V * 3;
//
//    if (!tz_vibrs->vibr_buf)
//    {
//    	tz_vibrs->vibr_buf = (uint16_t *)malloc(tz_vibrs->acc_buff_len * 2);
//
//        if (tz_vibrs->vibr_buf)
//        {
//            memset(tz_vibrs->vibr_buf, 0, tz_vibrs->acc_buff_len*2);
//            //printf("tz_vibrs->vibr_buf ok\n");
//        }
//        else
//        {
//        	printf("tz_vibrs->vibr_buf err\n");
//        }
//    }
//}
//
//void vibr_acc_datadeal(void)
//{
////	if(master_config_paras->device_num == 1)
////	{
//////		if(mvb_t.use_train1_flag)
//////			memmove(tz_vibr.speed_save.speed_buff,mvbmsg.speed_buf,30);
//////		else
//////			memmove(tz_vibr.speed_save.speed_buff,netmsg.speed_buf,30);
////	}
////	else if(master_config_paras->device_num == 8)
////	{
//////		if(mvb_t.use_train8_flag)
//////			memmove(tz_vibr.speed_save.speed_buff,mvbmsg.speed_buf,30);
//////		else
//////			memmove(tz_vibr.speed_save.speed_buff,netmsg.speed_buf,30);
////	}
////
////	tz_vibr.speed_save.speed_len=30;
////	tz_vibr.vibr_save.pub_info.next_station_id=*(uint16_t*)&mvbmsg.station_next[0];
////	tz_vibr.vibr_save.ch_num=spi_flag.spi_zd_sample_ch;
////	tz_vibr.vibr_save.pub_info.wheel=(int)(master_config_paras->polygon_diag_para.wheel_diameter*1000);
////	train_data_save_func(VIBR_RAW_DATA_TYPE,(uint8_t *)&tz_vibr, sizeof(struct VIBR_CALC));
//
//}
//
