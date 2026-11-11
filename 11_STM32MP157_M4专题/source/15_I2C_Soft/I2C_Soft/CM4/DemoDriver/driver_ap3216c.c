#include "driver_ap3216c.h"
#include "driver_i2c1.h"

uint8_t AP3216_Init(void)
{
	uint8_t ret_value = 0;
	AP3216_WriteOneByte(SYS_CONFIG_ADDR, SYS_SW_RESET);	// 系统软件复位
	HAL_Delay(50);

	AP3216_WriteOneByte(SYS_CONFIG_ADDR, SYS_ALS_PS_IR_ACT);	// IR+PS+ALS三个都激活使用
	HAL_Delay(50);
	ret_value = AP3216_ReadOneByte(SYS_CONFIG_ADDR);	// 读取配置字
	if(ret_value != SYS_ALS_PS_IR_ACT)	// 判断是否与设置的一致
	{
		return 1;
	}
	printf("\r I2C Configuration register: 0x%x \n", SYS_CONFIG_ADDR);
	printf("\r I2C Configuration value: 0x%x \n", SYS_ALS_PS_IR_ACT);
	printf("\r I2C Read configuration value: 0x%x \n", ret_value);
	return 0;
}

void AP3216_ReadDataTest(void)
{
	uint16_t ir = 0, ps = 0, als = 0;

	AP3216_Read_IR_Data(&ir);
	AP3216_Read_PS_Data(&ps);
	AP3216_Read_ALS_Data(&als);

	printf("\r *************** \n");
	printf("\r IR = 0x%x\n", ir);
	printf("\r PS = 0x%x\n", ps);
	printf("\r ALS = 0x%x\n", als);
	printf("\r *************** \n");
}

uint8_t AP3216_WriteOneByte(uint8_t reg, uint8_t data)
{
	I2C_Start();
	I2C_SendByte(AP3216_ADDR | AP3216_W);

	I2C_SendByte(reg);

	I2C_SendByte(data);

	I2C_Stop();

	return 0;
}

uint8_t AP3216_ReadOneByte(uint8_t reg)
{
	uint8_t ret_value = 0;

	I2C_Start();
	I2C_SendByte(AP3216_ADDR | AP3216_W);

	I2C_SendByte(reg);

	I2C_Start();
	I2C_SendByte(AP3216_ADDR | AP3216_R);

	ret_value = I2C_ReadByte(NACK);
	I2C_Stop();
	return ret_value;
}

void AP3216_Read_IR_Data(uint16_t *pIR)
{
	uint8_t ir_l = 0, ir_h = 0;

	ir_l = AP3216_ReadOneByte(IR_DATA_LOW);
	ir_h = AP3216_ReadOneByte(IR_DATA_HIGH);

	if( (ir_l&0x80) == 0x80)	// Invalid IR and PS data
	{
		*pIR = 0;
	}
	else	// ir_l Bit[1:0] is data, ps_l bits[3:0] ans ps_h bits[5:0] is data
	{
		*pIR = (ir_h<<8) | (ir_l&0x03);
	}
}

void AP3216_Read_PS_Data(uint16_t *pPS)
{
	uint8_t ps_l = 0, ps_h = 0;

	ps_l = AP3216_ReadOneByte(PS_DATA_LOW);
	ps_h = AP3216_ReadOneByte(PS_DATA_HIGH);

	if( (ps_l&0x40)==0x40)	// Invalid IR and PS data
	{
		*pPS = 0;
	}
	else	// ir_l Bit[1:0] is data, ps_l bits[3:0] ans ps_h bits[5:0] is data
	{
		*pPS = ((ps_h&0x1F)<<8) | (ps_l&0x0F);
	}
}

void AP3216_Read_ALS_Data(uint16_t *pALS)
{
	uint8_t als_l = 0, als_h = 0;

	als_l = AP3216_ReadOneByte(ALS_DATA_LOW);
	als_h = AP3216_ReadOneByte(ALS_DATA_HIGH);

	*pALS = (als_h<<8) | (als_l);
}

