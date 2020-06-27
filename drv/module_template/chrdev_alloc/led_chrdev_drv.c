/**
 * des:    动态分配设备号字符驱动模板
 * author: bo
 * date:   2020/06/27
 */

#include <linux/types.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
struct char_dev_t 
{
    unsigned int major;
    dev_t dev;
    char *name;
    char buf[1024];
};

struct char_dev_t char_dev=
{
    .major = 200,
    .name = "char_dev"
};

ssize_t chrdev_read(struct file *file, char __user *buf, size_t size, loff_t *loff)
{
    printk("read size =%d\n",size);

    if(size > sizeof(char_dev.buf))
    {
        size = sizeof(char_dev.buf);
    }
    copy_to_user(buf, "char_dev.buf:", size);
    copy_to_user(buf+13, char_dev.buf, size-15);

    return size;
}

ssize_t chrdev_write(struct file *file, const char __user *buf, size_t size, loff_t *loff)
{
    int i=0;
    unsigned long s;

    printk("write size = %d\n", size);
    if(size > sizeof(char_dev.buf))
    {
        size = sizeof(char_dev.buf);
    }
    s = copy_from_user(char_dev.buf, buf, size);

    while(size--)
    printk("%c",buf[i++]);
 
    return s;
}

int chrdev_open(struct inode *inode, struct file *file)
{
    printk("chrdev_open\n");
    
    return 0;
}

int chrdev_release(struct inode *inode, struct file *file)
{
    printk("chrdev_release\n");

    return 0;
}


struct file_operations fops=
{
    .owner   = THIS_MODULE,
    .open    = chrdev_open,
    .read    = chrdev_read,
    .write   = chrdev_write,
    .release = chrdev_release,
};


static int __init chr_dev_init(void)
{
    printk("chr_dev_init....\n");
   
    alloc_chrdev_region(&char_dev.dev, 0, 1, char_dev.name);
    
    return 0;
}


static void __exit che_dev_exit(void)
{
    printk("che_dev_exit,bye...\n");
    unregister_chrdev(char_dev.major, char_dev.name);
}


module_init(chr_dev_init);
module_exit(che_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bo");

