/*
 * gpio_app.c
 *
 *  Created on: Mar 1, 2021
 *      Author: linux-ls
 */


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include "gpio_app.h"

static int gpio_read(char* path,uint8_t *value)
{
	int fd=-1,ret=-1;
	char val;
	fd = open(path,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
	if(fd == -1)
	{
		return -1;
	}
	ret = read(fd,&val,sizeof(val));
	if(ret == -1)
	{
		return -1;
	}
	close(fd);

	if(val == '1')
		*value=1;
	else
		*value=0;
	return 0;
}

static int gpio_write(char* path,uint8_t value)
{
	int fd =-1,ret=-1;
	char val ;
	if (value==1)
		val = '1';
	else
		val = '0';

	fd = open(path,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
	if(fd == -1)
	{
		return -1;
	}
	ret = write(fd,&val,sizeof(val));
	if(ret == -1)
	{
		return-1;
	}
	close(fd);
	return 0;
}

#ifdef USE_LED_GPIO
void led_init(void)
{
	/*led1*/
	system(GPIO_EXPORT0);
	system(GPIO_OUT0);

	/*led2*/
	system(GPIO_EXPORT1);
	system(GPIO_OUT1);
	gpio_write(GPIO_VALUE1,1);

	/*led3*/
	system(GPIO_EXPORT2);
	system(GPIO_OUT2);
	gpio_write(GPIO_VALUE2,1);
}

void led_sys(uint16_t num)
{
	uint8_t value;
	if(num%2)
		value=1;
	else
		value=0;
	if(gpio_write(GPIO_VALUE0,value)==-1)
		printf("gpio_write err--led_sys\n");

}

void led_speed(uint16_t num)
{
	uint8_t value;
	if(num%2)
		value=1;
	else
		value=0;
	if(gpio_write(GPIO_VALUE1,value)==-1)
		printf("gpio_write err--led_speed\n");

}

#endif

#ifdef USE_CHECK_GPIO
void gpio_check_init(void)
{
	system(GPIO_PORT_SF);
	system(GPIO_OUT_SF);
	gpio_write(GPIO_VALUE_SF,1);

//	system(GPIO_PORT_SFTSET);
//	system(GPIO_IN_SFTSET);

	system(GPIO_PORT_WARM);
	system(GPIO_OUT_WARM);
	gpio_write(GPIO_VALUE_WARM,0);

//	system(GPIO_PORT_WARMTEST);
//	system(GPIO_IN_WARMTEST);

	system(GPIO_PORT_SPEEDTEST);
	system(GPIO_IN_SPEEDTEST);
//脱轨报警的开关及安全软开关
	system(GPIO_PORT_BJB);
	system(GPIO_OUT_BJB);
	gpio_write(GPIO_VALUE_BJB,0);
	system(GPIO_PORT_BJBSAFE);
	system(GPIO_OUT_BJBSAFE);
	gpio_write(GPIO_VALUE_BJBSAFE,0);
//脱轨切除检测引脚
	system(GPIO_PORT_BJBTEST);
	system(GPIO_IN_BJBTEST);
//脉冲复位检测引脚
	system(GPIO_PORT_RETEST);
	system(GPIO_IN_RETEST);

}

void sf_ctr(uint8_t value)
{
	if(gpio_write(GPIO_VALUE_SF,value)==-1)
		printf("gpio_write err--sf_ctr\n");
}

int sf_check(uint8_t*val)
{
	if(gpio_read(GPIO_VALUE_SFTSET,val)==-1)
	{
		printf("gpio_write err--sf_check\n");
		return -1;
	}
	return 0;
}

void warm_ctr(uint8_t value)
{
	if(gpio_write(GPIO_VALUE_WARM,value)==-1)
		printf("gpio_write err--warm_ctr\n");
}

int warm_check(uint8_t*val)
{
	if(gpio_read(GPIO_VALUE_WARMTEST,val)==-1)
	{
		printf("gpio_write err--warm_check\n");
		return -1;
	}
	return 0;
}
int speed_set(uint8_t val)
{
	printf("----------speed_set val   %d ------\n",val);
	if(val)
		led_speed(1);
	else
		led_speed(0);
}
int speed_check(uint8_t*val)
{
	if(gpio_read(GPIO_VALUE_SPEEDTEST,val)==-1)
	{
		printf("gpio_write err--power_check\n");
		return -1;
	}
	return 0;
}

void bjb_ctr(uint8_t value)
{
	if (1 == value)//故障报警
	{
		if(gpio_write(GPIO_VALUE_BJB,value)==-1)//报警引脚控制
			printf("gpio_write err--bjb_ctr\n");
		if(gpio_write(GPIO_VALUE_BJBSAFE,value)==-1)//安全软开关控制
			printf("gpio_write err--bjb_ctr\n");
		printf("gpio   set bjb  alarm!!!\n");
	}
	else if (0 == value)//清除报警
	{
		if(gpio_write(GPIO_VALUE_BJB,value)==-1)//报警引脚控制
			printf("gpio_write err--bjb_ctr\n");
		if(gpio_write(GPIO_VALUE_BJBSAFE,value)==-1)//安全软开关控制
			printf("gpio_write err--bjbsafe_ctr\n");
		printf("gpio  clean bjb  alarm!!!\n");
	}
	//printf("gpio_set bjb  alarm!!!\n");
}

void bjb_safe_ctr(uint8_t value)
{
	if (1 == value)//故障报警
	{
		if(gpio_write(GPIO_VALUE_BJBSAFE,value)==-1)//安全软开关控制
			printf("gpio_write err--GPIO_VALUE_BJBSAFE\n");
		printf("gpio   set bjb  safe open!!!\n");
	}
}

int bjb_check(uint8_t*val)
{
	uint8_t val_cut = 0;
	uint8_t val_reset = 0;
	//报警板脱轨切除状态检测，主机有切除开关的项目适用
	if(gpio_read(GPIO_VALUE_BJBTEST,&val_cut)==-1)
	{
		printf("gpio_write err--bjb_check\n");
		return -1;
	}
	//复位脉冲引脚检测，支持复位脉冲输入的项目专用，目前只有西安10可用
	if(gpio_read(GPIO_VALUE_RETEST,&val_reset)==-1)
	{
		printf("gpio_write err--bjb_check\n");
		return -1;
	}
	if(val_reset)
	{
		//usleep(2000);//检测是否有持续200ms的高电平脉冲输入
		if(gpio_read(GPIO_VALUE_RETEST,&val_reset)==-1)
		{
			printf("gpio_write err--bjb_check\n");
			return -1;
		}
	}
	if ((val_cut==0) || (val_reset==1))
	{
		*val = 1;
	}
	else *val = 0;
	return 0;
}

#endif

#ifdef USE_POWER_GPIO
void power_test_init(void)
{
	system(GPIO_PORT_V1);
	system(GPIO_IN_V1);

	system(GPIO_PORT_V2);
	system(GPIO_IN_V2);
}

int v1_check(uint8_t*val)
{
	if(gpio_read(GPIO_VALUE_V1,val)==-1)
	{
		printf("gpio_write err--v1_check\n");
		return -1;
	}
	return 0;
}

int v2_check(uint8_t*val)
{
	if(gpio_read(GPIO_VALUE_V2,val)==-1)
	{
		printf("gpio_write err--v2_check\n");
		return -1;
	}
	return 0;
}

void gpio_app_init(void)
{

#ifdef USE_LED_GPIO
	led_init();
#endif

#ifdef USE_CHECK_GPIO
	gpio_check_init();
#endif

#ifdef USE_POWER_GPIO
	power_test_init();
#endif

}

#endif

