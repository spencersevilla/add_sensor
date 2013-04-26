#!/bin/bash

cp include/Module.symvers src/Module.symvers
cd src
make
sudo insmod add_sensor.ko

mv *.o ../build/
mv *.ko ../build/
mv modules.order ../build/
mv Module.symvers ../build/
mv mobile_host.mod.c ../build/

# cd ../tests
# ./make_tests.sh

