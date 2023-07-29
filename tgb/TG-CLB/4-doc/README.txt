设备文件说明：
1、SD卡存储路径--> /media/comm
2、配置文件放置路径--> /media/local_config/config
3、应用程序放置路径--> /media/local_config/app
4、UART2设备--> /dev/ttyS2
5、UART5设备--> /dev/ttyS5
6、LRUN--> /sys/class/gpio/gpio27/value
7、LLAN--> /sys/class/gpio/gpio47/value
8、LCAN--> /sys/class/gpio/gpio46/value
9、LLAN--> /sys/class/gpio/gpio45/value
10、LMVB--> /sys/class/gpio/gpio44/value
11、LUSER1--> /sys/class/gpio/gpio26/value
12、LUSER2--> /sys/class/gpio/gpio61/value
13、MVB_RESET--> /sys/class/gpio/gpio5/value
14、TRDP_RESET-->  /sys/class/gpio/gpio4/value
15、生成应用程序名称：JXDS_COMM


CAN设备测试命令：
1、设置CAN的波特率： ip link set can0 type can bitrate 50000
2、设置CAN本地回环模式（测试自发自收）:ip link set can0 type can bitrate 50000 loopback on
3、使能CAN设备：ip link set can0 up　　或者　ifconfig cano up
4、一次发送8个字节:cansend can0 0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88　