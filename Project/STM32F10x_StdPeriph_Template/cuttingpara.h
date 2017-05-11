#ifndef __CUTTINGPARA_H
#define __CUTTINGPARA_H 		

#include "stdint.h"
#include "stdbool.h"
#include "stm32f10x_gpio.h"
#include "io.h"

typedef 
struct
{
   uint8_t Number;
	 uint8_t TimeOfLow;
	 uint8_t RotateSpeed;
	 uint16_t PulseWidthNum;
	 uint16_t Current;
}CuttingPara_t;

extern CuttingPara_t CuttingParaSave[8];
extern CuttingPara_t CuttingParaCom;
extern int CuttingNumber;

uint8_t Read8421(void);
void Write8421(int8_t num);
void Write8421FromRotateSpeed(uint8_t speed);
void SaveDataToEEPROM(CuttingPara_t* This, uint8_t writeAddr);
void ReadDataFromEEPROM(CuttingPara_t* This, uint8_t readAddr);
void SaveAllDataToEEPROM(void);
void ReadAllDataFormEEPROM(void);
bool CuttingPara_Equals(CuttingPara_t* left,CuttingPara_t* right);

#endif
