#!/bin/sh
mount -t ext2 /dev/sda1 /mnt/usb
/mnt/usb/linux-util/sbin/mkswap /mnt/usb/swapfile.img
/mnt/usb/linux-util/sbin/swapon /mnt/usb/swapfile.img
insmod /mnt/usb/drivers/cdc-acm.ko
export LD_LIBRARY_PATH="/mnt/usb/usr/arm-linux-gnueabi/lib;/mnt/usb/usr/arm-linux-gnueabi/components/lib"
cd /mnt/usb

