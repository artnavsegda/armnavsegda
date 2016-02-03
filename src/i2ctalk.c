#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int main()
{
	int fd;
	fd = open("/dev/i2c-1",O_RDWR);
	if (fd<=1)
	{
		printf("error on device\n");
		exit(1);
	}
	int addr = 0x04;
	if (ioctl(fd,I2C_SLAVE,addr) < 0)
	{
		printf("ioctl error\n");
		exit(1);
	}
	//write(fd,"\x00\x01\x02",3);
	write(fd,"hello",6);
	close(fd);
	return 0;
}
