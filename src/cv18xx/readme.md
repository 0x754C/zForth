# cv18xx zforth support

# build

```
make
```

# install

```
mount /dev/mmcblk0p1 /mnt/sd0p1
cp fip.bin /mnt/sd0p1/
```

# connect

connect your usb2ttl to UART0 pins

uart0 default baudrate is 128000

# memory barrier

we must use memory barrier, when write/read some value to/from mmio register,
because thead-c906 support out of order execution.
