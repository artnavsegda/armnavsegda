#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>

#define _BV(bit) (1 << (bit)) 

#define PCA9557_IO0 0x01
#define PCA9557_IO1 0x02
#define PCA9557_IO2 0x04
#define PCA9557_IO3 0x08
#define PCA9557_IO4 0x10
#define PCA9557_IO5 0x20
#define PCA9557_IO6 0x40
#define PCA9557_IO7 0x80

#define PCA9557_OUTPUT_REGISTER 0x01
#define PCA9557_POLARITY_REGISTER 0x02
#define PCA9557_DIRECTION_REGISTER 0x03
#define PCA9557_INPUT_REGISTER 0x00
#define PCA9557_BASE_ADDRESS 0x18
#define PCA9557_DIR_INPUT 0x01
#define PCA9557_DIR_OUTPUT 0x00
#define PCA9557_PIN_LEVEL_LOW 0x00
#define PCA9557_PIN_LEVEL_HIGH 0x01

void pca9557_init(int fd, char addr)
{
	ioctl(fd,I2C_SLAVE,addr);
	// polarity all bits retained
	i2c_smbus_write_byte_data(fd, PCA9557_POLARITY_REGISTER, 0x00);
}

void pca9557_set_pin_dir(int fd, char addr, char port, char direction)
{
	char state;
	ioctl(fd,I2C_SLAVE,addr);
	state = i2c_smbus_read_byte_data(fd,0x03);
	if (direction == PCA9557_DIR_INPUT)
		state |= _BV(port);
	else if (direction == PCA9557_DIR_OUTPUT)
		state &= ~_BV(port);
	i2c_smbus_write_byte_data(fd, 0x03, state);
}

void pca9557_set_pin_level(int fd, char addr, char port, bool level)
{
	char state;
	ioctl(fd,I2C_SLAVE,PCA9557_BASE_ADDRESS+addr);
	state = i2c_smbus_read_byte_data(fd,PCA9557_OUTPUT_REGISTER);
	if (level)
		state |= _BV(port);
	else
		state &= ~_BV(port);
	i2c_smbus_write_byte_data(fd, PCA9557_OUTPUT_REGISTER, state);
};

char pca9557_get_pin_level(int fd, char addr, char port)
{
	char state;
	ioctl(fd,I2C_SLAVE,PCA9557_BASE_ADDRESS+addr);
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

	pca9557_init(fd, 0);
	//write(fd,"\x00\x01\x02",3);
	//write(fd,"ohala",6);
	//write(fd,argv[1],strlen(argv[1]));
	//i2c_smbus_write_byte(fd,0x66);
	//printf("%d\n",i2c_smbus_read_byte(fd));
	//i2c_smbus_write_byte_data(fd,0x03,0x3F);
	//sleep(1);
	//i2c_smbus_write_byte_data(fd,0x01,0x40);
	//sleep(1);
	//i2c_smbus_write_byte_data(fd,0x01,0x80);
	close(fd);
	return 0;
}
