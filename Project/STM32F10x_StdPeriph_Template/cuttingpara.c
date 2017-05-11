
#include "cuttingpara.h"
#include "SystemCore.h"
#include "EEPROM_24c0x.h"


CuttingPara_t CuttingParaSave[8] = 
{
	{0,2,50,2000,1},
  {1,2,50,2000,1},
	{2,3,50,2000,1},
	{3,3,50,2000,1},
	{4,4,30,2000,1},
	{5,4,20,2000,1},
	{6,5,15,2000,1},
	{7,5,30,2000,1}
};

CuttingPara_t CuttingParaCom = {8,8,50,1900,2};

int CuttingNumber = 0;

uint8_t Read8421(void)
{
	uint8_t u8Temp = 0;
	(CPLD_PARM_A == 1) ? (BIT_SET(u8Temp,0)):(BIT_CLEAN(u8Temp,0));  
	(CPLD_PARM_B == 1) ? (BIT_SET(u8Temp,1)):(BIT_CLEAN(u8Temp,1));  
	(CPLD_PARM_C == 1) ? (BIT_SET(u8Temp,2)):(BIT_CLEAN(u8Temp,2));  
	return u8Temp;
}

void Write8421(int8_t num)
{
	num = (uint8_t)RANGE(num,0,7);
	BIT_GET(num,0) == 1 ? CPLD_VFD_A_HIGH : CPLD_VFD_A_LOW;
  BIT_GET(num,1) == 1 ? CPLD_VFD_B_HIGH : CPLD_VFD_B_LOW;
	BIT_GET(num,2) == 1 ? CPLD_VFD_C_HIGH : CPLD_VFD_C_LOW;
}


void Write8421FromRotateSpeed(uint8_t speed)
{
	static uint8_t speed_last;
	uint8_t speed_n = 6;
	if(speed_last == speed)
		return;
	else
		speed_last = speed;
	switch(speed)
	{
		case 10:speed_n = 6;break; //10Hz 0b110
		case 15:speed_n = 5;break; //15Hz 0b101 
		case 20:speed_n = 4;break; //20Hz 0b100 
		case 25:speed_n = 3;break; //25Hz 0b011 
		case 30:speed_n = 2;break; //30Hz 0b010
		case 40:speed_n = 1;break; //40Hz 0b001
		case 50:speed_n = 0;break; //50Hz 0b000
		default:speed_n = 6;break; //10Hz 0b110
	}
	Write8421(speed_n);
}

void SaveDataToEEPROM(CuttingPara_t* This, uint8_t writeAddr)
{
	uint8_t bytes[8];
	bytes[0] = This->Number;
	bytes[1] = This->TimeOfLow;
	bytes[2] = This->RotateSpeed;
	bytes[3] = (uint8_t)This->PulseWidthNum;
	bytes[4] = (uint8_t)This->PulseWidthNum >> 8;
	bytes[5] = (uint8_t)This->Current;
	bytes[6] = (uint8_t)This->Current >> 8;
	I2C_EE_BufferWrite(bytes,writeAddr,8);
}

void ReadDataFromEEPROM(CuttingPara_t* This, uint8_t readAddr)
{
	uint8_t bytes[8];
	I2C_EE_BufferRead(bytes,readAddr,8);
	This->Number = bytes[0];
	This->TimeOfLow = bytes[1];
	This->RotateSpeed = bytes[2];
	This->PulseWidthNum = bytes[3] + ((uint16_t)bytes[4] << 8);
	This->Current = bytes[5] + ((uint16_t)bytes[6] << 8);
}

void SaveAllDataToEEPROM(void)
{
	int i = 0;
	for(; i < ARR_SIZE(CuttingParaSave) ;++i)
	{
		uint8_t addr = i * 10;
		SaveDataToEEPROM(&(CuttingParaSave[i]),addr);
	}
}

void ReadAllDataFormEEPROM(void)
{
	int i = 0;
	for(; i < ARR_SIZE(CuttingParaSave) ;++i)
	{
		uint8_t addr = i * 10;
		ReadDataFromEEPROM(&(CuttingParaSave[i]),addr);
	}
}

bool CuttingPara_Equals(CuttingPara_t* left,CuttingPara_t* right)
{
	if(left->Number == right->Number
		&& left->TimeOfLow == right->TimeOfLow
	  && left->RotateSpeed == right->RotateSpeed
	  && left->PulseWidthNum == right->PulseWidthNum
	  && left->Current == right->Current)
		return true;
	return false;
}

