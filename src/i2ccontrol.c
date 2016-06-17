#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>

#define PCA9557_DIRECTION_REGISTER 0x03
#define PCA9557_OUTPUT_REGISTER 0x01
#define PCA9557_INPUT_REGISTER 0x00
#define PCA9557_BASE_ADDRESS 0x18
#define PCA9557_DIR_INPUT 0x01
#define PCA9557_DIR_OUTPUT 0x00
#define PCA9557_PIN_LEVEL_LOW 0x00
#define PCA9557_PIN_LEVEL_HIGH 0x01

void pca9557_init(uint8_t addr)
{
	// output all bits level low
	i2c_send(&TWIE, PCA9557_BASE_ADDRESS+addr, 0x01, 0x00);
	// polarity all bits retained
	i2c_send(&TWIE, PCA9557_BASE_ADDRESS+addr, 0x02, 0x00);
	// direction all bits input
	i2c_send(&TWIE, PCA9557_BASE_ADDRESS+addr, PCA9557_DIRECTION_REGISTER, 0xFF);
}

void pca9557_set_pin_dir(uint8_t addr, uint8_t port, uint8_t direction)
{
	uint8_t state;
	state = i2c_read(&TWIE,PCA9557_BASE_ADDRESS+addr,PCA9557_DIRECTION_REGISTER);
	if (direction == PCA9557_DIR_INPUT)
		state = state | port;
	else if (direction == PCA9557_DIR_OUTPUT)
		state = state & port;
	i2c_send(&TWIE, PCA9557_BASE_ADDRESS+addr, PCA9557_DIRECTION_REGISTER, state);
}

void pca9557_set_pin_level(uint8_t addr, uint8_t port, uint8_t level)
{
	uint8_t state;
	state = i2c_read(&TWIE,PCA9557_BASE_ADDRESS+addr,PCA9557_OUTPUT_REGISTER);
	if (level == PCA9557_PIN_LEVEL_LOW)
		state = state | port;
	else if (level == PCA9557_PIN_LEVEL_HIGH)
		state = state & port;
	i2c_send(&TWIE, PCA9557_BASE_ADDRESS+addr, PCA9557_OUTPUT_REGISTER, state);
};

uint8_t pca9557_get_pin_level(int fd, uint8_t addr, uint8_t port)
{
	uint8_t state;
	ioctl(fd,I2C_SLAVE,PCA9557_BASE_ADDRESS+addr)
	state = i2c_smbus_read_byte_data(fd,PCA9557_INPUT_REGISTER);
	return state | ~port;
}

int main(int argc, char *argv[])
{
	int fd;
	fd = open("/dev/i2c-1",O_RDWR);
	if (fd<=1)
	{
		printf("error on device\n");
		exit(1);
	}
	int addr = 0x18;
	if (ioctl(fd,I2C_SLAVE,addr) < 0)
	{
		printf("ioctl error\n");
		exit(1);
	}
	//write(fd,"\x00\x01\x02",3);
	//write(fd,"ohala",6);
	//write(fd,argv[1],strlen(argv[1]));
	//i2c_smbus_write_byte(fd,0x66);
	//printf("%d\n",i2c_smbus_read_byte(fd));
	i2c_smbus_write_byte_data(fd,0x03,0x3F);
	sleep(1);
	i2c_smbus_write_byte_data(fd,0x01,0x40);
	sleep(1);
	i2c_smbus_write_byte_data(fd,0x01,0x80);
	close(fd);
	return 0;
}
