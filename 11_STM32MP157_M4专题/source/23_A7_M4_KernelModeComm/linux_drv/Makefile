
# export ARCH=arm
# export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
# export PATH=$PATH:~/100ask_stm32mp157_pro-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin

KERN_DIR = ~/stm32mp157/V2/stm32mp15xc-kernel/

all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m	+= rpmsg_client_sample.o
