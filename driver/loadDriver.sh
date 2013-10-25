#!/bin/sh                                                                                                                                                                                                                                     
module="EpitechSerial"
device="EpitechSerialDev"
major="240"

insmod ./$module.ko $* || exit 1
mknod /dev/$device c $major 0
chmod 777 /dev/$device
