KVER = $(shell uname -r)
obj-m :=module_a.o module_b.o module_c.o 

kernel_modules:
	make -C /lib/modules/$(KVER)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVER)/build M=$(PWD) clean

insmod:
	insmod module_a.ko
	insmod module_b.ko
	insmod module_c.ko
rmmod:
	rmmod module_c.ko
	rmmod module_b.ko
	rmmod module_a.ko

