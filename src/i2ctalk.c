#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int fd;
	fd = open("/dev/i2c-1",O_RDWR);
	if (fd<=1)
	{
		printf("error on device\n");
		exit(1);
	}
	int addr = 0x09;
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
	//i2c_smbus_write_byte_data(fd,0x66,0x77);
	printf("%d\n",i2c_smbus_read_byte_data(fd,0x00));
	printf("%d\n",i2c_smbus_read_byte_data(fd,0x01));
	printf("%d\n",i2c_smbus_read_byte_data(fd,0x02));
	printf("%d\n",i2c_smbus_read_byte_data(fd,0x03));
	printf("%d\n",i2c_smbus_read_byte_data(fd,0x04));
	printf("%d\n",i2c_smbus_read_byte_data(fd,0x05));
	close(fd);
	return 0;
}
