/****************************************
 * @file    DS3231.c
 * @author  Ghasem Sharafiyan
 * @version V1.1
 * @date    July-2017 (Tir-1396)
 * @license MIT
 * @brief 	this file is library for initialize and
 *  		work with DS3231 IC
 *****************************************
 */

#include "DS3231.h"
#include "stm32f10x_i2c.h"

#define DS3231_ADDRESS_CONTROL_REGISTER 0x0E
#define DS3231_ADDRESS_STATUS_REGISTER 0x0F


void DS3231_SetDateTime(I2C_TypeDef* I2Cx, struct DS3231_DateTime *DS3231_Date)
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	DS3231_GenerateStart(I2Cx);// send start condition
	DS3231_SendAddressWrite(I2Cx);// send address DS3231(address: 0xD0)

	DS3231_SendData(I2Cx, 0x00);// send WORD ADDRESS 0
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_Date->Seconds));// send second
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_Date->Minutes));// send Minutes
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_Date->Hour));// send Hour
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_Date->Day));// send Day
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_Date->Date));// send Date
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_Date->Month));// send Month
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_Date->Year));// send Year

	DS3231_GenerateStop(I2Cx);
};
void DS3231_GetDateTime(I2C_TypeDef* I2Cx, struct DS3231_DateTime *DS3231_Date)
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge
	DS3231_GoToAddress(I2Cx, 0x00);
	DS3231_GenerateStart(I2Cx);// send start condition
	DS3231_SendAddressRead(I2Cx);// send address DS3231(address: 0xD0)

	DS3231_Date->Seconds = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx));// receive seconds
	DS3231_Date->Minutes = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx));// receive Month
	DS3231_Date->Hour = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx));// receive Hour
	DS3231_Date->Day = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx));// receive Day
	DS3231_Date->Date = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx));// receive Date
	DS3231_Date->Month = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx));// receive Month
	DS3231_Date->Year = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx));// receive Year

	I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);// create Not acknowledge
	I2C_AcknowledgeConfig(I2Cx, DISABLE);// disable acknowledge
	DS3231_GenerateStop(I2Cx);
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// Enable acknowledge
};

void DS3231_SetAlarm1(I2C_TypeDef* I2Cx, struct DS3231_Alarm1 *DS3231_WriteAlarm1)
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge
	DS3231_GenerateStart(I2Cx);// send start condition
	DS3231_SendAddressWrite(I2Cx);// send address DS3231(address: 0xD0)

	DS3231_SendData(I2Cx, 0x07);// send WORD ADDRESS 7
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_WriteAlarm1->Seconds));// send second
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_WriteAlarm1->Minutes));// send Minutes
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_WriteAlarm1->Hour));// send Hour
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_WriteAlarm1->DY_DT));// send Day
	//DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_WriteAlarm1->Date));// send Date

	DS3231_GenerateStop(I2Cx);
};
void DS3231_GetAlarm1(I2C_TypeDef* I2Cx, struct DS3231_Alarm1 *DS3231_ReadAlarm1)
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge
	DS3231_GoToAddress(I2Cx, 0x07);
	DS3231_GenerateStart(I2Cx);// send start condition
	DS3231_SendAddressRead(I2Cx);// send address DS3231(address: 0xD0)

	DS3231_ReadAlarm1->Seconds = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx) & 0x7F);// receive seconds
	DS3231_ReadAlarm1->Minutes = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx) & 0x7F);// receive Month
	DS3231_ReadAlarm1->Hour = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx) & 0x3F);// receive Hour
	DS3231_ReadAlarm1->DY_DT = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx) & 0x3F);// receive Day or Date
	//DS3231_ReadAlarm1->Date = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx) & 0x7F);// receive Date

	I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);// generate not acknowledge
	I2C_AcknowledgeConfig(I2Cx, DISABLE);// disable acknowledge
	DS3231_GenerateStop(I2Cx);// generate stop condition
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge
};

void DS3231_SetAlarm2(I2C_TypeDef* I2Cx, struct DS3231_Alarm2 *DS3231_WriteAlarm2)
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge

	DS3231_GenerateStart(I2Cx);// send start condition
	DS3231_SendAddressWrite(I2Cx);// send address DS3231(address: 0xD0)

	DS3231_SendData(I2Cx, 0x0B);// send WORD ADDRESS 7
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_WriteAlarm2->Minutes));// send Minutes
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_WriteAlarm2->Hour));// send Hour
	DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_WriteAlarm2->DY_DT));// send Day
	//DS3231_SendData(I2Cx, DS3231_DecToBCD(DS3231_WriteAlarm2->Date));// send Date

	DS3231_GenerateStop(I2Cx);
};
void DS3231_GetAlarm2(I2C_TypeDef* I2Cx, struct DS3231_Alarm2 *DS3231_ReadAlarm2)
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge
	DS3231_GoToAddress(I2Cx, 0x0B);
	DS3231_GenerateStart(I2Cx);// send start condition
	DS3231_SendAddressRead(I2Cx);// send address DS3231(address: 0xD0)

	DS3231_ReadAlarm2->Minutes = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx) & 0x7F);// receive Month
	DS3231_ReadAlarm2->Hour = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx) & 0x3F);// receive Hour
	DS3231_ReadAlarm2->DY_DT = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx) & 0x3F);// receive Day or date
	//DS3231_ReadAlarm2->Date = DS3231_BCDToDec(DS3231_ReceiveData(I2Cx) & 0x7F);// receive Date

	I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);// generate not acknowledge
	I2C_AcknowledgeConfig(I2Cx, DISABLE);// disable acknowledge
	DS3231_GenerateStop(I2Cx);// generate stop condition
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge
};

