/*
 * vibr_diagnosis.h
 *
 *  Created on: Nov 2, 2020
 *      Author: linux-ls
 */

#ifndef _VIBR_DIAGNOSIS_H
#define _VIBR_DIAGNOSIS_H


#include <stdint.h>

#define FS_ACC_V 3000
/**
 * vibr的特征数据
 */
struct VIBR_SAVE
{
	struct AD_PUBLIC_INFO pub_info;
	uint8_t ch_num;//1-8
	uint8_t rcc1[3];
	int rcc2[44];
};


/**
 * 计算过程
 */
struct VIBR_CALC
{

    uint16_t *vibr_buf;
    int acc_buff_len;
    int speed_buff_len;
    struct VIBR_SAVE vibr_save;

};

void init_vibr_diag(struct VIBR_CALC *tz_vibrs);
void vibr_acc_datadeal(void);
extern struct VIBR_CALC tz_vibr;

#endif
