#!/bin/sh                                                                                                                                                                                                                                     
module="EpitechSerial"
device="EpitechSerialDev"

rmmod ./$module.ko $* || exit 1
rm /dev/$device