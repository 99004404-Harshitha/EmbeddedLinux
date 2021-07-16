#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>

#define IOC_MAGIC 'p'
#define MY_IOCTL_LEN     _IO(IOC_MAGIC, 1)
#define MY_IOCTL_AVAIL   _IO(IOC_MAGIC, 2)
#define MY_IOCTL_RESET   _IO(IOC_MAGIC, 3)

int8_t write_buf[1024];
int8_t read_buf[1024];

int main()
{
int fd;
int ret,option;

printf("Welcome to the sudo device driver...\n");

fd=open("/dev/psample0",O_RDWR);
if(fd<0){
perror("open");
exit(1);
 }
 ret=ioctl(fd, MY_IOCTL_LEN);
 if(ret<0){
 perror("ioctl");
 exit(3);
 }
 ret=ioctl(fd, MY_IOCTL_AVAIL);
 if(ret<0){
 perror("ioctl");
 exit(3);
 }
 ret=ioctl(fd, MY_IOCTL_RESET);
 if(ret<0){
 perror("ioctl");
 exit(3);
 }
 if(fd<0){
printf("Cannot open device file..../n");
return 0;
}
 while(1)
 {
 printf("*****please enter your option*****\n");
 printf("			1.Write			\n");
 printf("			2.Read				\n");
 printf("			3.Exit				\n"); 
 fflush(stdin);
 scanf("%d",&option);
 printf("your options = %d\n",option);
 
 switch(option){
 case 1:
 printf("Enter the string to write into the driver:\n");
 scanf("%[^\t\n]s",write_buf);
 printf("Data written");
 write(fd,write_buf,strlen(write_buf)+1);
 printf("Done...\n");
 break;
 case 2:
 printf("Data is Reading...");
 read(fd,read_buf,1024);
 printf("Done...\n\n");
 printf("Data=%s\n\n",read_buf);
 break;
 case 3:
 close(fd);
 exit(1);
 break;
 default:
 printf("Enter valid option = %d\n",option);
 break;
 }
 }
 
 close(fd);
}
