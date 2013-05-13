#!/bin/bash

cd src
make
sudo insmod add_sensor.ko add_id=$1 is_controller=$2

mv *.o ../build/
mv *.ko ../build/
mv modules.order ../build/
mv Module.symvers ../build/
mv mobile_host.mod.c ../build/

# cd ../tests
# ./make_tests.sh

