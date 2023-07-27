#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <signal.h>
#include <sys/time.h>
#include <linux/can.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can/raw.h>
#include <semaphore.h>
#include "user_data.h"
#include "hdd_save.h"
#include "bearing_diagnosis.h"
#include "ploygon_diagnosis.h"
#include "vibr_diagnosis.h"
#include "crc_table.h"
#include "spi_app.h"
#include <dirent.h>
#include <sys/vfs.h>
#include <unistd.h>
#include <sys/stat.h>

#define HDD_DEV   "/dev/hdd"             //SD卡设备文件


/*--------------- private para ------------------------*/
struct TRAIN_DATA_ST train_data;
struct sync_msg syncflag;
struct hdd_tset_t hdds;

//test
int test_flie_fd[8];
sem_t sync_dir_sem;

int test_hdd_fd=-1;
void hdd_int (struct hdd_tset_t *hdd_t)
{
	int ret;
	uint8_t buf[128];
	test_hdd_fd = open("/media/LH_DATA/hdd_test.dat" , O_RDWR | O_CREAT ,0777);//O_CREAT|O_RDWR|O_APPEND|__O_LARGEFILE,0777
	if (test_hdd_fd < 0)
	{
		printf("test_hdd_fd open err\n");
		return ;
	}
	ret = read(test_hdd_fd,buf,sizeof(struct hdd_tset_t));

	if(ret<0)
		return;
	memmove(&hdds,(struct hdd_tset_t*)buf,ret);

	close(test_hdd_fd);
	test_flie_fd[4]=-1;
	printf("hdd_test_int success \n");
}

void hdd_test_write (struct hdd_tset_t *hdd_t)
{
	int ret;
	test_hdd_fd = open("/media/LH_DATA/hdd_test.dat" , O_RDWR ,0777);//O_CREAT|O_RDWR|O_APPEND|__O_LARGEFILE,0777
	if (test_hdd_fd < 0)
	{
		printf("test_hdd_fd open err\n");
		return ;
	}
	ret = write(test_hdd_fd,hdd_t,sizeof(struct hdd_tset_t));

	if(ret<0)
		return;
	close(test_hdd_fd);
	test_hdd_fd=-1;
	printf("hdd_test_write success \n");
	printf("power_cnt:%d \n",hdd_t->power_cnt);
	printf("umount_cnt:%d \n",hdd_t->umount_cnt);
	printf("mount_emmc_cnt:%d \n",hdd_t->mount_emmc_cnt);
	printf("mount_hdd_cnt:%d \n",hdd_t->mount_hdd_cnt);

}
/*
* 创建年月日文件目录名
*/
void create_ymd_dir_name(char *name)
{
	 struct LOCAL_TIME time_now;
    get_local_time(&time_now);
    sprintf(name,"%04d%02d%02d",time_now.year,time_now.mon,time_now.day);
   //sprintf(tar,"%04d%02d%02d/%02d%02d%02d",time_now.year,time_now.mon,time_now.day,time_now.hour,time_now.min,time_now.sec);
}


/*************************************************
Function:    free_system_cache
Description: 释放系统高速缓存
Input:
Output:
Return:
Others:
*************************************************/
void free_system_cache(void)
{
	system("echo 1 > /proc/sys/vm/drop_caches");
	system("echo 2 > /proc/sys/vm/drop_caches");
	system("echo 3 > /proc/sys/vm/drop_caches");
}


