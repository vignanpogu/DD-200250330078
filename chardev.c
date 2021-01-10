#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/kernel.h>
#include<linux/uaccess.h>
#include<linux/types.h>


/***************************** Prototypes for file operation fuctions ************/
int my_open(struct inode *inodep,struct file *filep);
ssize_t my_read(struct file *filp, char __user *Ubuff, size_t count, loff_t  *offp);
ssize_t my_write(struct file *filep, const char __user *Ubuff, size_t count, loff_t  *offp);
int my_release(struct inode *indoe, struct file *filp);

/***************************** Operations of driver (file_operations structure) ******************************/
struct file_operations fops={
	.open= my_open,
	.read= my_read,
	.write= my_write,
	//.release= my_release,
};
/***************************** OPEN METHOD **************************************/
int my_open(struct inode *inodep,struct file *filep) {


	printk("opening file is done inkernel space....do some operations\n");
return 0;
}
/****************************** READ METHOD **************************************/
ssize_t my_read(struct file *filp, char __user *Ubuff, size_t count, loff_t  *offp) {



	printk("good you performed read operations in kernel space........\n");
	
return 0;
}
/****************************** WRITE METHOD ************************************/
ssize_t my_write(struct file *filep, const char __user *Ubuff, size_t count, loff_t  *offp) {





	printk("nice you performed write operation in kernel space .............\n");
return 0;
}
/***************************** RELEASE METHOD **************************************/
int my_release(struct inode *indoe, struct file *filp){


	printk("bye you are quiting from kernel module..........\n");
	return 0;
}
/******************************* CDEV structure *************************************/
struct cdev *my_cdev;
/******************************** module initialization ******************************/
static int __init chardev_init(void){
	
	dev_t devno;
	devno=MKDEV(255,1);		//device number 
	int MAJOR=MAJOR(devno); //macro to find major number
	int MINOR=MINOR(devno);//macro to find minor number
	printk("the MAHJOR number %d........and MINOR number %d..........\n",MAJOR,MINOR);
	int ret = register_chrdev_region(devno,4,"cdev_test");
		if(ret){
			printk("error: registering device number\n");
			return ret;
			}
	my_cdev = cdev_alloc(); //memory allocation to cdev structure
	my_cdev->ops = &fops;	// link our file operations to cdev structure

	ret = cdev_add(my_cdev,devno,4);  //notify the kernel about cdev and device
		if(ret<0) {
			printk("the char device has not been created......................\n");
			return ret;
			}

	printk(" chardev module initalization is completed\n nice to see you..........\n");
return 0;
}
/******************************* module exit ***************************************/
static void __exit chardev_exit(void) {
	dev_t devno = MKDEV(255,1);
	int MAJOR = MAJOR(devno);
	int MINOR = MINOR(devno);
	printk("the MAHJOR number %d........and MINOR number %d..........\n",MAJOR,MINOR);
	cdev_del(my_cdev);
	unregister_chrdev_region(devno,4);



printk("what ever you created is destroyed.......................\n");
printk("chardev module exit is complete\n bye.....hope i will see you soon...\n");
}
/******************************* module init & exit macros ******************************/
module_init(chardev_init);
module_exit(chardev_exit);
