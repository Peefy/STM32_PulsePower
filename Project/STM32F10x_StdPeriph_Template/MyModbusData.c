
#include <stdint.h>
#include "MyModbusData.h"
#include "mb.h"
#include "cuttingpara.h"

#define REG_HOLDING_START 0x0000
#define REG_HOLDING_NREGS 8

uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {0x147b,0x3f8e,
0x147b,0x400e,0x1eb8,0x4055,0x147b,0x408e};

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    /* error state */
    eMBErrorCode    eStatus = MB_ENOERR;
    /* offset */
    int16_t iRegIndex;
    
    /* test if the reg is in the range */
    if (((int16_t)usAddress-1 >= REG_HOLDING_START) 
        && (usAddress-1 + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        /* compute the reg's offset */
        iRegIndex = (int16_t)(usAddress-1 - REG_HOLDING_START);
        switch (eMode)
        {
            case MB_REG_READ:
                while (usNRegs > 0)
                {
                    *pucRegBuffer++ = (uint8_t)( usRegHoldingBuf[iRegIndex] >> 8 );
                    *pucRegBuffer++ = (uint8_t)( usRegHoldingBuf[iRegIndex] & 0xff);
                    iRegIndex ++;
                    usNRegs --;
                }
                break;
            case MB_REG_WRITE:
                while (usNRegs > 0)
                {
                    usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                    usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                    iRegIndex ++;
                    usNRegs --;
                }
                break;
                
        }
    }
    else{
        eStatus = MB_ENOREG;
    }
    
    return eStatus;
    
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
	static uint8_t flag = 0;
	
	flag == 0 ? GPIO_SetBits(GPIOC,GPIO_Pin_1) : GPIO_ResetBits(GPIOC,GPIO_Pin_1);  
	flag ^= 1;
    return eStatus;
}

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    static uint8_t flag = 0;
	
	flag == 0 ? GPIO_SetBits(GPIOC,GPIO_Pin_3) : GPIO_ResetBits(GPIOC,GPIO_Pin_3);  
	flag ^= 1;
	return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    static uint8_t flag = 0;
	
	flag == 0 ? GPIO_SetBits(GPIOC,GPIO_Pin_4) : GPIO_ResetBits(GPIOC,GPIO_Pin_4);  
	flag ^= 1;
	return MB_ENOREG;
}

extern float SetDA_A;
extern uint16_t PWMTime_us;
extern uint8_t TimesOfLow ;
extern uint8_t RotateHz;

typedef union 
{
	float Float;
	u16 UShort16[2];
}Float2Uint16;

uint16_t mbTemp;

#define A1 mbTemp
#define A2 CuttingParaCom.PulseWidthNum
#define A3 CuttingParaCom.Current //setda_A high16
#define A4 mbTemp //setda_A low16
#define A5 mbTemp
#define A6 mbTemp
#define A7 mbTemp
#define A8 mbTemp

void MyModbusBindingToHoldReg(void)
{
	
	uint8_t u8Temp = (uint8_t)((CuttingParaCom.Number << 4) + CuttingParaCom.TimeOfLow);
	uint16_t u16Temp = ((uint16_t)u8Temp << 8) + CuttingParaCom.RotateSpeed;
	usRegHoldingBuf[0] = u16Temp;
  usRegHoldingBuf[1] = A2;
	usRegHoldingBuf[2] = A3;
	usRegHoldingBuf[3] = A4;
	usRegHoldingBuf[4] = A5;
	usRegHoldingBuf[5] = A6;
	usRegHoldingBuf[6] = A7;
	usRegHoldingBuf[7] = A8;
}


void MyModbusDataFromHoldReg(void)
{	
	uint8_t  u8Temp;
  uint16_t u16Temp;
	u16Temp = usRegHoldingBuf[0] ;
	A2 = usRegHoldingBuf[1] ;
	A3 = usRegHoldingBuf[2] ;
	A4 = usRegHoldingBuf[3] ;
	A5 = usRegHoldingBuf[4] ;
	A6 = usRegHoldingBuf[5] ;
	A7 = usRegHoldingBuf[6] ;
	A8 = usRegHoldingBuf[7] ;
	
  CuttingParaCom.RotateSpeed = (uint8_t)u16Temp;
	u8Temp = u16Temp >> 8;
	CuttingParaCom.Number = u8Temp >> 4;
	CuttingParaCom.TimeOfLow = u8Temp & 0x0f;
	
	if(CuttingParaCom.Number <= 7)
	{
		//if(CuttingPara_Equals(&(CuttingParaSave[CuttingParaCom.Number]),
		//	&CuttingParaCom) == false)
		//{
			CuttingParaSave[CuttingParaCom.Number] = CuttingParaCom;
		//	SaveDataToEEPROM(&CuttingParaCom,CuttingParaCom.Number * 10);
		//}		
		//SetDA_A = CuttingParaSave[CuttingNumber].Current / 10.0f;
		//TimesOfLow = CuttingParaSave[CuttingNumber].TimeOfLow;
		//PWMTime_us = CuttingParaSave[CuttingNumber].PulseWidthNum * 20 /1000;
		//RotateHz = CuttingParaSave[CuttingNumber].RotateSpeed;
		SetDA_A = CuttingParaCom.Current / 10.0f;
		TimesOfLow = CuttingParaCom.TimeOfLow;
		PWMTime_us = CuttingParaCom.PulseWidthNum * 20 /1000;
		RotateHz = CuttingParaCom.RotateSpeed;
	}
	else if(CuttingParaCom.Number == 8)
	{
		SetDA_A = CuttingParaCom.Current / 10.0f;
		TimesOfLow = CuttingParaCom.TimeOfLow;
		PWMTime_us = CuttingParaCom.PulseWidthNum * 20 /1000;
		RotateHz = CuttingParaCom.RotateSpeed;
	}
}



