/* Anonymous mapping */
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>

void handler_int(int signo) {
  printf("SIGINT arrived\n");
}

int main()
{
	printf("welcome---pid=%d\n",getpid());
	const int MAPSIZE=64*sysconf(_SC_PAGE_SIZE);
    signal(SIGINT, handler_int);
	void* pbase;
	pid_t ret;//int ret;
	int i,fd,val;
    fd=-1;
	//fd=open("test.img",O_RDWR);
	printf("going to map some memory\n");
	pause();	//pause/sigsuspend/sleep
	pbase = mmap(0,MAPSIZE, PROT_WRITE|PROT_READ,MAP_ANONYMOUS|MAP_SHARED, fd, 0);
	if (pbase == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}
	printf("going to access pages\n");
	pause();
	char *ptr=pbase;
	for(i=0;i<64;i++)
	{
		*ptr=rand()%100;val=*ptr;
		//*ptr=rand()%100;val=*ptr;
		//ptr+=4096;	
		sleep(1);	//500ms
		//msync(ptr,4096,MS_SYNC);
		ptr+=4096;
	}
	printf("going to unmap memory\n");
	pause();
	munmap(pbase,MAPSIZE);
	printf("going to exit\n");
	pause();
	return 0;	//exit(0);
}

