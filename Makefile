modname := kisni
obj-m := $(modname).o

kisni-objs := spy.o

KVERSION = $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build

ifdef DEBUG
CFLAGS_$(obj-m) := -DDEBUG
endif

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	-rmmod $(modname)
	insmod $(modname).ko

unload:
	-rmmod $(modname)

install:
	mkdir -p /lib/modules/$(KVERSION)/misc/$(modname)
	install -m 0755 -o root -g root $(modname).ko /lib/modules/$(KVERSION)/misc/$(modname)
	depmod -a

uninstall:
	rm /lib/modules/$(KVERSION)/misc/$(modname)/$(modname).ko
	rmdir /lib/modules/$(KVERSION)/misc/$(modname)
	rmdir /lib/modules/$(KVERSION)/misc
	depmod -a
