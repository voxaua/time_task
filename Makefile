ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m  := time_task.o
else
# normal makefile
KDIR ?= /home/vivashchenko/Documents/linux

default:
	$(MAKE) -C $(KDIR) M=$$PWD
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
endif

