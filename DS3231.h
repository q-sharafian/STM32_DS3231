#include "stm32f10x.h"

#define DS3231_FLAG_STATUS_A1F (1 << 0)
#define DS3231_FLAG_STATUS_A2F (1 << 1)

#define DS3231_FLAG_CONTROL_A1IE (1 << 0)
#define DS3231_FLAG_CONTROL_A2IE (1 << 1)
#define DS3231_FLAG_CONTROL_INTCN (1 << 2)
#define DS3231_FLAG_CONTROL_RS1 (1 << 3)
#define DS3231_FLAG_CONTROL_RS2 (1 << 4)
#define DS3231_FLAG_CONTROL_CONV (1 << 5)
#define DS3231_FLAG_CONTROL_BBSQW (1 << 6)
#define DS3231_FLAG_CONTROL_EOSC (1 << 7)

#define DS3231_ALARM_NO_MATCH 0x50
#define DS3231_ALARM_MATCH 0x00
#define DS3231_ALARM_DAY_MATCH 0x28// DY
#define DS3231_ALARM_DATE_MATCH 0x00// DT

#pragma once

struct DS3231_DateTime{
	char Seconds;
	char Minutes;
	char Hour;
	char Day;// Range 1-7
	char Date;// Range 1-31
	char Month;
	char Year;// Range 0-99
};

struct DS3231_Alarm1{
	char Seconds;
	char Minutes;
	char Hour;
	char DY_DT;//Day;// Range 1-7
	char Date;// Range 1-31
};

struct DS3231_Alarm2{
	char Minutes;
	char Hour;
	char DY_DT;//Day;// Range 1-7
	char Date;// Range 1-31
};

struct DS3231_DateTime DS3231_WriteDate;// structure for write date
struct DS3231_DateTime DS3231_ReadDate;// structure for read date

struct DS3231_Alarm1 DS3231_WriteAlarm1;// structure for write Alarm1
struct DS3231_Alarm1 DS3231_ReadAlarm1;// structure for read Alarm1

struct DS3231_Alarm2 DS3231_WriteAlarm2;// structure for write Alarm2
struct DS3231_Alarm2 DS3231_ReadAlarm2;// structure for read Alarm2

#ifndef MY_LIBRARY_DS3231_H_
#define MY_LIBRARY_DS3231_H_

void DS3231_SetDateTime(I2C_TypeDef* I2Cx, struct DS3231_DateTime *DS3231_Date);
void DS3231_GetDateTime(I2C_TypeDef* I2Cx, struct DS3231_DateTime *DS3231_Date);

void DS3231_SetAlarm1(I2C_TypeDef* I2Cx, struct DS3231_Alarm1 *DS3231_WriteAlarm1);
void DS3231_GetAlarm1(I2C_TypeDef* I2Cx, struct DS3231_Alarm1 *DS3231_ReadAlarm1);

void DS3231_SetAlarm2(I2C_TypeDef* I2Cx, struct DS3231_Alarm2 *DS3231_WriteAlarm2);
void DS3231_GetAlarm2(I2C_TypeDef* I2Cx, struct DS3231_Alarm2 *DS3231_ReadAlarm20);

uint8_t DS3231_GetFlag(I2C_TypeDef* I2Cx, uint8_t DS3231_FLAG_STATUS);
void DS3231_ClearFlag(I2C_TypeDef* I2Cx, uint8_t DS3231_FLAG_STATUS);

uint8_t DS3231_GetCotrolRegister(I2C_TypeDef* I2Cx);
void DS3231_SetCtrlRegister(I2C_TypeDef* I2Cx, uint8_t DS3231_FLAG_CONTROL);

void DS3231_SendData(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t DS3231_ReceiveData(I2C_TypeDef* I2Cx);

void DS3231_SendAddressRead(I2C_TypeDef* I2Cx);
void DS3231_SendAddressWrite(I2C_TypeDef* I2Cx);

void DS3231_GenerateStart(I2C_TypeDef* I2Cx);
void DS3231_GenerateStop(I2C_TypeDef* I2Cx);

uint8_t DS3231_DecToBCD(uint8_t value);
uint8_t DS3231_BCDToDec(uint8_t value);

void DS3231_GoToAddress(I2C_TypeDef* I2Cx, uint8_t AddressByte);

#endif /* MY_LIBRARY_DS3231_H_ */

/******** End Of File ***********/
