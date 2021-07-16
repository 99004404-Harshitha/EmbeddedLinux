#include<linux/fs.h>              
#include <linux/module.h>
#include <linux/cdev.h>
#include<linux/device.h>
#include<linux/kfifo.h>
#include<linux/slab.h>
#include <linux/ioctl.h>


#define MAX_BUF_SIZE 1024
#define MAX_SIZE 1024
#define IOC_MAGIC 'p'
#define MY_IOCTL_LEN     _IO(IOC_MAGIC, 1)
#define MY_IOCTL_AVAIL   _IO(IOC_MAGIC, 2)
#define MY_IOCTL_RESET   _IO(IOC_MAGIC, 3)
#define MY_IOCTL_PSTAT	  _IO(IOC_MAGIC, 4) 

struct cdev cdev; 	//global
struct device *pdev;	//device creation
struct class *pclass;	//class creation
struct kfifo k1;	//kfifo

struct pseudo_stat{
	int len;
	int avail;
};

int fd;
int ndevices=1;	
dev_t pdevid;
char *pbuffer;
struct pseudo_stat stat;
//prototypes
int pseudo_open(struct inode* inode, struct file* file);
int pseudo_close(struct inode* inode, struct file* file);
ssize_t pseudo_read(struct file * file, char __user * buf, size_t size, loff_t * off);
ssize_t pseudo_write(struct file * file, const char __user * buf, size_t size, loff_t * off);
ssize_t pseudo_ioctl(struct file * file, unsigned int cmd, unsigned long arg);



struct file_operations fops ={
.open      = 	 pseudo_open,
.release   = 	 pseudo_close,
.write     = 	 pseudo_write,
.read      = 	 pseudo_read,
.unlocked_ioctl=pseudo_ioctl
};


static int __init psuedo_init(void)
{
int ret,i=0;

pclass=class_create(THIS_MODULE,"pseudo_class");

kfifo_alloc(&k1, MAX_SIZE, GFP_KERNEL);
//Dynamically allocate device number
alloc_chrdev_region(&pdevid,0,ndevices,"pseudo_sample");

//Intialize cdev structure
cdev_init(&cdev,&fops); 
kobject_set_name(&cdev.kobj, "pdevice%d", 1);

//Register device in drivers
ret = cdev_add(&cdev, pdevid, 1);


pdev = device_create(pclass,NULL,pdevid,NULL,"psample%d",i);
if(ret)
{
printk("Pseudo: Failed to register driver\n");
return -EINVAL;
}
printk("Successfullt registered, major-%d,minor=%d\n",MAJOR(pdevid),MINOR(pdevid));
printk("Pseudo Driver Sample..welcome\n");


return 0;
}
static void __exit pseudo_exit(void) {
cdev_del(&cdev);
kfifo_free(&k1);
device_destroy(pclass,pdevid);
unregister_chrdev_region(pdevid, ndevices);
class_destroy(pclass);

printk("Pseudo Driver Sample..Bye\n");

}


int pseudo_open(struct inode* inode, struct file* file)

{ 
  printk("Pseudo--open method\n"); 
  return 0;
} 
int pseudo_close(struct inode* inode, struct file* file)
 { 
 printk("Pseudo--release method\n");
 return 0;
} 
ssize_t pseudo_read(struct file * file, char __user * ubuf, size_t size, loff_t * off)
{
 int ret,rcount = size;
if(kfifo_is_empty(&k1))
 return 0;
if(rcount > kfifo_len(&k1))
rcount =kfifo_len(&k1);
char *tbuf=kmalloc(rcount,GFP_KERNEL);
kfifo_out(&k1, tbuf, rcount);
ret=copy_to_user(ubuf, tbuf, rcount);
if(ret) {
     printk("Erorr inn copy_to_user,failed");
     return -EINVAL;
   }
   
      
   printk("Pseudo--read method, transferred %d bytes\n",rcount);
kfree(tbuf);
return rcount;
}
ssize_t pseudo_write(struct file * file, const char __user * ubuf, size_t size, loff_t * off) 
{
int ret,wcount = size;
if(kfifo_is_full(&k1))
 return -ENOSPC;
if(wcount > kfifo_avail(&k1))
wcount =kfifo_avail(&k1);
char *tbuf=kmalloc(wcount,GFP_KERNEL);
ret=copy_from_user(tbuf, ubuf, wcount);
if(ret) {
     printk("Erorr inn copy_from_user");
     return -EINVAL;
   }   

   
   printk("Pseudo--write method, transferred %d bytes\n",wcount);
kfifo_in(&k1, tbuf, wcount);
kfree(tbuf);
return wcount;
}

ssize_t pseudo_ioctl(struct file * file, unsigned int cmd, unsigned long arg)
{
 printk("pseudo--ioctl method\n");
 int ret;
 switch (cmd) {
 case MY_IOCTL_LEN :
 	printk("IOCTL--KFIFO LENGTH IS %d\n",kfifo_len(&k1));
 	break;
 case MY_IOCTL_AVAIL :
 	printk("ioctl--kfifo avail is %d\n",kfifo_avail(&k1));
 	break;
 case MY_IOCTL_RESET:
 	printk("ioctl--kfifo got reset\n");
 	kfifo_reset(&k1);
 break;
 case MY_IOCTL_PSTAT:
 	printk("ioctl--kfifo statistics\n");
 	stat.len=kfifo_len(&k1);
 	stat.avail=kfifo_avail(&k1);
 	ret=copy_to_user((char __user*)arg, &stat, sizeof(stat));
 	if(ret) {
 		printk("error in copy_to_user\n");
 		return -EFAULT;
 	}
 	break;
 }
 return 0;
}

module_init(psuedo_init);
module_exit(pseudo_exit);
MODULE_LICENSE ("GPL");
MODULE_AUTHOR("HARSHITHA");
MODULE_DESCRIPTION("first Step");
