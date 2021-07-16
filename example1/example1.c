#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int filesz=4096;
	int fd,offset=0;
	fd=open("sample.txt", O_RDONLY);
	if(fd<0)
	{
		perror("open"); 
		exit(1);
	}

	void *pbase;
	pbase=mmap(0, filesz, PROT_READ, MAP_PRIVATE, fd,offset); 
	if(pbase==MAP_FAILED)
	{
		fprintf(stderr, "mapping failed\n");
		//close(fd);
		exit(1);
	}
	char buf[64]={};
	strncpy(buf,pbase, 10); //for loop 
	printf("buf=%s\n",buf);
	bzero(buf,20);
	strcpy(buf,pbase+10);
	printf("buf=%s\n", buf);
	munmap(pbase, filesz);
	close(fd);
	return 0;
}



