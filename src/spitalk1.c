#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

uint8_t mode = SPI_CPHA | SPI_CPOL;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay = 10;

main()
{
	uint8_t tx1[1];
	uint8_t rx1[1] = {0};
	int fd;
	char buf[4];
	struct spi_ioc_transfer tr;
	memset(&tr,0,sizeof(tr));
	fd = open("/dev/spidev0.0",O_RDWR);
	if (fd<=1)
	{
		printf("error on device\n");
		exit(1);
	}
	ioctl(fd, SPI_IOC_WR_MODE, &mode);
	ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	tr.tx_buf = (unsigned long)tx1;
	tr.rx_buf = (unsigned long)rx1;
	tr.len = ARRAY_SIZE(tx1);
	tr.delay_usecs = delay;
	tr.speed_hz = speed;
	tr.bits_per_word = bits;
	tx1[0] = 0x08;
	//ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	buf[0] = 0x08;
	write(fd,buf,1);
	int n = read(fd,buf,1);
	printf("read %d %d\n",n,buf[0]);
	return 0;
}
