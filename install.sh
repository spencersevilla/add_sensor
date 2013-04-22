#!/bin/bash

cd src
make
sudo insmod add_sensor.ko
# cd ../tests
# ./make_tests.sh

