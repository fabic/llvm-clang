# kmodules/Makefile
#
# 2015-11-30
#
# See misc/linux-kernel/Documentation/kbuild/modules.txt

obj-m+=simple1_hello.o

all:
	make -C ../misc/linux-kernel/ M=$(PWD) modules

clean:
	make -C ../misc/linux-kernel/ M=$(PWD) clean

#all:
#	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
#
#clean:
#	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
