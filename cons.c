#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include"ioctlh.h"
#define SIZE 8
int main()
{
	int fd,ret,k=10,l;
	//char buf[SIZE]="cdaccc";
	char kbuf[SIZE];
	//cbuf[]="";
	fd=open("/dev/circular",O_RDWR);
	if( fd < 0 )
	{
		perror("error in reading from device\n");
		return fd;
	}
	ioctl(fd,CMD_1);
	ioctl(fd,CMD_GET,&k);
        ioctl(fd,CMD_DUMMY,&l);
	//read(fd,kbuf,strlen(kbuf));
	printf("\n l = %d \n",l);
	return 0;
}
	
