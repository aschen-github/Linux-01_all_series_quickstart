#!/bin/sh
cd /etc/openocd/scripts
openocd -f interface/stlink.cfg -f target/stm32f0x.cfg -c init -c halt -c "flash write_image erase /root/demo.bin 0x08000000" -c reset -c shutdown
