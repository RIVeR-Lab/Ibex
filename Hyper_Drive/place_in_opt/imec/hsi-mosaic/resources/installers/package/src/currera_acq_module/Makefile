KERNEL?=$(shell uname -r)

modname=currera_acq

obj-m := $(modname).o
all:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) modules
reload:
	rmmod $(modname)
	insmod $(modname).ko
load:
	insmod $(modname).ko
	mknod /dev/cura0 c `awk '$$2=="$(modname)" {print $$1}' /proc/devices` 0
	chmod a+rw /dev/cura0
unload:
	rmmod $(modname)
	rm /dev/cura0
test: test.o libsh.o
	gcc -ggdb -o test test.o libsh.o
libsh.o: libsh.c
	gcc -ggdb -c libsh.c
test.o: test.c
	gcc -ggdb -c test.c
clean:
	rm -fR .*.cmd *.o *.ko .tmp_versions Module.symvers modules.order *.mod.c test
