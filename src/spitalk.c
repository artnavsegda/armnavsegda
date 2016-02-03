#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdbool.h>

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
	write(fd,"\x20",1);
	write(fd,"\x0C",1);
	write(fd,"\x10",1);
	write(fd,"\x04",1);
	write(fd,"\x60",1);
	write(fd,"\x18\x3a\x00",3);
	write(fd,"\x70",1);
	write(fd,"\x89\x78\xd7",3);
	while (1)
	{
		write(fd,"\x08",1);
		read(fd,buf,1);
		if (buf[0] == 8)
		{
			num2 = 0;
			write(fd,"\x38",1);
			read(fd,buf,2);
			swab(&buf,&num2,2);
			printf("%ld\n",(long)num2-0x17CC);
		}
	}
	close(fd);
	return 0;
}
