obj-m += add_sensor.o
add_sensor-objs := module_hooks.o add_sendmsg.o add_hello.o add.o add_tables.o add_rreq.o add_rrep.o add_controller.o

INCLUDE='$(PWD)/../include/Module.symvers'

all:
	make -C /lib/modules/$(shell uname -r)/build KBUILD_EXTRA_SYMBOLS=$(INCLUDE) M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean 
	rm -f *.ko *.o .*.tmp .*.cmd
