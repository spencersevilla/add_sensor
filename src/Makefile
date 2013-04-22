obj-m += add_sensor.o
add_sensor-objs := module_hooks.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean 
	rm -f *.ko *.o .*.tmp .*.cmd
