- Generate dtbo(Linux Host):
export HEAD_ERFILE_PATH=~/100ask_sdk/stm32mp157_buildroot_2020.02/output/build/linux-origin_master/include
export DTBO_NAME=100ask_stm32mp1_i2c1_m4
cpp -nostdinc -I $HEAD_ERFILE_PATH -I arch -undef -x assembler-with-cpp $DTBO_NAME.dts dts.pre
dtc -O dtb -o $DTBO_NAME.dtbo -I dts dts.pre && rm dts.pre



- Use dtbo(Linux Board):
export DTBO_NAME=100ask_stm32mp1_i2c1_m4

mkdir -p /sys/kernel/config/device-tree/overlays/$DTBO_NAME
cat $DTBO_NAME.dtbo > /sys/kernel/config/device-tree/overlays/$DTBO_NAME/dtbo
echo 1 > /sys/kernel/config/device-tree/overlays/$DTBO_NAME/status
cat /sys/kernel/config/device-tree/overlays/$DTBO_NAME/status


