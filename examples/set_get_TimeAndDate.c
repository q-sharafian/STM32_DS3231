#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_gpio.h"
#include "SHT2x.h"
#include "stm32f10x_rcc.h"

void main(){
// set clock 18 MHz for MCU
	PLL_SRC_Sel(2);
	PLL_MUL(2);
	Src_Sys_Clk(3);
	AHB_Div(4);
	APB1_APB2_Div(1, 1);

// set Clocks Peripherals MCU
	SetPeriphAPB1(APB1_I2C1);
	SetPeriphAPB2(APB2_IOPB);
	SetPeriphAPB2(APB2_AFIO);

// config pins for I2C 
	GPIO_InitTypeDef GPIOPB6_Init;
	GPIOPB6_Init.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIOPB6_Init.GPIO_Pin = 6;
	GPIOPB6_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIOPB6_Init);// config SCL

	GPIO_InitTypeDef GPIOPB7_Init;
	GPIOPB7_Init.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIOPB7_Init.GPIO_Pin = 6;
	GPIOPB7_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIOPB7_Init);// config SDA

// initialize I2C1
	I2C_Cmd(I2C1, ENABLE);
	I2C1_Init.I2C_Ack = I2C_Ack_Enable;
	I2C1_Init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C1_Init.I2C_ClockSpeed = 40000;
	I2C1_Init.I2C_DutyCycle = I2C_DutyCycle_16_9;
	I2C1_Init.I2C_Mode = I2C_Mode_I2C;
	I2C1_Init.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C1, &I2C1_Init);
	I2C_Cmd(I2C1, ENABLE);

/****** config Time and Date *********/

	// DS3231_WriteDate declaration in DS3231.h
	DS3231_WriteDate.Date = 8;
	DS3231_WriteDate.Day = 1;
	DS3231_WriteDate.Hour = 21;
	DS3231_WriteDate.Minutes = 59;
	DS3231_WriteDate.Month = 7;
	DS3231_WriteDate.Seconds = 36;
	DS3231_WriteDate.Year = 17;

	DS3231_SetDateTime(I2C1, &DS3231_WriteDate);// set time and date

/******* read Time and Date **********/

	// DS3231_ReadDate declaration in DS3231.h
	DS3231_GetDateTime(I2C1, &DS3231_ReadDate);// read time and date
	while(1);
	
};
