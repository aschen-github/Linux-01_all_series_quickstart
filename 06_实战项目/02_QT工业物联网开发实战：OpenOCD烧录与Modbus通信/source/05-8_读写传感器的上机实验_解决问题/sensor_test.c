#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <modbus.h>
#include <errno.h>

#define DO_BEEP1 0
#define DO_BEEP2 1
#define DO_LED1 2
#define DO_LED2 3
#define DO_LED3 4
#define AI_TEMP 0
#define AI_HUMI 1

void usage(const char *progname) {
    printf("Usage: %s <device> <command>\n", progname);
    printf("Commands:\n");
    printf("  beep1 on/off\n");
    printf("  beep2 on/off\n");
    printf("  led1 on/off\n");
    printf("  led2 on/off\n");
    printf("  led3 on/off\n");
    printf("  temp\n");
    printf("  humi\n");
}

int set_digital_output(modbus_t *ctx, int addr, int value) {
    return modbus_write_bit(ctx, addr, value);
}

int get_analog_input(modbus_t *ctx, int addr, float *value) {
    uint16_t reg;
    int rc = modbus_read_input_registers(ctx, addr, 1, &reg);
    if (rc == -1) {
        return -1;
    }
    *value = (float)reg / 10.0; // Assuming 10x scaling for temperature and humidity
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        usage(argv[0]);
        return -1;
    }

    modbus_t *ctx;
    int rc;

    // 创建Modbus上下文
    ctx = modbus_new_rtu(argv[1], 115200, 'N', 8, 1);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    modbus_set_debug(ctx, TRUE);

    // 设置Modbus从站地址为3
    rc = modbus_set_slave(ctx, 3);
    if (rc == -1) {
        fprintf(stderr, "Failed to set slave address: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // 连接到Modbus设备
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // 设置Modbus RTU模式为RS-485
    rc = modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    if (rc == -1) {
        fprintf(stderr, "Failed to set RS-485 mode: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }    
    
    if (strcmp(argv[2], "beep1") == 0) {
        if (argc != 4 || (strcmp(argv[3], "on") != 0 && strcmp(argv[3], "off") != 0)) {
            usage(argv[0]);
            return -1;
        }
        rc = set_digital_output(ctx, DO_BEEP1, strcmp(argv[3], "on") == 0);
    } else if (strcmp(argv[2], "beep2") == 0) {
        if (argc != 4 || (strcmp(argv[3], "on") != 0 && strcmp(argv[3], "off") != 0)) {
            usage(argv[0]);
            return -1;
        }
        rc = set_digital_output(ctx, DO_BEEP2, strcmp(argv[3], "on") == 0);
    } else if (strcmp(argv[2], "led1") == 0) {
        if (argc != 4 || (strcmp(argv[3], "on") != 0 && strcmp(argv[3], "off") != 0)) {
            usage(argv[0]);
            return -1;
        }
        rc = set_digital_output(ctx, DO_LED1, strcmp(argv[3], "on") == 0);
    } else if (strcmp(argv[2], "led2") == 0) {
        if (argc != 4 || (strcmp(argv[3], "on") != 0 && strcmp(argv[3], "off") != 0)) {
            usage(argv[0]);
            return -1;
        }
        rc = set_digital_output(ctx, DO_LED2, strcmp(argv[3], "on") == 0);
    } else if (strcmp(argv[2], "led3") == 0) {
        if (argc != 4 || (strcmp(argv[3], "on") != 0 && strcmp(argv[3], "off") != 0)) {
            usage(argv[0]);
            return -1;
        }
        rc = set_digital_output(ctx, DO_LED3, strcmp(argv[3], "on") == 0);
    } else if (strcmp(argv[2], "temp") == 0) {
        if (argc != 3) {
            usage(argv[0]);
            return -1;
        }
        float temp;
        rc = get_analog_input(ctx, AI_TEMP, &temp);
        if (rc == 0) {
            printf("Temperature: %.1f C\n", temp);
        }
    } else if (strcmp(argv[2], "humi") == 0) {
        if (argc != 3) {
            usage(argv[0]);
            return -1;
        }
        float humi;
        rc = get_analog_input(ctx, AI_HUMI, &humi);
        if (rc == 0) {
            printf("Humidity: %.1f %%\n", humi);
        }
    } else {
        usage(argv[0]);
        return -1;
    }

    if (rc == -1) {
        fprintf(stderr, "%s\n", modbus_strerror(errno));
    }

    // 关闭连接并释放资源
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
