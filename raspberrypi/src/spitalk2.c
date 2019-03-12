#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <endian.h>

main()
{
	int fd;
	char buf[4];
	int num2;
	fd = open("/dev/spidev0.0",O_RDWR);
	if (fd<=1)
	{
		printf("error on device\n");
		exit(1);
	}
	buf[0] = 0x38;
	write(fd,buf,1);
	int n = read(fd,buf,2);
	swab(&buf,&num2,2);
	printf("read %d\n",num2-0x8000);
	return 0;
}
