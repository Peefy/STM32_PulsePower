
#include <stdint.h>
#include "MyModbusData.h"
#include "mb.h"


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

typedef union 
{
	float Float;
	u16 UShort16[2];
}Float2Uint16;

uint16_t mbTemp;

#define A1 PWMTime_us
#define A2 TimesOfLow
#define A3 mbTemp //setda_A high16
#define A4 mbTemp //setda_A low16
#define A5 mbTemp
#define A6 mbTemp
#define A7 mbTemp
#define A8 mbTemp

void MyModbusBindingToHoldReg(void)
{
	Float2Uint16 float2Uint16;
	float2Uint16.Float = SetDA_A;
	
	usRegHoldingBuf[0] = A1;
  usRegHoldingBuf[1] = A2;
	usRegHoldingBuf[2] = float2Uint16.UShort16[0];
	usRegHoldingBuf[3] = float2Uint16.UShort16[1];
	usRegHoldingBuf[4] = A5;
	usRegHoldingBuf[5] = A6;
	usRegHoldingBuf[6] = A7;
	usRegHoldingBuf[7] = A8;
}


void MyModbusDataFromHoldReg(void)
{	
	Float2Uint16 float2Uint16;
	float2Uint16.Float = SetDA_A;
	
	A1 = usRegHoldingBuf[0] ;
	A2 = usRegHoldingBuf[1] ;
	float2Uint16.UShort16[0] = usRegHoldingBuf[2] ;
	float2Uint16.UShort16[1] = usRegHoldingBuf[3] ;
	A5 = usRegHoldingBuf[4] ;
	A6 = usRegHoldingBuf[5] ;
	A7 = usRegHoldingBuf[6] ;
	A8 = usRegHoldingBuf[7] ;
	
	SetDA_A = float2Uint16.Float;
	
}



