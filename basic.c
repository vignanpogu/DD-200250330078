#include<linux/init.h>
#include<linux/module.h>

static int my_init(void)
{
	printk(KERN_ALERT"inside my %s module\n",__FUNCTION__);
	return 0;
}

void my_exit(void)
{
	printk(KERN_ALERT"inside my %s module\n",__FUNCTION__);
}
module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LINUX KERNAL MODULE");
MODULE_AUTHOR("POGU VIGNAN");


