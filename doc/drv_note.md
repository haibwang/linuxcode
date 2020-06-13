##### 三种设备：字符设备，块设备，网络设备

- 字符设备：一个一个字节的字节流，读写分先后顺序

> linux驱动书序内核的一部分

##### 模块加载命令

- insmod:

  rmmod：卸载模块

- modprobe:比insmod智能，能够智能解决模块依赖问题，支持错误检查，错误报告功能

  modprobe -r 卸载模块,卸载模块时，若果模块依赖的模块没有被别的模块依赖，会被一起卸载

##### 字符设备区驱动组成

###### 模块初始化和退出

```
module_init(ststic int __init (*init_func)(void));
module_exit(static void __exit (*exit_func)(void));
```



###### 驱动注册注销

- `static inline int register_chrdev(major,name,fops)`

- `static inline void unregister_chrdev(major,name)`
  - 字符设备的查看

    `cat /proc/devices`可以查看已经使用的设备号

  - 设备号 = 主设备号 + 次设备号

    高12位位主设备号，低20位为次设备号

###### 添加LICENSE

- 必须添加license，否者编译报错
- `MODULE_LICENSE("GPL");`



###### 完整的驱动模块一

```c
#include<linux/module.h>

static int __init module_init(void)
{
    
     return 0；
}

static void __exit module_exit(void)
{

}

module_init(module_init);
module_exit(module_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("AUTHOR");
```

###### 模块编译Makefile

```makefile
MODPATH := $(shel pwd)
KERNALE_ROOT := somewhere   #源码头文件目录

obj-m := targets.o    #模块名,可以是多个模块, -y是编译到内核
#targets-m := xxx.o   #模块依赖的其他文件
#ccfalgs-m := xxx     #模块的cflags

all:
	$(MAKE) -C $(KERNALE_ROOT) M=$(MODPATH) modules
clean:
	$(MAKE) -C $(KERNALE_ROOT) M=$(MODPATH) clean

```





