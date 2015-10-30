CFLAGS_kisni.o := -DDEBUG

obj-m += kisni.o

kisni-objs := keysniffer.o

KERNELVERSION = $(shell uname -r)
KDIR := /lib/modules/$(KERNELVERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
