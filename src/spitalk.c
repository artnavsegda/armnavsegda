#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdbool.h>

#include <bcm2835.h>
#include <tm1638.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <endian.h>

main()
{
	tm1638_p t;
	int fd;
	char buf[4];
	int num2;

	if (!bcm2835_init())
	{
		printf("Unable to initialize BCM library\n");
		return -1;
	}

	t = tm1638_alloc(17, 21, 22);
	if (!t)
	{
		printf("Unable to allocate TM1638\n");
		return -2;
	}
	tm1638_set_7seg_text(t, "start", 0x00);

	fd = open("/dev/spidev0.0",O_RDWR);
	if (fd<=1)
	{
		printf("error on device\n");
		exit(1);
	}
	while (1)
	{
		buf[0] = 0x08;
		write(fd,buf,1);
		read(fd,buf,1);
		//printf("read %x\n",buf[0]);
		if (buf[0] == 8)
		{
			buf[0] = 0x38;
			num2 = 0;
			write(fd,buf,1);
			read(fd,buf,2);
			swab(&buf,&num2,2);
			printf("read %d\n",num2-0x8000);
		}
	}
	return 0;
}
