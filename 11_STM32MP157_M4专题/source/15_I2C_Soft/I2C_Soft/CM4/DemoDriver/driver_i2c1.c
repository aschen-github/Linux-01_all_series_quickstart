/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 *
 * 文件名称：driver_I2C.c
 * 摘要：
 *
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#include "tim.h"
#include "driver_i2c1.h"
#include "driver_tim.h"

#define I2C_Delay()     us_timer_delay(5)

/*
 *  函数名：void I2C_Init(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用: SCL/SDA都初始为高电平
*/
void I2C_Init(void)
{
    SCL_H();
    SDA_H();
}

/*
 *  函数名：static void I2C_SDA_OUT(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：配置SDA引脚为输出
*/
static void I2C_SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin       = I2C1_SDA_Pin;
    HAL_GPIO_Init(I2C1_SDA_GPIO_Port, &GPIO_InitStruct);
}

/*
 *  函数名：static void I2C_SDA_IN(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：配置SDA引脚为输入
*/
static void I2C_SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin       = I2C1_SDA_Pin;
    HAL_GPIO_Init(I2C1_SDA_GPIO_Port, &GPIO_InitStruct);
}

/*
 *  函数名：void I2C_Start(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：I2C开始信号
*/
void I2C_Start(void)
{
    I2C_SDA_OUT();

    SCL_H();
	I2C_Delay();

    SDA_H();
    I2C_Delay();

    SDA_L();
    I2C_Delay();

    SCL_L();
    I2C_Delay();
}

/*
 *  函数名：void I2C_Stop(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：I2C停止信号
*/
void I2C_Stop(void)
{
    I2C_SDA_OUT();

    SDA_L();
    I2C_Delay();

    SCL_H();
    I2C_Delay();

    SDA_H();
    I2C_Delay();
}

/*
 *  函数名：void I2C_ACK(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：I2C发出应答信号
*/
void I2C_ACK(void)
{
    I2C_SDA_OUT();

    SCL_L();
    I2C_Delay();

    SDA_L();
    I2C_Delay();

    SCL_H();
    I2C_Delay();

    SCL_L();
    I2C_Delay();
}

/*
 *  函数名：void I2C_NACK(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：I2C发出非应答信号
*/
void I2C_NACK(void)
{
    I2C_SDA_OUT();

    SCL_L();
    I2C_Delay();

    SDA_H();
    I2C_Delay();

    SCL_H();
    I2C_Delay();

    SCL_L();
    I2C_Delay();
}

/*
 *  函数名：uint8_t I2C_GetACK(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：I2C等待从机的应答信号
*/
uint8_t I2C_GetACK(void)
{
    uint8_t time = 0;
    I2C_SDA_IN();

    SCL_L();
	I2C_Delay();

    SDA_H();
	I2C_Delay();

	SCL_H();
	I2C_Delay();

    while(SDA_INPUT())
    {
        time++;
        if(time>250)
        {
            SCL_L();
            return 1;
        }
    }
    SCL_L();

    return 0;
}

/*
 *  函数名：void I2C_SendByte(uint8_t data)
 *  输入参数：data->发送的数据
 *  输出参数：无
 *  返回值：无
 *  函数作用：I2C发送一个字节
*/
void I2C_SendByte(uint8_t data)
{
    uint8_t cnt = 0;

    I2C_SDA_OUT();

    for(cnt=0; cnt<8; cnt++)
    {
        SCL_L();
        I2C_Delay();

        if(data & 0x80)
        {
            SDA_H();
        }
        else
        {
            SDA_L();
        }
        data = data<<1;

        SCL_H();
        I2C_Delay();
    }

    SCL_L();
    I2C_Delay();
    I2C_GetACK();
}

/*
 *  函数名：uint8_t I2C_ReadByte(uint8_t ack)
 *  输入参数：ack->发送的应答标志，1应答，0非应答
 *  输出参数：无
 *  返回值：返回读到的字节
 *  函数作用：I2C读出一个字节
*/
uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t cnt;
    uint8_t data = 0xFF;
    I2C_SDA_IN();
    SCL_L();
    I2C_Delay();

    for(cnt=0; cnt<8; cnt++)
    {
        SCL_H();                 //SCL高(读取数据)
        I2C_Delay();

        data <<= 1;
        if(SDA_INPUT())
        {
            data |= 0x01;        //SDA高(数据有效)
        }
        SCL_L();
        I2C_Delay();
    }
    //发送应答信号，为低代表应答，高代表非应答
    if(ack == 0)
    {
        I2C_NACK();
    }
    else
    {
        I2C_ACK();
    }
    return data;                 //返回数据
}


