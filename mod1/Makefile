obj-m += mod1.o

ifdef ARCH
  #You can update your Beaglebone path here.
  KSRC := /home/angad/Documents/BBG/linux
else
  KSRC = /lib/modules/$(shell uname -r)/build
endif

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KSRC) M=$(PWD) modules

clean:
	make -C $(KSRC) M=$(PWD) clean
	


                                                     



