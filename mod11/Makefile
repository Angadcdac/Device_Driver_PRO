obj-m  += mod11-2IRQ_all.o
 
ifdef ARCH
  #You can update your Beaglebone path here.
  KSRC = /home/angad/Documents/BBG/linux
else
  KSRC = /lib/modules/$(shell uname -r)/build
endif
 
all:
	make -C $(KSRC)  M=$(shell pwd) modules
 
clean:
	make -C $(KSRC)  M=$(shell pwd) clean
