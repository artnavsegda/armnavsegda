#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int fd;

uint8_t transfer(uint8_t bytetosend)
{
	uint8_t bytestorecieve[1];
	uint8_t bytestosend[1];
	bytestosend[0] = bytetosend;
	struct spi_ioc_transfer package = {
		.tx_buf = (unsigned long)bytestosend,
		.rx_buf = (unsigned long)bytestorecieve,
		.len = 1,
		.delay_usecs = 0,
		.speed_hz = 244000,
		.bits_per_word = 8,
		.cs_change = 1 // maybe one ?
	};
	int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &package);
	if (ret < 1)
	{
		perror("can't send spi message");
		abort();
	}
	return bytestorecieve[0];
}

void startspi(void)
{
	fd = open("/dev/spidev0.0",O_RDWR);
	if (fd<=1)
	{
		perror("error on device");
		abort();
	}
	uint8_t mode = SPI_MODE_1 | SPI_CS_HIGH; // CS_HIGH IS PROBABLY WRONG BUT ANYWAY
	int ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
	{
		perror("can't set spi mode");
		abort();
	}
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
	{
		perror("can't get spi mode");
		abort();
	}
	printf("spi mode %d\n", mode);
}


int main(void)
{
	startspi();
	//if (!bcm2835_init())
	//	return 1;
	//bcm2835_spi_begin();
	//bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST );
	//bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);
	//bcm2835_gpio_fsel(RPI_GPIO_P1_24, BCM2835_GPIO_FSEL_OUTP);
	//bcm2835_gpio_write(RPI_GPIO_P1_24, HIGH);
	//bcm2835_gpio_fsel(RPI_GPIO_P1_11, BCM2835_GPIO_FSEL_INPT);
	//bcm2835_gpio_set_pud(RPI_GPIO_P1_11, BCM2835_GPIO_PUD_UP);
	//while(bcm2835_gpio_lev(RPI_GPIO_P1_11));
	//bcm2835_gpio_write(RPI_GPIO_P1_24,LOW);
	usleep(2);
	transfer(0x10|0);
	usleep(2);
	transfer(0x00);
	usleep(10);
	uint8_t _data = transfer(0xff);
	//bcm2835_gpio_write(RPI_GPIO_P1_24, HIGH);
	//printf("chip id %d\n",_data >> 4);
	printf("chip id %d\n",_data);
	//bcm2835_spi_end();
	//bcm2835_close();
}

