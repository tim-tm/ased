#!/bin/bash

grep -q BCM2835 /proc/cpuinfo
if [ $? = 0 ]; then
  echo Pi
fi