uint8_t DS3231_GetFlag(I2C_TypeDef* I2Cx, uint8_t DS3231_FLAG_STATUS)
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge
	DS3231_GoToAddress(I2Cx, DS3231_ADDRESS_STATUS_REGISTER);
	DS3231_GenerateStart(I2Cx);// send start condition
	DS3231_SendAddressRead(I2Cx);// send address DS3231(address: 0xD0)

	uint8_t calculate = DS3231_ReceiveData(I2Cx) & DS3231_FLAG_STATUS;// read Flag status

	I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	DS3231_GenerateStop(I2Cx);
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge

	return calculate;
};
void DS3231_ClearFlag(I2C_TypeDef* I2Cx, uint8_t DS3231_FLAG_STATUS)
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge
	DS3231_GoToAddress(I2Cx, DS3231_ADDRESS_STATUS_REGISTER);
	DS3231_GenerateStart(I2Cx);// send start condition
	DS3231_SendAddressWrite(I2Cx);// send address DS3231(address: 0xD0)

	DS3231_SendData(I2Cx, DS3231_ADDRESS_STATUS_REGISTER);// go to address status
	I2C_SendData(I2Cx, ~DS3231_FLAG_STATUS);// send FLAG bit for clear bit
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));// wait send address
	DS3231_GenerateStop(I2Cx);
};

uint8_t DS3231_GetCotrolRegister(I2C_TypeDef* I2Cx)
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge

	DS3231_GoToAddress(I2Cx, DS3231_ADDRESS_CONTROL_REGISTER);// go to address 0x0E
	DS3231_GenerateStart(I2Cx);// generate start condition
	DS3231_SendAddressRead(I2Cx);// send address DS3231(address: 0xD0)

	uint8_t readCtrlReg = DS3231_ReceiveData(I2Cx);// read control register

	I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);// create not acknowledge
	I2C_AcknowledgeConfig(I2Cx, DISABLE);// disable acknowledge
	DS3231_GenerateStop(I2Cx);// generate stop condition
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge

	return readCtrlReg;
};
void DS3231_SetCtrlRegister(I2C_TypeDef* I2Cx, uint8_t DS3231_FLAG_CONTROL)
{
	DS3231_FLAG_CONTROL = DS3231_GetCotrolRegister(I2Cx) | DS3231_FLAG_CONTROL;

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge

	DS3231_GenerateStart(I2Cx);// generate start condition
	DS3231_SendAddressWrite(I2Cx);// send address DS3231(address: 0xD0)
	DS3231_SendData(I2Cx, DS3231_ADDRESS_CONTROL_REGISTER);// send word address 0x0E
	DS3231_SendData(I2Cx, DS3231_FLAG_CONTROL);// send data for control register
	DS3231_GenerateStop(I2Cx);
};

void DS3231_SendData(I2C_TypeDef* I2Cx, uint8_t data)
{
	I2C_SendData(I2Cx, data);// send a byte
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));// wait send a byte
};
uint8_t DS3231_ReceiveData(I2C_TypeDef* I2Cx)
{
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));// wait receive a byte
	return I2C_ReceiveData(I2Cx);
};

void DS3231_SendAddressWrite(I2C_TypeDef* I2Cx)
{
	I2C_Send7bitAddress(I2Cx, 0xD0, I2C_Direction_Transmitter);// send Address DS3231(0xD0)
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));// wait send address
};
void DS3231_SendAddressRead(I2C_TypeDef* I2Cx)
{
	I2C_Send7bitAddress(I2Cx, 0xD0, I2C_Direction_Receiver);// send Address DS3231(0xD0)
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));// wait send address
};

void DS3231_GenerateStart(I2C_TypeDef* I2Cx)
{
	I2C_GenerateSTART(I2Cx, ENABLE);
	while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB));// wait generate start
};
void DS3231_GenerateStop(I2C_TypeDef* I2Cx)
{
	I2C_GenerateSTOP(I2Cx, ENABLE);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));// wait generate stop
};

uint8_t DS3231_DecToBCD(uint8_t value)
{
	return ((value / 10) << 4) + (value % 10);// convert decimal to BCD
};
uint8_t DS3231_BCDToDec(uint8_t value)
{
	return ((value >> 4) * 10) + (value & 0x0F);// convert BCD(Binary Coded Decimal) to Decimal
};

void DS3231_GoToAddress(I2C_TypeDef* I2Cx, uint8_t AddressByte)
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx, ENABLE);// enable acknowledge

	DS3231_GenerateStart(I2Cx);// send start condition
	DS3231_SendAddressWrite(I2Cx);// send address DS3231(address: 0xD0)
	DS3231_SendData(I2Cx, AddressByte);// send WORD ADDRESS
	DS3231_GenerateStop(I2Cx);
};
/******** End Of File ***********/
