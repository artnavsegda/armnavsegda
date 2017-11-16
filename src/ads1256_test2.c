#include <bcm2835.h>
#include <stdio.h>

int main(void)
{
	if (!bcm2835_init())
		return 1;
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST );
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);
	bcm2835_gpio_fsel(RPI_GPIO_P1_24, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(RPI_GPIO_P1_24, HIGH);
	bcm2835_gpio_fsel(RPI_GPIO_P1_11, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(RPI_GPIO_P1_11, BCM2835_GPIO_PUD_UP);
	while(bcm2835_gpio_lev(RPI_GPIO_P1_11));
	bcm2835_gpio_write(RPI_GPIO_P1_24,LOW);
	bcm2835_delayMicroseconds(2);
	bcm2835_spi_transfer(0x10|0);
	bcm2835_delayMicroseconds(2);
	bcm2835_spi_transfer(0x00);
	bcm2835_delayMicroseconds(10);
	uint8_t _data = bcm2835_spi_transfer(0xff);
	bcm2835_gpio_write(RPI_GPIO_P1_24, HIGH);
	printf("chip id %d\n",_data >> 4);
	bcm2835_spi_end();
	bcm2835_close();
}

