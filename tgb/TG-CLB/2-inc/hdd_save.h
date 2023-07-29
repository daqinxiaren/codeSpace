#ifndef _HDD_SAVE_H_
#define _HDD_SAVE_H_
#include <stdint.h>
#include "udp_client.h"

//设备TRAIN文件夹定义
#if LOCAL_BOARD==TCLB1_BOARD
#define DIR_TCLB_RAW "/media/LH_DATA/hdd/tclb1/raw/"
#define DIR_TCLB_TZ "/media/LH_DATA/hdd/tclb1/tz/"
#elif LOCAL_BOARD==TCLB2_BOARD
#define DIR_TCLB_RAW "/media/LH_DATA/hdd/tclb2/raw/"
#define DIR_TCLB_TZ "/media/LH_DATA/hdd/tclb2/tz/"
#endif

#define DIR_TCLB_RAW "/media/LH_DATA/hdd/tclb1/raw/"
#define DIR_TCLB_TZ "/media/LH_DATA/hdd/tclb1/tz/"

#define HDD "/media/LH_DATA/hdd/"

#define FREE_LAST_SIZE 14500//10240

enum DEL_DATA_TYPE
{
  TRAIN1_BEAR_RAW,
  TARIN1_PLOY_RAW,
  TARIN1_TEMP_RAW,
};

enum GET_SD_SIZE_TYPE
{
  SD_TOTAL_SIZE, SD_USED_SIZE, SD_FREE_SIZE,
};

//test
struct hdd_tset_t
{
	uint32_t power_cnt;
	uint32_t umount_cnt;
	uint32_t mount_emmc_cnt;
	uint32_t mount_hdd_cnt;

};

/* 数据类型 */
/* 存储数据文件类型 */
struct RAW_TZ_FD_ST{
	int raw_fd;
	int tz_fd;
};

/* 存储设备 */
struct TRAIN_NUM_ST
{
	struct RAW_TZ_FD_ST raw;
	struct RAW_TZ_FD_ST tz;

};
struct TRAIN_DATA_ST
{
	struct TRAIN_NUM_ST train_dat;
};

struct sync_msg
{
	uint8_t time_update_flag;
	uint8_t public_comm_err;
	char cur_rawpath_name[128];
	char cur_tzpath_name[128];
	char old_rawpath_name[128];
	char old_tzpath_name[128];
	uint32_t comm_counter;
};

/**************extern para********************/
extern struct TRAIN_DATA_ST train_data;
extern struct sync_msg syncflag;

void hdd_test_write (struct hdd_tset_t *hdd_t);
void hdd_test_int (struct hdd_tset_t *hdd_t);

extern sem_t delete_dir_sem;
void check_hdd_test(void);
void creat_sys_file_fd(void);
void fsync_sys_fd(void);
void close_sys_fd(void);

/*************************************************
Function:    free_system_cache
Description: 释放系统高速缓存
Input:
Output:
Return:
Others:
*************************************************/
void free_system_cache(void);

/*************************************************
Function:    get_memoccupy
Description: 获取当前系统可用内存大小
Input:
Output:
Return:　可用内存大小
Others:
*************************************************/
uint32_t get_memoccupy (void);

void test_sys_fd(void);
void write_test();
uint8_t check_sdk_file (void);
int check_emmc_size (void);
int delete_sdk_file (uint8_t flile_flag);
void file_del_thread_entry ();
int init_file_del_thread ();
int creat_dir(char *file_dir);

void sync_file_work(uint8_t flag);

void save_sample_data(uint8_t* buf ,int len);

int remove_dir ( char *dir);

#endif
