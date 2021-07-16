obj-m += semdemo.o
KSRC = /home/harshitha/ebuildws/workspace/kbuild/linux-4.14.238


all:
	make -C ${KSRC} M=${PWD} modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
clean:
	make -C ${KSRC} M=${PWD} clean ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
 
host:
	make -C /lib/modules/${shell uname -r}/build/ M=${PWD} modules
