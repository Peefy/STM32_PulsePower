
#ifndef _EEPROM24C0X_H_
#define _EEPROM24C0X_H_

#include <stm32f10x.h>
#include <string.h>
#include <stdbool.h>

void I2C_EE_Init(void);
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
void I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr);
void I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void);
bool EEPROM_Test(void);

#endif
