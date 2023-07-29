/*
 * gpio_app.h
 *
 *  Created on: Mar 1, 2021
 *      Author: linux-ls
 */

#ifndef _GPIO_APP_H_
#define _GPIO_APP_H_

#define USE_LED_GPIO
#ifdef USE_LED_GPIO
#define GPIO_EXPORT0   "echo 27 > /sys/class/gpio/export"//系统运行 面板丝印D4
#define GPIO_OUT0      "echo out > /sys/class/gpio/gpio27/direction"
#define GPIO_VALUE0    "/sys/class/gpio/gpio27/value"

#define GPIO_EXPORT1   "echo 47 > /sys/class/gpio/export"//零速信号输入时常亮，否则熄灭 面板丝印D5
#define GPIO_OUT1      "echo out > /sys/class/gpio/gpio47/direction"
#define GPIO_VALUE1    "/sys/class/gpio/gpio47/value"

#define GPIO_EXPORT2   "echo 46 > /sys/class/gpio/export"//预留/熄灭 面板丝印D6
#define GPIO_OUT2      "echo out > /sys/class/gpio/gpio46/direction"
#define GPIO_VALUE2    "/sys/class/gpio/gpio46/value"
#endif

#define USE_CHECK_GPIO
#ifdef USE_CHECK_GPIO
#define GPIO_PORT_SF   "echo 89 > /sys/class/gpio/export"//SF1/故障信号输出 常态输出高电平，故障时输出低电平
#define GPIO_OUT_SF     "echo out > /sys/class/gpio/gpio89/direction"
#define GPIO_VALUE_SF  "/sys/class/gpio/gpio89/value"

#define GPIO_PORT_SFTSET   "echo 87 > /sys/class/gpio/export"//SF_TEST1输出反馈检测
#define GPIO_IN_SFTSET     "echo in > /sys/class/gpio/gpio87/direction"
#define GPIO_VALUE_SFTSET  "/sys/class/gpio/gpio87/value"//低电平代表控制信号已输出，系统正常

#define GPIO_PORT_WARM   "echo 86 > /sys/class/gpio/export"//WARM1/故障信号输出
#define GPIO_OUT_WARM     "echo out > /sys/class/gpio/gpio86/direction"
#define GPIO_VALUE_WARM  "/sys/class/gpio/gpio86/value"//常态输出低电平，故障时输出高电平

#define GPIO_PORT_WARMTEST   "echo 88 > /sys/class/gpio/export"//WARM_TEST1/故障信号输出反馈检测
#define GPIO_IN_WARMTEST     "echo in > /sys/class/gpio/gpio88/direction"
#define GPIO_VALUE_WARMTEST  "/sys/class/gpio/gpio88/value"//低电平代表故障信号已输出，报警板报警

#define GPIO_PORT_SPEEDTEST   "echo 7 > /sys/class/gpio/export"//零速信号反馈检测
#define GPIO_IN_SPEEDTEST     "echo in > /sys/class/gpio/gpio7/direction"
#define GPIO_VALUE_SPEEDTEST  "/sys/class/gpio/gpio7/value"//低电平代表非零速

#define GPIO_PORT_BJBSAFE  "echo 61 > /sys/class/gpio/export"//BJBSAFE/报警板安全软开关，
#define GPIO_OUT_BJBSAFE     "echo out > /sys/class/gpio/gpio61/direction"
#define GPIO_VALUE_BJBSAFE  "/sys/class/gpio/gpio61/value"//故障需要报警时，输出低电平；常态为高电平

#define GPIO_PORT_BJB   "echo 86 > /sys/class/gpio/export"//JDQ_CT/报警板报警及清除开关
#define GPIO_OUT_BJB     "echo out > /sys/class/gpio/gpio86/direction"
#define GPIO_VALUE_BJB  "/sys/class/gpio/gpio86/value"//故障需要报警时，输出高电平；常态为低电平

#define GPIO_PORT_BJBTEST   "echo 65 > /sys/class/gpio/export"//此功能禁用，无需采集检测
#define GPIO_IN_BJBTEST     "echo in > /sys/class/gpio/gpio65/direction"
#define GPIO_VALUE_BJBTEST  "/sys/class/gpio/gpio65/value"

#define GPIO_PORT_RETEST   "echo 44 > /sys/class/gpio/export"//脉冲复位检测引脚
#define GPIO_IN_RETEST     "echo in > /sys/class/gpio/gpio44/direction"
#define GPIO_VALUE_RETEST  "/sys/class/gpio/gpio44/value"

#endif

#define USE_POWER_GPIO
#ifdef USE_POWER_GPIO
#define GPIO_PORT_V1   "echo 115 > /sys/class/gpio/export"//24V1电压检测 低电平代表24V1电压正常
#define GPIO_IN_V1     "echo in > /sys/class/gpio/gpio115/direction"
#define GPIO_VALUE_V1  "/sys/class/gpio/gpio115/value"

#define GPIO_PORT_V2   "echo 117 > /sys/class/gpio/export"//24V2电压检测 低电平代表24V2电压正常
#define GPIO_IN_V2     "echo in > /sys/class/gpio/gpio117/direction"
#define GPIO_VALUE_V2  "/sys/class/gpio/gpio117/value"
#endif

#ifdef USE_LED_GPIO
void led_sys(uint16_t num);
void led_speed(uint16_t num);
#endif

#ifdef USE_CHECK_GPIO
void dog_ctr(uint8_t value);
int dog_check(uint8_t*val);
void warm_ctr(uint8_t value);
int warm_check(uint8_t*val);
int speed_check(uint8_t*val);
void bjb_ctr(uint8_t value);
int bjb_check(uint8_t*val);
#endif

#ifdef USE_POWER_GPIO
int v1_check(uint8_t*val);
int v2_check(uint8_t*val);
#endif

void gpio_app_init(void);

#endif /* 2_INC_GPIO_APP_H_ */
