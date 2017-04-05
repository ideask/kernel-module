KVER = $(shell uname -r)
obj-m :=module_a.o module_b.o module_c.o 

kernel_modules:
	make -C /lib/modules/$(KVER)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVER)/build M=$(PWD) clean
