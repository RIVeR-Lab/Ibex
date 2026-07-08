LBITS := $(shell getconf LONG_BIT)
GEV_COMMON_DIR := s2i

KERNELRELEASE := $(shell uname -r)

DIR	    := precompiled/x86_64
DIR_SAV := precompiled/x86

KDIR   := /lib/modules/$(KERNELRELEASE)/build
PWD    := $(shell pwd)
MODLIB := /lib/modules/$(KERNELRELEASE)/extra

ifeq ($(M),)
GEV_DIR := $(PWD)
else
GEV_DIR := $(M)
ccflags-y += -I$(M) -I$(GEV_DIR)/$(GEV_COMMON_DIR)
endif

obj-m := s2igevfilter.o 


s2igevfilter-y := s2i_filter.o s2i_os.o

ifeq ($(shell test -e $(GEV_DIR)/$(GEV_COMMON_DIR)/s2i_gev.o && echo -n yes),yes) 
ldflags-y += -r $(GEV_DIR)/$(DIR)/s2i_gev.o
$(shell test -d "$(GEV_DIR)/precompiled" || mkdir -p "$(GEV_DIR)/precompiled")
$(shell test -d "$(GEV_DIR)/$(DIR)" || mkdir -p "$(GEV_DIR)/$(DIR)")
$(shell mv $(GEV_DIR)/$(GEV_COMMON_DIR)/s2i_gev.o $(GEV_DIR)/$(DIR))
else
ifeq ($(shell test -e $(GEV_DIR)/$(DIR)/s2i_gev.o && echo -n yes),yes) 
ldflags-y += $(GEV_DIR)/$(DIR)/s2i_gev.o
else
s2igevfilter-y += $(GEV_COMMON_DIR)/s2i_gev.o
endif
endif

ifeq ($(shell uname -a | grep -i 'ubuntu'), 0)
CFLAGS_s2i_filter.o := -D_UBUNTU_
endif

ifeq ($(shell test -e $(GEV_DIR)/$(GEV_COMMON_DIR)/s2i_utils.c && echo -n has-source),)
$(info ************  Link with s2i_utils  ************)
ldflags-y +=  $(GEV_DIR)/$(GEV_COMMON_DIR)/s2i_utils.o
else
$(info ************  Compile with s2i_utils ************)
s2igevfilter-y += $(GEV_COMMON_DIR)/s2i_utils.o
endif

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

copy:
	sudo cp s2igevfilter.ko $(MODLIB)

# depmod version for rpm builds
DEPVER := $(shell /sbin/depmod -V 2>/dev/null | \
          awk 'BEGIN {FS="."} NR==1 {print $$2}')
	
install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install 
	depmod -a
	$(shell test -d "/lib/udev/devices" || mkdir -p "/lib/udev/devices" ) 
	echo alias char-major-120 s2igevfilter >> /etc/modprobe.conf
	echo or 
	echo s2igevfilter >> /etc/modules 

uninstall:
	if [ -c $(MODLIB)/s2igevfilter.ko ] >/dev/null 2>&1; then rm $(MODLIB)/s2igevfilter.ko; fi
	depmod -a

clean:
	cp $(GEV_DIR)/$(DIR)/s2i_gev.o $(GEV_DIR)/$(DIR)/s2i_gev.sav
	if [ -f "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.o" ]; then cp "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.o" "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.sav"; fi
	$(shell test -f "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.o" && cp "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.o $(GEV_DIR)/$(DIR_SAV)/s2i_gev.sav" )
	make -C $(KDIR) M=$(PWD) clean
	-rm -f *.symvers
	-rm $(GEV_DIR)/$(GEV_COMMON_DIR)/s2i*.o
	-rm $(GEV_DIR)/$(GEV_COMMON_DIR)/.s2i*.o.cmd
	find . -name '*~' -exec rm {} \;
	mv $(GEV_DIR)/$(DIR)/s2i_gev.sav $(GEV_DIR)/$(DIR)/s2i_gev.o
	if [ -f "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.sav" ]; then mv "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.sav" "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.o"; fi

cleanall:
	if [ -f "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.o" ]; then cp "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.o" "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.sav"; fi
	make -C $(KDIR) M=$(PWD) clean
	-rm -f *.symvers
	find . -name '*~' -exec rm {} \;
	if [ -d "$(GEV_DIR)/$(DIR)" ]; then rm -r "$(GEV_DIR)/$(DIR)"; fi
	if [ -f "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.sav" ]; then mv "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.sav" "$(GEV_DIR)/$(DIR_SAV)/s2i_gev.o"; fi

