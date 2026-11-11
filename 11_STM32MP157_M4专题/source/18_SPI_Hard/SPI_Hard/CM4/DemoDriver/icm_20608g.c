#include "icm_20608g.h"
#include "spi.h"

#define Dummy_Byte				(0xFF)

uint8_t ICM_RW_Register(uint8_t reg, uint8_t data)
{
	uint16_t rxdata = 0;
	uint16_t txdata = (data<<8) | reg;

	if(HAL_SPI_TransmitReceive(&hspi5, (uint8_t*)&txdata, (uint8_t*)&rxdata, sizeof(rxdata), 300) != HAL_OK)
	{
		Error_Handler();
	}

	return (rxdata>>8);
}


void ICM_Init(void)
{
	uint8_t ret = 0;

	// 复位ICM
	ICM_RW_Register(ICM_PWM_MGMT_1 | ICM_WRITE, 0x80);
	HAL_Delay(50);

	// 选择时钟
	ICM_RW_Register(ICM_PWM_MGMT_1 | ICM_WRITE, 0x01);
	HAL_Delay(50);
	// 获取设备ID
	ret = ICM_RW_Register(ICM_WHO_AM_I | ICM_READ, Dummy_Byte);
	printf("\rICM ID: 0x%x\n", ret);

	// 输出速率：内部采样率
	ICM_RW_Register(ICM_SMPLRT_DIV | ICM_WRITE, 0x00);
	// 陀螺仪 2000dps
	ICM_RW_Register(ICM_GYRO_CONFIG | ICM_WRITE, 0x18);
	// 加速度16G量程
	ICM_RW_Register(ICM_ACCEL_CONFIG | ICM_WRITE, 0x18);
	// 陀螺仪低通滤波 20Hz
	ICM_RW_Register(ICM_CONFIG | ICM_WRITE, 0x04);
	// 加速度低通滤波 21.2Hz
	ICM_RW_Register(ICM_ACCEL_CONFIG_2 | ICM_WRITE, 0x04);

	// 打开加速度计和陀螺仪所有轴
	ICM_RW_Register(ICM_PWM_MGMT_1 | ICM_WRITE, 0x00);

	// 关闭低功耗
	ICM_RW_Register(ICM_LP_MODE_CFG | ICM_WRITE, 0x00);

	// 关闭FIFO
	ICM_RW_Register(ICM_FIFO_EN | ICM_WRITE, 0x00);
}

void ICM_ReadGyroAccel(void)
{
	int16_t temp = 0;
	float temp_value = 0;
	uint8_t temp_h = 0, temp_l = 0;

	int16_t xa = 0, ya = 0, za = 0;
	float xa_act = 0, ya_act = 0, za_act = 0;
	uint8_t xa_l = 0, ya_l = 0, za_l = 0;
	uint8_t xa_h = 0, ya_h = 0, za_h = 0;

	int16_t xg = 0, yg = 0, zg = 0;
	float xg_act = 0, yg_act = 0, zg_act = 0;
	uint8_t xg_l = 0, yg_l = 0, zg_l = 0;
	uint8_t xg_h = 0, yg_h = 0, zg_h = 0;

	temp_l = ICM_RW_Register(ICM_TEMP_OUT_L | ICM_READ, Dummy_Byte);
	temp_h = ICM_RW_Register(ICM_TEMP_OUT_H | ICM_READ, Dummy_Byte);
	temp = (temp_h<<8) | temp_l;
	temp_value = (temp - 25)/326.8 + 25;

	xa_l = ICM_RW_Register(ICM_ACCEL_XOUT_L | ICM_READ, Dummy_Byte);
	xa_h = ICM_RW_Register(ICM_ACCEL_XOUT_H | ICM_READ, Dummy_Byte);
	ya_l = ICM_RW_Register(ICM_ACCEL_YOUT_L | ICM_READ, Dummy_Byte);
	ya_h = ICM_RW_Register(ICM_ACCEL_YOUT_H | ICM_READ, Dummy_Byte);
	za_l = ICM_RW_Register(ICM_ACCEL_ZOUT_L | ICM_READ, Dummy_Byte);
	za_h = ICM_RW_Register(ICM_ACCEL_ZOUT_H | ICM_READ, Dummy_Byte);
	xa = xa_l + (xa_h<<8);	xa_act = xa / 2048.0;
	ya = ya_l + (ya_h<<8);	ya_act = ya / 2048.0;
	za = za_l + (za_h<<8);	za_act = za / 2048.0;

	xg_l = ICM_RW_Register(ICM_GYRO_XOUT_L | ICM_READ, Dummy_Byte);
	xg_h = ICM_RW_Register(ICM_GYRO_XOUT_H | ICM_READ, Dummy_Byte);
	yg_l = ICM_RW_Register(ICM_GYRO_YOUT_L | ICM_READ, Dummy_Byte);
	yg_h = ICM_RW_Register(ICM_GYRO_YOUT_H | ICM_READ, Dummy_Byte);
	zg_l = ICM_RW_Register(ICM_GYRO_ZOUT_L | ICM_READ, Dummy_Byte);
	zg_h = ICM_RW_Register(ICM_GYRO_ZOUT_H | ICM_READ, Dummy_Byte);
	xg = xg_l + (xg_h<<8);	xg_act = xg / 16.4;
	yg = yg_l + (yg_h<<8);	yg_act = yg / 16.4;
	zg = zg_l + (zg_h<<8);	zg_act = zg / 16.4;

	printf("\r-----------------------------------\n");

	printf("\rTemperature Original value: 0x%x \n", temp);
	printf("\rTemperature Converted value: %.2f\n", temp_value);
	printf("\r\n");
	printf("\rAccelerometer Original value: xa:0x%x ya:0x%x za:0x%x \n", xa, ya, za);
	printf("\rAccelerometer Converted value: xa_act:%.2fg ya_act:%.2fg za_act:%.2fg \n", xa_act, ya_act, za_act);
	printf("\r\n");
	printf("\rGyroscope Original value: xg:0x%x yg:0x%x zg:0x%x \n", xg, yg, zg);
	printf("\rGyroscope Converted value: xg_act:%.2fdeg/s yg_act:%.2fdeg/s zg_act:%.2fdeg/s \n", xg_act, yg_act, zg_act);

	printf("\r-----------------------------------\n");
}
