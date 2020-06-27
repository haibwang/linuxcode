## Linux驱动

### 调试

#### 挂载工作目录

##### 开发板挂载在linux虚拟机上

1. ip配置

​		windows，虚拟机，开发板IP配置，并处于同一个网段
> 虚拟机ip配置的时候，需要选择桥接模式，桥接模式默认是自动的，有可能会选用无线网卡，所以开发板会ping不通虚拟机。需在在vm里面（编辑->虚拟机网络编辑）添加一个桥接模式，使用有线网卡。

2. 虚拟机`nfs`共享文件夹

   - 安装NFS服务器:`nfs-kernel-server`

   ```shell
   sudo apt-get install nfs-kernel-server
   ```

   - 安装NFS客户端:`nfs-common`

   ```shell
   sudo apt-get install nfs-common
   ```

   - 添加nfs共享目录

   ```shell
   sudo vi /etc/exports
   /nfsroot  *(rw,sync,no_root_squash) #共享目录为/nfsroot
   ```

   - 启动nfs

   ```shell
   sudo /etc/init.d/nfs-kernel-server start
   #或者
   sudo /etc/init.d/nfs-kernel-server restart
   ```

3. 开发板挂载

```shell
mount -t nfs -o nolock,nfsvers=3 192.168.1.10:/nfsroot  /mnt/nfs
```

>文件系统挂接过程中出现如下错误： 
>mount -t nfs -o nolock 192.168.1.10:/home/b/linux/nfs  /mnt/nfs 
>mount.nfs: an incorrect mount option was specified
>
>解决方法： 添加nfsvers=3参数
>
>nfsvers＝n:设定要使用的NFS版本，默认是使用2，这个选项的设定还要取决于server端是否支持NFS VER 3 
>
>mount -t nfs -o nolock,**nfsvers=3** 192.168.1.10:/home/b/linux/nfs  /mnt/nfs



### 概念

#### 三种设备

**字符设备，块设备，网络设备**

- 字符设备

  一个一个字节的字节流，读写分先后顺序

- 块设备

   **系统中能够随机（不需要按顺序）访问固定大小数据片（chunks）的设备被称作块设备**，这些数据片就称作块。最常见的块设备是硬盘，除此以外，还有软盘驱动器、CD-ROM驱动器和闪存等等许多其他块设备 

   	**与字符设备的根本区别在于它可以被随机访问——换句话说就是，能在访问设备时随意地从一个位置跳转到另一个位置** 

- 网络设备

> linux驱动属于内核的一部分

#### 模块加载命令
- insmod:

  rmmod：卸载模块

- modprobe:比insmod智能，能够智能解决模块依赖问题，支持错误检查，错误报告功能

  modprobe -r 卸载模块,卸载模块时，若果模块依赖的模块没有被别的模块依赖，会被一起卸载

#### 设备节点创建

- 手动创建

    ```shell
    #filename :  设备文件名
    #type        :  设备文件类型
    #major      :   主设备号
    #minor      :   次设备号
    mknod filename type major minor

    #           设备名        c代表字符设备 200是主设备号 0是次设备号
    mknode /dev/device_name       c         200         0 
    ```

- 自动创建

  - 创建

      ```c
    struct class *cdev_class; 
      cdev_class = class_create(owner,name)
      device_create(_cls,_parent,_devt,_device,_fmt)
    ```
  
  - 销毁
  
      ```c
      device_destroy(_cls,_device)
      class_destroy(struct class * cls)
      ```
  
      

#### 设备号

- 设备号由**主设备号**和**次设备号**组成，数据类型为**dev_t**, 高12位主设备号，低20位次设备号，所以主设备号范围是0~4095

- 设备号常用操作方式

    ```c
    #define MINORBITS 20
    #define MINORMASK ((1U << MINORBITS) - 1)
    #define MAJOR(dev) ((unsigned int) ((dev) >> MIN
    #define MINOR(dev) ((unsigned int) ((dev) & MINO
    #define MKDEV(ma,mi) (((ma) << MINORBITS) | (mi))
    ```

- 设备号的申请

  注册字符设备的时候需要指定设备号

  ```c
  static inline int register_chrdev(unsigned int major, const char *name,
  				  const struct file_operations *fops)
  ```

  注册字符设备的函数参数需要输入主设备号major，该设备号可以通过静态分配和动态分配

  - 静态分配

    通过查看当前系统未使用的设备号，`cat  /proc/devices`，指定未使用的设备号即可

  - 动态分配

    ```c
    #include <linux/fs.h>
    
    typedef __u32 __kernel_dev_t;
    typedef __kernel_dev_t		dev_t;
    /*
    *  dev:保存申请到的设备号
    *  baseminor: 次设备号起始地址
    *  count: 要申请的设备数量
    *  name：设备名字
    */
    int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count, const char *name)
    
    /*
    *  已知主设备号时采用
    *  from：给定的设备号，采用MKDEV获取
    *  count：要申请的设备数量
    */   
    int register_chrdev_region(dev_t from, unsigned count, const char *name);
    
    /*
    *  from:要释放的设备号
    *  count: 表示从 from 开始，要释放的设备号数量
    */    
    void unregister_chrdev_region(dev_t from, unsigned count);
    ```
    
    

### 字符设备驱动

#### 字符设备区驱动组成

##### 模块初始化和退出

```c
module_init(ststic int __init (*init_func)(void));
module_exit(static void __exit (*exit_func)(void));
```

##### 驱动注册注销

- `static inline int register_chrdev(major,name,fops)`

- `static inline void unregister_chrdev(major,name)`

  - 字符设备的查看

    `cat /proc/devices`可以查看已经使用的设备号

  - 设备号 = 主设备号 + 次设备号

    高12位位主设备号，低20位为次设备号

##### 添加LICENSE

- 必须添加license，否者编译报错
- `MODULE_LICENSE("GPL");`

##### 完整的驱动模块一

```c
#include<linux/module.h>

static int __init module_init(void)
{
    
     return 0;
}

static void __exit module_exit(void)
{

}

module_init(module_init);
module_exit(module_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("AUTHOR");
```

##### 模块编译Makefile

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

##### 字符设备新的驱动方法

1. 申请设备号

2. 初始化字符设备

   ```c
   void cdev_init(struct cdev *cdev, const struct file_operations *fops)
   ```

3. 向内核添加字符设备

   ```c
   int cdev_add(struct cdev *p, dev_t dev, unsigned count)
   ```

4. 注销字符设备

   ```c
   void cdev_del(struct cdev *p)
   ```

   

