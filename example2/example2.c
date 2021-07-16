/*Anonymous shared mapping - related processes*/
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include<string.h>


int main()
{
	const int SIZE=4096;
	void* ptr;
	pid_t ret;//int ret;
	int fd=-1;
	ptr = mmap(0,SIZE, PROT_WRITE|PROT_READ, MAP_SHARED|MAP_ANONYMOUS, fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}
	ret=fork();
	if(ret<0)
	{
		perror("fork");
		exit(1);
	}
	if(ret==0)
	{
		printf("child--welcome..pid=%d,ppid=%d\n",
				getpid(),getppid());
		printf("shm says:%s\n",(char*)ptr);
		printf("shm says:%s\n",(char*)ptr+1024);
		exit(0);
	}
	else		//ret>0
	{
		strcpy(ptr,"Hello shared anonymous\n");
		printf(ptr+1024,"something on 2nd page\n");
		printf("parent--welcome,pid=%d,ppid=%d\n",
				getpid(),getppid());
		printf("parent--ret=%d\n",ret);
	}
	//some code -- common
	return 0;	//exit(0);
}