/*************************************************
Function:    get_memoccupy
Description: 获取当前系统可用内存大小
Input:
Output:
Return:　可用内存大小
Others:
*************************************************/
uint32_t get_memoccupy (void)
{
    FILE *fd;

    char buff[256];
    char name[20];
    uint32_t total;
    char name2[20];
    uint32_t free;
    fd = fopen ("/proc/meminfo", "r");
    fgets (buff, sizeof(buff), fd);

    sscanf (buff, "%s %d %s", name, &total,name2);
    fgets (buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里

    sscanf (buff, "%s %d %s", name, &free,name2);
   // DEBUG("%s%d%s \r\n",name,free,name2);

    fclose(fd);     //关闭文件fd

    if(free<150000)
    	free_system_cache();

    return free;
 }

/*
 * 创建年月日时分秒文件目录名
 */
static void create_ymd_hms_dir_name(char *ymddir,char *ymdhmddir)
{
	  struct LOCAL_TIME time_now;
     get_local_time(&time_now);
     sprintf(ymdhmddir,"%04d%02d%02d/%02d%02d%02d",time_now.year,time_now.mon,time_now.day,time_now.hour,time_now.min,time_now.sec);
     sprintf(ymddir,"%04d%02d%02d",time_now.year,time_now.mon,time_now.day);
     //sprintf(tar,"%04d%02d%02d/%02d%02d%02d",time_now.year,time_now.mon,time_now.day,time_now.hour,time_now.min,time_now.sec);
}


/*
 * 创建年月日文件目录
 */
static int create_sys_ymd_dir(char *ymd)
{
	char dif_buf[128];

	if(ymd==NULL)
		return -1;

	memset(dif_buf,0,sizeof(dif_buf));
	strcpy(dif_buf,DIR_TCLB_RAW);
	strcat(dif_buf,ymd);
	printf("create_sys_ymd_dir  mkdir :%s\n",dif_buf);
	if (access(dif_buf, F_OK) != 0)
	{
		mkdir(dif_buf,0777);
		chmod(dif_buf,0777);
		printf("mkdir date dir :%s\n",ymd);
	}

	memset(dif_buf,0,sizeof(dif_buf));
	strcpy(dif_buf,DIR_TCLB_TZ);
	strcat(dif_buf,ymd);
	if (access(dif_buf, F_OK) != 0)
	{
		mkdir(dif_buf,0777);
		chmod(dif_buf,0777);
	}
	return 0;
}

/*************************************************
Function:  creat_sys_file_fd
Description: 创建系统的文件
Input: 	无
Output:
Return: 无
Others:
*************************************************/
static void creat_sys_file(char * ymdhms)
{
	 char file_buf[128];

	if(ymdhms==NULL)
		return;

	//原始数据文件
	memset(file_buf,0,sizeof(file_buf));
	strcpy(file_buf,DIR_TCLB_RAW);
	strcat(file_buf,ymdhms);
	strcat(file_buf,"_raw.dat");
	printf("new:%s\n",file_buf);
	train_data.train_dat.raw.raw_fd=open(file_buf,O_CREAT|O_RDWR|O_APPEND|__O_LARGEFILE,0777);
	if(train_data.train_dat.raw.raw_fd<0)
	{
		printf("open raw_fd filed");
		return;
	}
	memset(syncflag.cur_rawpath_name,0,128);
	memcpy(syncflag.cur_rawpath_name,file_buf,128);

	memset(file_buf,0,sizeof(file_buf));
	strcpy(file_buf,DIR_TCLB_TZ);
	strcat(file_buf,ymdhms);
	strcat(file_buf,"_tz.dat");
	printf("new:%s\n",file_buf);
	train_data.train_dat.tz.tz_fd=open(file_buf,O_CREAT|O_RDWR|O_APPEND|__O_LARGEFILE,0777);
	if(train_data.train_dat.tz.tz_fd<0)
	{
		perror("open tz_fd filed");
		return;
	}
	memset(syncflag.cur_tzpath_name,0,128);
	memcpy(syncflag.cur_tzpath_name,file_buf,128);

}

/*************************************************
Function:  creat_sys_file_fd
Description: 创建系统的文件
Input: 	无
Output:
Return: 无
Others:
*************************************************/
void creat_sys_file_fd(void)
{

	char ymd[64]={0},ymdhms[64]={0};

	create_ymd_hms_dir_name(ymd,ymdhms);

	if(create_sys_ymd_dir(ymd)==0)
	{
		creat_sys_file(ymdhms);//数据文件
	}

}


/*************************************************
Function:  fsync_sys_fd
Description: 更新刷新系统的文件
Input: 	无
Output:
Return: 无
Others:
*************************************************/
void fsync_sys_fd(void)
{
	fsync(train_data.train_dat.tz.tz_fd);
	fsync(train_data.train_dat.raw.raw_fd);
}

/*************************************************
Function:  close_sys_fd
Description: 更新刷新系统的文件
Input: 	无
Output:
Return: 无
Others:
*************************************************/
void close_sys_fd(void)
{
	close(train_data.train_dat.raw.raw_fd);
	close(train_data.train_dat.tz.tz_fd);
	train_data.train_dat.raw.raw_fd=-1;
	train_data.train_dat.tz.tz_fd=-1;
}

void sync_file_work(uint8_t flag)
{
	if(flag)
	{
		fsync_sys_fd();
		close_sys_fd();

		memset(syncflag.old_tzpath_name,0,128);
		memcpy(syncflag.old_tzpath_name,syncflag.cur_tzpath_name,128);
		memset(syncflag.old_rawpath_name,0,128);
		memcpy(syncflag.old_rawpath_name,syncflag.cur_rawpath_name,128);

		sem_post(&sync_dir_sem);
		creat_sys_file_fd();
	}
}

void save_sample_data(uint8_t* buf ,int len)
{
	if(train_data.train_dat.raw.raw_fd>0)
	{
		write(train_data.train_dat.raw.raw_fd,buf,len);
		fsync(train_data.train_dat.raw.raw_fd);
	}
}

/*************************************************
 Function:    init_del_thread
 Description: 初始化线程
 Input:
 Output:
 Return:成功：0
 失败:非0
 Others:
 *************************************************/
int init_file_del_thread ()
{
  pthread_t file_del_thread_id;
  int ret;
  sem_init (&sync_dir_sem, 0, 0); //信号量初始化
  ret = pthread_create (&file_del_thread_id, NULL, (void *) file_del_thread_entry, NULL);
  if (ret != 0)
  {
    DEBUG("Create file del thread error!\n");
    return ret;
  }
  return 0;
}

/*文件目录删除线程*/
void file_del_thread_entry ()
{
	sleep (1);
	//对时完成后,可以执行数据删除线程
	sem_wait (&sync_dir_sem);
	while (1)
	{
		sem_wait (&sync_dir_sem);
		{
			if(syncflag.public_comm_err)
			{
				remove_dir(syncflag.old_rawpath_name);
				remove_dir(syncflag.old_tzpath_name);
			}
			else
			{
				system("/usr/local/sync_data_put.sh ");
			}

		}

	}

}

/*************************************************
 Function:  remove_dir
 Description:删除指定路径中的文件及该路径
 Input: 	需要删除的路径
 Output:
 Return: 0:ok  -1:err
 Others:
 *************************************************/
int remove_dir ( char *dir)
{

  struct stat dir_stat;
  // 参数传递进来的目录不存在，直接返回
  if (0 != access (dir, F_OK))
  {
    return 0;
  }

  // 获取目录属性失败，返回错误
  if (0 > stat (dir, &dir_stat))
  {
    perror ("get directory stat error");
    return -1;
  }

  if (S_ISREG(dir_stat.st_mode))
  {  // 普通文件直接删除
    printf ("remove dir:%s\n", dir);
    remove (dir);
    //usleep (200000);
    return 0;
  }
  else
  {
    //perror ("unknow file type!");
    return -1;
  }
}

int delete_sdk_file (uint8_t flile_flag)
{
  DIR * dir_t=NULL;
  uint8_t first_use_flag = 0;
  char min_names[128];
  struct dirent * ptr = NULL;
  char dir1_file[128];
  memset (min_names, 0, sizeof(min_names));

//  switch (flile_flag)
//  {
//    case TRAIN1_BEAR_RAW:
//      strcpy (dir1_file, DEL_TRAIN1_BEAR_RAW);
//      dir_t = opendir (DEL_TRAIN1_BEAR_RAW);
//      break;
//    case TARIN1_PLOY_RAW:
//      strcpy (dir1_file, DEL_TARIN1_PLOY_RAW);
//      dir_t = opendir (DEL_TARIN1_PLOY_RAW);
//      break;
//    default:
//      break;
//  }

  if (dir_t == NULL)
  {
    return -1;
  }

  while ((ptr = readdir(dir_t)) != NULL)
  {
    //printf ("ptr->d_name:%s\n", ptr->d_name);

	if((strcmp(ptr->d_name,".")==0) || (strcmp(ptr->d_name,"..")==0))
		continue;
    if (!first_use_flag)
    {
      strcpy (min_names, ptr->d_name);
      //printf ("min_names:%s\n", min_names);
      first_use_flag = 1;
    }
    else
    {
      if (strcmp (min_names, ptr->d_name) >= 0)			//找到最早日期路径
      {
    	  strcpy (min_names, ptr->d_name);
      }
    }
  }

  if(strlen(min_names)<1)
	return 0;

  strcat (dir1_file, min_names);
  return remove_dir (dir1_file);
}


unsigned long long int get_sd_size (char *dev_path, enum GET_SD_SIZE_TYPE type)
{

  struct statfs statFS;
  unsigned long long int capacity;
  unsigned long long int usedBytes = 0;
  unsigned long long int freeBytes = 0;
  unsigned long long int totalBytes = 0;
  unsigned long long int endSpace = 0;

 // printf ("dev_path:%s\n", dev_path);
  if (statfs (dev_path, &statFS) == -1)
  {
    printf ("statfs failed for path->[%s]\n", dev_path);
    return (-1);
  }

  /***得到字节数据****//*块数乘以每一块的字节数*/
  totalBytes = (uint64_t) statFS.f_blocks * (uint64_t) statFS.f_bsize;

  freeBytes = (uint64_t) statFS.f_bavail * (uint64_t) statFS.f_bsize;

  usedBytes = totalBytes - freeBytes;


  switch (type)
  {
    case SD_TOTAL_SIZE:
      endSpace = totalBytes / 1024 / 1024;			//单位MB

      break;

    case SD_USED_SIZE:
      endSpace = usedBytes / 1024 / 1024;					//单位MB
      break;

    case SD_FREE_SIZE:
      endSpace = freeBytes / 1024 / 1024;					//单位MB

     // printf ("sd_free_size:%lld\n", endSpace);
      break;

    default:
      return (-1);
  }
  capacity = endSpace;

  return capacity;
}


//检测　ＨＤＤ剩余空间
uint8_t check_sdk_file (void)
{

  uint8_t del_data_file_flag=0;
  unsigned long long int free_capacity = 0;
  unsigned long long int tatol_capacity = 0;

  //获取剩余空间
  free_capacity = get_sd_size (HDD, SD_FREE_SIZE);
  printf ("free_capacity---------------------------->:%lld MB\n", free_capacity);

  //获取总大小空间
  tatol_capacity = get_sd_size (HDD, SD_TOTAL_SIZE);
  printf ("tatol_capacity--------------------------->:%lld MB\n", tatol_capacity);

  if(tatol_capacity<10240)//防止硬盘挂载到ｅｍｍｃ
  {

	  del_data_file_flag=2;
	  return del_data_file_flag;
  }

  if (free_capacity <  FREE_LAST_SIZE)
  {
	 if(tatol_capacity>FREE_LAST_SIZE)
		 del_data_file_flag = 1;
  }
  else
  {
	  del_data_file_flag = 0;
  }
  return del_data_file_flag;
}


/*************************************************
 Function:
 Description://检测EMMC剩余空间
 Input:
 Output:
 Return: 0:
 Others:
 *************************************************/
int check_emmc_size (void)
{
	//int ret=-1;
	unsigned long long int free_capacity = 0;
	unsigned long long int tatol_capacity = 0;
	//printf("\n[EMMC-USE]:\n");
	//获取剩余空间


	free_capacity = get_sd_size ("/", SD_FREE_SIZE);
	printf ("\t->emmc_free---------------------------->:%lld MB\n", free_capacity);

	//获取总大小空间
	tatol_capacity = get_sd_size ("/", SD_TOTAL_SIZE);
	printf ("\t->emmc_tatol--------------------------->:%lld MB\n", tatol_capacity);


	if (free_capacity <  1200)
	{
		system(" rm /media/LH_DATA/hdd/tclb1/raw/* -rf ");
		printf ("/t->emmc delete buffer--\n");
		return 1;
	}
	else
	{
		return 0;
	}


}



