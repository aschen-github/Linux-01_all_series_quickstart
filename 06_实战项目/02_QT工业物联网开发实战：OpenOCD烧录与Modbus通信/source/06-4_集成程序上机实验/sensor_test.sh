#!/bin/sh

# 判断参数数量
if [ $# -lt 1 ] || [ $# -gt 2 ]; then
    echo "Usage: /bin/sensor_test.sh [led1|led2|led3|beep1|beep2|humi|temp] [on|off]"
    exit 1
fi

device="/dev/ttymxc2"

# 判断第一个参数
case $1 in
    led1|led2|led3|beep1|beep2)
        # 判断是否为 led 或 beep 命令
        if [ $# -eq 2 ] && [[ "$2" == "on" || "$2" == "off" ]]; then
            # 调用传感器测试程序
            /bin/sensor_test "$device" "$1" "$2"
 	else
            echo "Error: Invalid command for $1. Use 'on' or 'off'."
            exit 1
        fi
        ;;
    humi|temp)
        # 判断是否为 humi 或 temp 命令
        if [ $# -eq 1  ]; then
            # 调用传感器测试程序
            result=$(/bin/sensor_test "$device" "$1")

	    # 使用 grep 和 sed 提取数值
            if [ "$1" == "temp" ]; then
                value=$(echo "$result" | grep -oP "Temperature: \K[0-9]+\.[0-9]+")
            elif [ "$1" == "humi" ]; then
                value=$(echo "$result" | grep -oP "Humidity: \K[0-9]+\.[0-9]+")
            fi
            # 输出结果
            echo "$value"

        else
            echo "Error: Invalid command for $1. No additional arguments needed."
            exit 1
        fi
        ;;
    *)
        echo "Error: Invalid device. Use led1, led2, beep1, beep2, humi, or temp."
        exit 1
        ;;
esac
