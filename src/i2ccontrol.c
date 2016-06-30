#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>

#define _BV(bit) (1 << (bit))

#define U3_IGNIT 1
#define SERVO_1_LEFT_OUT 7
#define SERVO_1_RIGHT_OUT 6
#define SERVO_1_LEFT_IN 5
#define SERVO_1_RIGHT_IN 4

#define SERVO_2_LEFT_OUT 3
#define SERVO_2_RIGHT_OUT 2
#define SERVO_2_LEFT_IN 1
#define SERVO_2_RIGHT_IN 7

#define SERVO_3_LEFT_OUT 6
#define SERVO_3_RIGHT_OUT 5
#define SERVO_3_LEFT_IN 4
#define SERVO_3_RIGHT_IN 3

#define SERVO_4_LEFT_OUT 2
#define SERVO_4_RIGHT_OUT 1
#define SERVO_4_LEFT_IN 7
#define SERVO_4_RIGHT_IN 6

enum pca9557_direction {
	PCA9557_DIR_INPUT,
	PCA9557_DIR_OUTPUT,
};

void pca9557_init(int fd, char addr)
{
	ioctl(fd,I2C_SLAVE,addr);
	// polarity all bits retained
	i2c_smbus_write_byte_data(fd, 0x02, 0x00);
}

void pca9557_set_pin_dir(int fd, char addr, char port, enum pca9557_direction direction)
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
	ioctl(fd,I2C_SLAVE,addr);
	state = i2c_smbus_read_byte_data(fd,0x01);
	if (level)
		state |= _BV(port);
	else
		state &= ~_BV(port);
	i2c_smbus_write_byte_data(fd, 0x01, state);
};

char pca9557_get_pin_level(int fd, char addr, char port)
{
	char state;
	ioctl(fd,I2C_SLAVE,addr);
	state = i2c_smbus_read_byte_data(fd,0x00);
	return state & _BV(port);
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

//	pca9557_init(fd, 0x18);
	pca9557_init(fd, 0x18);
	pca9557_set_pin_dir(fd, 0x18, SERVO_1_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(fd, 0x18, SERVO_1_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(fd, 0x18, SERVO_1_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(fd, 0x18, SERVO_1_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(fd, 0x18, SERVO_2_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(fd, 0x18, SERVO_2_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(fd, 0x18, SERVO_2_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_init(0x19);
	pca9557_set_pin_dir(fd, 0x19, SERVO_2_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(fd, 0x19, SERVO_3_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(fd, 0x19, SERVO_3_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(fd, 0x19, SERVO_3_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(fd, 0x19, SERVO_3_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(fd, 0x19, SERVO_4_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(fd, 0x19, SERVO_4_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_init(0x1a);
	pca9557_set_pin_dir(fd, 0x1a, SERVO_4_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(fd, 0x1a, SERVO_4_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(fd, 0x1a, U3_IGNIT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_level(fd, 0x1a, U3_IGNIT, true);
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
