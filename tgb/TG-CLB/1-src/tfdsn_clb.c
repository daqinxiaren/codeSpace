/*
 ============================================================================
 Name        : LH-TFDSE-CLB
 Author      : liushuang
 Version     : v1.0.0.0
 Copyright   : Your copyright notice
 Description : lh tfdse in C, Ansi-style
 ============================================================================
 PIS----ETH0---外部 ETH1
 NET----ETH1---外部 ETH3
 PTU----ETH2---外部 ETH2
 */
/**
*                _ooOoo_
*               o8888888o
*                    88" . "88
*                    (| -_- |)
*               O\ = /O
*                ____/`---'\____
*              .   ' \\| |// `.
*               / \\||| : |||// \
*             / _||||| -:- |||||- \
*               | | \\\ - /// | |
*             | \_| ''\---/'' | |
*              \ .-\__ `-` ___/-. /
*           ___`. .' /--.--\ `. . __
*        ."" '< `.___\_<|>_/___.' >'"".
*       | | : `- \`.;`\ _ /`;.`/ - ` : | |
*         \ \ `-. \_ __\ /__ _/ .-` / /
* ======`-.____`-.___\_____/___.-`____.-'======
*                    `=---='
*
* .............................................
*          佛祖保佑             永无BUG
*/



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <poll.h>
#include <fcntl.h>
#include "ploygon_diagnosis.h"
#include "user_data.h"
#include "watchdog.h"
#include "error.h"
#include "spi_app.h"
#include <unistd.h>
#include "IIR_polygon.h"

int main(int argc ,char **argv)
{
		int ret = -1;

		/****************看门狗初始化******************************/
		init_hw_watchdog();
		//init_watchdog(40); //看门狗溢出时间40s

		/****************CMD CONFIG******************************/
		cmd_input_check(argc,argv);
		/****************IP CONFIG******************************/
		config_ini_file_init();
		/*************UDP线程***************************/
		init_udp();

		/*************脱轨检测线程*************************/
		ret = init_derail_thread();
		if (ret !=0)
		{
			DEBUG("derail init failed!\n");
		}
		/************算法模块初始化******************************/
		ret = fft_diag_init();
		if (ret !=0)
		{
			DEBUG("fft diag init failed!\n");
		}
		/************spi线程初始化******************************/
		ret = init_spi1_thread();
		if (ret !=0)
		{
			DEBUG("spi1 init failed!\n");
		}

		ret = init_spi2_thread();
		if (ret !=0)
		{
			DEBUG("spi2 init failed!\n");
		}
		printf("  TFDSN-TCLB  update time :2023-06-28 09:39\n");
		/************线程初始化******************************/
		ret = init_file_del_thread();
		if (ret !=0)
		{
			DEBUG("init_file_del_thread failed!\n");

		}

		/***************自检和初始化线程初始化***********************/
		ret = init_sec_thread();
		if(ret != 0)
		{
			DEBUG("sec thread init failed!\r\n");
		}

		while(1)
		{
//			for (int i=0; i<20; i++)
//			{
//			    test_result =IIR_filter_polygon((float)testbuff[i]);
//			    printf("test iir filter  %f + %f\n",testbuff[i],test_result);
//			}
//		    printf("\n");
			DEBUG("\ -----------test---------\n");
			sleep(5);
		}

	return 0;
}

void cmd_input_check(int argc,char** argv)
{

	init_data();
}

