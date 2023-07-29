#include <stdio.h>
#include <string.h>
#include <stdint.h>

uint16_t sum_check_16(uint8_t *data, uint16_t crclen)
{
	uint16_t check_sum = 0;
	for(uint16_t i = 0; i < crclen; i++)
	{
		check_sum += data[i];
	}
	return check_sum;
}

typedef struct
{
	uint8_t device_code;
	uint8_t sn[2];
	uint8_t local_ip[4];
	uint8_t mask[4];
	uint8_t gateway[4];
	uint8_t port[2];
	uint8_t train_num;
	uint8_t reserve[4];
}broadcast_ack_cmd_t;

int main()
{
	printf( "size = %ld \n", sizeof(broadcast_ack_cmd_t) );

    uint8_t recv[39] = { 0xAA, 0x51, 0x00, 0x27, 0x04, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3F, 0x33, 0x1D, 0x03, 0xC0, 0xA8, 0x03, 0x0D, 0xFF, 0xFF, 0x00, 0x00, 0xC0, 0xA8, 0x01, 0x01, 0x1F, 0x40, 0x03, 0x00, 0x00, 0x00, 0x00, 0x5E, 0x2D, 0x08, 0x87 };  

    uint8_t *p = recv;
 
    uint16_t sum = sum_check_16(p, 37);

    printf("sum = %d \n", sum);

    return 0;
}