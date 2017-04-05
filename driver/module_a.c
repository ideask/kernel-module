#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#define MAX_LENGTH 64
static int major = 232;
static int minor = 0;
module_param(major,int,S_IRUGO);
module_param(minor,int,S_IRUGO);

static int module_a_open(struct inode *inode,struct file *file);
static ssize_t module_a_read(struct file *filp,char *buf,size_t count, loff_t * f_pos);
static ssize_t module_a_write (struct file *filp,const char *buf,size_t count,loff_t *ppos);
static int module_a_release(struct inode *inode,struct file *file);
static char module_a_buffer[MAX_LENGTH]={0};

struct cdev *module_a;
static dev_t devno;
static struct class *module_a_class;

#define DEVICE_NAME "module_a"
struct file_operations module_a_fops ={
	.owner = THIS_MODULE,
	.open = module_a_open,
	.release = module_a_release,
	.read = module_a_read,
	.write = module_a_write
};

static int module_a_open(struct inode *inode,struct file *file)
{
	try_module_get(THIS_MODULE);
	return 0;
}
static int module_a_release(struct inode *inode,struct file *file)
{
	module_put(THIS_MODULE);
	return 0;
}


static ssize_t module_a_read(struct file *filp,char *buf,size_t count, loff_t * f_pos)
{
	if(count > MAX_LENGTH)
	{
		printk("Max length is %d",MAX_LENGTH);
		count = MAX_LENGTH;
	}
	if(copy_to_user((void *)buf, module_a_buffer, count))
	{
		printk("copy_to_user error \n");
		return -EFAULT;
	}
	return count;

}
static ssize_t module_a_write (struct file *filp,const char *buf,size_t count,loff_t *f_pos)
{
	if(count > MAX_LENGTH)
	{
		printk("Max length is %d",MAX_LENGTH);
		count = MAX_LENGTH;
	}
	if(copy_from_user(&module_a_buffer, buf, count))
	{
		printk("copy_from_user error \n");
		return -EFAULT;
	}
	return count;
}



static int __init module_a_udev_init(void)
{
	int ret;
	if(major > 0)
	{
		devno = MKDEV(major,minor);
		ret = register_chrdev_region(devno,1,DEVICE_NAME);
	}
	else
	{
		ret = alloc_chrdev_region(&devno,minor,1,DEVICE_NAME);
		major = MAJOR(devno);
	}
	if(ret < 0)
	{
		printk(KERN_ERR "cannot get major %d\n",major);
		return -1;
	}
	module_a = cdev_alloc();
	if(module_a != NULL)
	{
		cdev_init(module_a,&module_a_fops);
		module_a->owner = THIS_MODULE;
		if(cdev_add(module_a,devno,1)!= 0)
		{
			printk(KERN_ERR"add module_a cdev error!\n");
			goto error;
		}
	}
	else
	{
		printk(KERN_ERR"module_a alloc error!\n");
		return -1;
	}
	
	module_a_class = class_create(THIS_MODULE,"module_a_class");
	if(IS_ERR(module_a_class))
	{
		printk(KERN_INFO"create module_a_class error \n");
		return -1;
	}
	device_create(module_a_class,NULL,devno,NULL,DEVICE_NAME);
	return 0;
error:
	unregister_chrdev_region(devno,1);
	return ret;
}

static void __exit module_a_udev_exit(void)
{
	device_destroy(module_a_class,devno);
	class_destroy(module_a_class);
	cdev_del(module_a);
	unregister_chrdev_region(devno,1);
}
module_init(module_a_udev_init);
module_exit(module_a_udev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ideask,ideask@ideask.cf");

