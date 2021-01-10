#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int mani(){

	int fd =open("/dev/chardev_test",O_RDWR);
		if(-1==fd){
		perror("error: opening device\n");
		return -1;
		}
	write(fd,NULL,0);
	read(fd,NULL,0);
	printf("application is done don't waste time....................\ngo and check in kernel space............\nall the best..............\n");
return 0;
}

