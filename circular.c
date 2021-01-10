#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/circ_buf.h>
#include<linux/slab.h>
#include<linux/cdev.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/wait.h>
#include<linux/sched.h>
#include<linux/ioctl.h>
#include "ioctlh.h"

#define NAME jdev
#define SIZE 16

int NAME_open(struct inode *inode, struct file *file);
int NAME_release(struct inode *inode,struct file *file);
ssize_t NAME_write(struct file *file,const char __user *buff,size_t count,loff_t *offp);
ssize_t NAME_read(struct file *file,char __user *buff,size_t count,loff_t *offp);
long test_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);
wait_queue_head_t twq;//wait queue

struct file_operations fops={
	.open=NAME_open,
	.read=NAME_read,
	.write=NAME_write,
	.release=NAME_release,
	.unlocked_ioctl = test_ioctl,
	};
int k,l=120;	
struct cdev my_cdev;
struct circ_buf cbuf;
dev_t mydev;

long test_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	
	switch(cmd)
	{
		case CMD_1:
			printk("CMD_1 invoked\n");
			break;
		case CMD_GET:
			get_user(k,(int*)arg);
			printk("Got %d",k);
		case CMD_DUMMY:
			put_user(l,(int*)arg);
			break;
		default:
			printk("not valid\n");
			return -ENOTTY;
	}	
}

static int circular_init(void)
{
	
	int MAJOR,MINOR,ret;
	mydev=MKDEV(45,0);
	if(mydev == 0)
	{
		printk("not allocated\n");
	}

	MAJOR=MAJOR(mydev);
	MINOR=MINOR(mydev);
	printk("major = %d and minor = %d numbers are allocated to devices\n",MAJOR,MINOR);
	
	ret=register_chrdev_region(mydev,1,"jdev");
	if(ret)
	{
	printk("error in registering device to kernel\n");
	return ret;
	}
	
	/*ret=alloc_chardev_region(mydev,1,"jdev");
	if(ret)
	{
	printk("error in allocating\n");
	return ret;
	}*/
	/*my_cdev = cdev_alloc();
	my_cdev->ops = &fops;*/

	cdev_init(&my_cdev,&fops);
	
	cbuf.buf=kmalloc(SIZE,GFP_KERNEL);
	if(!cbuf.buf)
	{
	printk("allocation failed\n");
	return -ENOMEM;
	}

	init_waitqueue_head(&twq);

	ret = cdev_add(&my_cdev,mydev,1);
	if(ret)
	{
		printk(KERN_ALERT "\nFailed to Add Char Device\n");
		return ret;
	}
	return 0;
}
static void  circular_exit(void) 
{
//	dev_t mydev; 
	cdev_del(&my_cdev);
	kfree(cbuf.buf);
	unregister_chrdev_region(mydev,1);
	printk(KERN_ALERT "\nUnregistered successfully\n");
}


int NAME_open(struct inode *inode, struct file *file) {
	printk(KERN_ALERT"\n KERNEL OPEN\n");
	return 0;
}


int NAME_release(struct inode *inode,struct file *file) {
	printk(KERN_ALERT"\n RELEASE\n");
	return 0;
}

ssize_t NAME_write(struct file *file,const char __user *ubuff,size_t count,loff_t *offp)
{
	//char cbuff[1024];
	unsigned long ret;
	int m,i;
	m=min(CIRC_SPACE(cbuf.head,cbuf.tail,SIZE),(int)count);
	printk("m = %d\n",m);
	printk("m in read = %d",m);
	for(i=0;i<m;i++)
	{		   //to kernel       from user
	ret = copy_from_user(cbuf.buf+cbuf.head,ubuff+i,1); //(destination,source)
	if(ret)
	{
		printk("not copied\n");
		return -ENOMEM;
	}
	printk("copied from user(write) %c %d\n",cbuf.buf[cbuf.head],m);
	cbuf.head=(cbuf.head+1)&(SIZE-1);
	}
	
	wake_up(&twq);	
	return i;
}

ssize_t NAME_read(struct file *filep, char __user *ubuff,size_t count,loff_t *offp){
	
	unsigned long ret;
	int m,i;
	
	
	if ((filep->f_flags) & (O_NONBLOCK))
	{
		if(CIRC_CNT(cbuf.head,cbuf.tail,SIZE)==0)
		{
			return -EAGAIN;
		}
		else
			goto next;
	}

	wait_event_interruptible( twq, CIRC_CNT(cbuf.head,cbuf.tail,SIZE) >= 1 );
next:	m=min(CIRC_CNT(cbuf.head,cbuf.tail,SIZE),(int)count);
	printk("m in read = %d",m);

	for(i=0;i<m;i++)
	{
			// to user,    from kernel  ,size	
	ret = copy_to_user(ubuff+i,cbuf.buf+cbuf.tail,1);
	if(ret)
	{
		printk("not copied\n");
		return -ENOMEM;
	}
	printk("copied(read) from user %c %d \n",ubuff[i],m);
	cbuf.tail=(cbuf.tail+1)&(SIZE-1);
	}	
	return i;
}
	
module_init(circular_init);
module_exit(circular_exit);
		
