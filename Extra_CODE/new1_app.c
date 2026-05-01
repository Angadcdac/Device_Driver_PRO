#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define DEV "/dev/dev_pradeep"
int main()
{
	int val=0;
	//char *ptr = "sudo insmod new1.ko";
	//char *ptr1 = "sudo rmmod new1.ko";
	//system(ptr);
	char *buff="HELLO WORLD FROM USER SPACE !\n";
	int fd=open(DEV,O_RDWR,0777);
	if(fd < 0){
		perror("DEVICE COULD NOT OPEN\n");
		return EXIT_FAILURE;
	}

	int nwrite=write(fd,buff,strlen(buff));
	if(nwrite < 0){
		perror("COULD NOT WRITE\n");
		return EXIT_FAILURE;
	}

	while(val != 1){
		int nread=read(fd,&val,sizeof(int));
		if(nread < 0){
			perror("COULD NOT READ\n");
			return EXIT_FAILURE;
		}
		printf("%d\n",val);
		sleep(1);
	}
	printf("THE VALUE READ FROM KERNEL IS %d \n",val);
	close(fd);
	//system(ptr1);
	return 0;
}

