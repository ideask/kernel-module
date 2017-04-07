#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
static int major = 232;
static int minor = 0;
module_param(major,int,S_IRUGO);
module_param(minor,int,S_IRUGO);

static int module_a_open(struct inode *inode,struct file *file);
static ssize_t module_a_write (struct file *filp,const char *buf,size_t count,loff_t *ppos);
static int module_a_release(struct inode *inode,struct file *file);

struct cdev *module_a;
static dev_t devno;
static struct class *module_a_class;
spinlock_t spinlock;

struct module_select
{
  char *string;//要识别的字符串
  void (*module_fun)(void);//模块函数指针
  struct list_head list;//链表链接域
};

struct module_select module_select_head;//建立链表头

#define DEVICE_NAME "module_a"

struct file_operations module_a_fops ={
	.owner = THIS_MODULE,
	.open = module_a_open,
	.release = module_a_release,
	.write = module_a_write
};


void module_a_add_list(char *string, void(*module_fun)(void))//增加节点函数
{
    struct module_select *tmp_node;//每次申请链表节点时临时所用的指针
    tmp_node = (struct module_select *)kmalloc(sizeof(struct module_select), GFP_KERNEL);
    tmp_node->string = string;
    tmp_node->module_fun = module_fun;
    list_add_tail(&(tmp_node->list),&(module_select_head.list));
//    printk("Module_a:Node string: %s added!\n",string);	
}

void module_a_remove_list(char *string, void(*module_fun)(void))//删除节点函数
{
    struct list_head *pos, *n;//定义一个节点指针
    struct module_select *tmp_select;//定义一个module_select结构体指针变量
    list_for_each_safe(pos, n, &module_select_head.list)
    {
      tmp_select = list_entry(pos, struct module_select, list);
      if((tmp_select->string) == string)//是否匹配
      {
//        printk("Found the string is:%s Then delete it!\n",tmp_select->string);
        list_del(pos);//删除匹配特征字符串的链表节点
        kfree(tmp_select);//释放该数据节点所占内存    
      }
    }
}


static int module_a_open(struct inode *inode,struct file *file)
{
    struct list_head *pos;//定义一个节点指针
    struct module_select *tmp_select;//定义一个module_select结构体指针变量
    try_module_get(THIS_MODULE);
    list_for_each(pos, &module_select_head.list)
    {
       tmp_select = list_entry(pos, struct module_select, list);
       printk("module_a list is :%s\n",tmp_select->string);
    }
    return 0;
}
static int module_a_release(struct inode *inode,struct file *file)
{
    module_put(THIS_MODULE);
    return 0;
}


static ssize_t module_a_write (struct file *filp,const char *buf,size_t count,loff_t *f_pos)
{
    unsigned long flags = 0;
    int ret = 0;
    char *get_line_string = NULL;//行输入保存的指针
    struct list_head *pos;//定义一个节点指针
    struct module_select *tmp_select;//定义一个module_select结构体指针变量
    get_line_string = (char *)kmalloc(count+1, GFP_KERNEL);//+1增加结束符的空间
    get_line_string[count]='\0';//补回字符串结束符，因为送进内核的字符不包含结束符
    
    spin_lock_irqsave(&spinlock,flags);//自旋锁开始
    ret = copy_from_user(get_line_string, buf, count);//临界数据区
    spin_unlock_irqrestore(&spinlock,flags);//自旋锁结束
    
    if(ret)
    {
	printk("copy_from_user error \n");
	return -EFAULT;
    }
    
    list_for_each(pos, &module_select_head.list)
    {
       tmp_select = list_entry(pos, struct module_select, list);
       if(strstr(get_line_string,(tmp_select->string)) != NULL)//发现特征字符串
       {
	   (*(tmp_select->module_fun))();
       }
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
    device_create(module_a_class,NULL,devno,NULL,DEVICE_NAME);//创建设备节点
    spin_lock_init(&spinlock);//初始化自旋锁
    INIT_LIST_HEAD(&module_select_head.list);//初始化链表头 完成双向循环链表的创建 
    return 0;
error:
	unregister_chrdev_region(devno,1);
	return ret;
}

static void __exit module_a_udev_exit(void)
{
    struct list_head *pos,*n;
    struct module_select *p;
    device_destroy(module_a_class,devno);
    class_destroy(module_a_class);
    cdev_del(module_a);
    unregister_chrdev_region(devno,1);
    list_for_each_safe(pos, n, &module_select_head.list)//遍历并清除链表节点，释放内存空间
    {
      list_del(pos);
      p = list_entry(pos, struct module_select,list);
      kfree(p);
    }
    printk("Module_a:the line nodes are all clean!\nModule_a removed!\n");
  	
}
EXPORT_SYMBOL(module_a_add_list);//添加节点
EXPORT_SYMBOL(module_a_remove_list);//删除节点
module_init(module_a_udev_init);
module_exit(module_a_udev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ideask");

