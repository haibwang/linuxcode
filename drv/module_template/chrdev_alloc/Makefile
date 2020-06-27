MAKE := make
MODPATH := $(shell pwd)
KERNEL_ROOT := /home/think/linux/kernel/linux-imx-rel_imx_4.1.15_2.1.0_ga
ARCH = arm
CROSS_COMPILE = arm-linux-gnueabihf-

export ARCH
export CROSS_COMPILE

obj-m := led_chrdev_drv.o

all:
	$(MAKE) -C $(KERNEL_ROOT) M=$(MODPATH) modules
	cp led_chrdev_drv.ko /home/think/nfs
clean:
	$(MAKE) -C $(KERNEL_ROOT) M=$(MODPATH) clean
	rm /home/think/nfs/*.ko






