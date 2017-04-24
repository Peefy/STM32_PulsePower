/*************************************************************
#include "UserCustom.h"
Last Write Time: 2017-04-13  11:06:36
***************************************************************/

#ifndef USER_CUSTOM_H
#define USER_CUSTOM_H

/**************** Includes ****************************************/
#include "stm32f10x.h"
#include "peripheral_init.h"

/****************** Defines ********************************/
#define LC_IS_LITTLE_ENDIAN
////////Variables Type Defines//////// (Change These by Your Program Requirements!)
#define LC_Uint8  u8
#define LC_Uint16 u16
#define LC_Uint32 u32
#define LC_Int8   s8
#define LC_Int16  s16
#define LC_Int32  s32

///<The Datas to Send>//// 
#define S1_CUSTOM1     temp2
#define TYPE_S1_C1     LC_Int16
#define S1_CUSTOM2     temp3
#define TYPE_S1_C2     LC_Int16
//#define S1_CUSTOM3     
//#define TYPE_S1_C3_DONOT_SEND
#define S1_CUSTOM4     SetDA
#define TYPE_S1_C4     LC_Uint32
#define S1_CUSTOM5     PWMTime
#define TYPE_S1_C5     LC_Uint16
#define S1_CUSTOM6     TimesOfLow
#define TYPE_S1_C6     LC_Uint8
#define S1_CUSTOM7     CpldPwmEnable
#define TYPE_S1_C7     LC_Uint8
#define S1_CUSTOM8     AD_Value[0]
//#define TYPE_S1_C8     LC_Uint16
#define S1_CUSTOM9     CpldEN
#define TYPE_S1_C9     LC_Uint8
#define S1_CUSTOM10     SetDA_A
#define TYPE_S1_C10_FLOAT_DEC
#define TYPE_S1_C10_FLOAT
#define S1_CUSTOM11     PWMTime_us
#define TYPE_S1_C11     LC_Uint16
//#define S1_CUSTOM12     
//#define TYPE_S1_C12_DONOT_SEND
//#define S1_CUSTOM13     
//#define TYPE_S1_C13_DONOT_SEND
//#define S1_BIT76        
//#define TYPE_S1_BIT76_DONOT_SEND
//#define S1_BIT5        
//#define TYPE_S1_BIT5_DONOT_SEND
//#define S1_BIT4        
//#define TYPE_S1_BIT4_DONOT_SEND
//#define S1_BIT3        
//#define TYPE_S1_BIT3_DONOT_SEND
//#define S1_BIT2        
//#define TYPE_S1_BIT2_DONOT_SEND
//#define S1_BIT1        
//#define TYPE_S1_BIT1_DONOT_SEND
//#define S1_BIT0        
//#define TYPE_S1_BIT0_DONOT_SEND

//#define S2_CUSTOM1     
//#define TYPE_S2_C1_DONOT_SEND
//#define S2_CUSTOM2     
//#define TYPE_S2_C2_DONOT_SEND
//#define S2_CUSTOM3     
//#define TYPE_S2_C3_DONOT_SEND
//#define S2_CUSTOM4     
//#define TYPE_S2_C4_DONOT_SEND
//#define S2_CUSTOM5     
//#define TYPE_S2_C5_DONOT_SEND
//#define S2_CUSTOM6     
//#define TYPE_S2_C6_DONOT_SEND
//#define S2_CUSTOM7     
//#define TYPE_S2_C7_DONOT_SEND
//#define S2_CUSTOM8     
//#define TYPE_S2_C8_DONOT_SEND
//#define S2_CUSTOM9     
//#define TYPE_S2_C9_DONOT_SEND
//#define S2_CUSTOM10     
//#define TYPE_S2_C10_DONOT_SEND
//#define S2_CUSTOM11     
//#define TYPE_S2_C11_DONOT_SEND
//#define S2_CUSTOM12     
//#define TYPE_S2_C12_DONOT_SEND
//#define S2_CUSTOM13     
//#define TYPE_S2_C13_DONOT_SEND
//#define S2_BIT76        
//#define TYPE_S2_BIT76_DONOT_SEND
//#define S2_BIT5        
//#define TYPE_S2_BIT5_DONOT_SEND
//#define S2_BIT4        
//#define TYPE_S2_BIT4_DONOT_SEND
//#define S2_BIT3        
//#define TYPE_S2_BIT3_DONOT_SEND
//#define S2_BIT2        
//#define TYPE_S2_BIT2_DONOT_SEND
//#define S2_BIT1        
//#define TYPE_S2_BIT1_DONOT_SEND
//#define S2_BIT0        
//#define TYPE_S2_BIT0_DONOT_SEND


///<The Datas Received> ////(Change These by Your Program Requirements!)
#define R1_CUSTOM1      temp2
#define TYPE1_R1      LC_Int16
#define R1_CUSTOM2      SetDA_A
#define TYPE1_R2_FLOAT_DEC
#define TYPE1_R2_FLOAT
#define R1_CUSTOM3      PWMTime_us
#define TYPE1_R3      LC_Uint16
#define R1_CUSTOM4      TimesOfLow
#define TYPE1_R4      LC_Uint8
#define R1_CUSTOM5      CpldPwmEnable
#define TYPE1_R5      LC_Uint8
//#define R1_CUSTOM6      
//#define TYPE1_R6_DONOT_SEND
//#define R1_CUSTOM7      
//#define TYPE1_R7_DONOT_SEND
#define NUM_R1_BYTE   12

//#define R2_CUSTOM1      
//#define TYPE2_R1_DONOT_SEND
//#define R2_CUSTOM2      
//#define TYPE2_R2_DONOT_SEND
//#define R2_CUSTOM3      
//#define TYPE2_R3_DONOT_SEND
//#define R2_CUSTOM4      
//#define TYPE2_R4_DONOT_SEND
//#define R2_CUSTOM5      
//#define TYPE2_R5_DONOT_SEND
//#define R2_CUSTOM6      
//#define TYPE2_R6_DONOT_SEND
//#define R2_CUSTOM7      
//#define TYPE2_R7_DONOT_SEND
#define NUM_R2_BYTE   0

//#define R3_CUSTOM1      
//#define TYPE3_R1_DONOT_SEND
//#define R3_CUSTOM2      
//#define TYPE3_R2_DONOT_SEND
//#define R3_CUSTOM3      
//#define TYPE3_R3_DONOT_SEND
//#define R3_CUSTOM4      
//#define TYPE3_R4_DONOT_SEND
//#define R3_CUSTOM5      
//#define TYPE3_R5_DONOT_SEND
//#define R3_CUSTOM6      
//#define TYPE3_R6_DONOT_SEND
//#define R3_CUSTOM7      
//#define TYPE3_R7_DONOT_SEND
#define NUM_R3_BYTE   0

//#define R4_CUSTOM1      
//#define TYPE4_R1_DONOT_SEND
//#define R4_CUSTOM2      
//#define TYPE4_R2_DONOT_SEND
//#define R4_CUSTOM3      
//#define TYPE4_R3_DONOT_SEND
//#define R4_CUSTOM4      
//#define TYPE4_R4_DONOT_SEND
//#define R4_CUSTOM5      
//#define TYPE4_R5_DONOT_SEND
//#define R4_CUSTOM6      
//#define TYPE4_R6_DONOT_SEND
//#define R4_CUSTOM7      
//#define TYPE4_R7_DONOT_SEND
#define NUM_R4_BYTE   0


////////Other  Declarations /////////
extern u16 AD_Value[2];


////Function Defines ////(Change These by Your Program Requirements!)
#define f_COM_INIT         func_void()   //Serial Port Initialize，格式要求：返回值为void，没有形参
#define f_SEND_BYTE(data)  USART2PutByte(data)   // 串 口 发 送 函 数，格式要求：返回值为void，形参为一个无符号8位整型
#define COM_delay_ms(MS)   delay_ms(MS)   // 延 时 函 数，格式要求：返回值为void，形参为一个无符号16位整型

//#define CMD01_f   ()   // 函数返回值类型必须为void，并且没有形参
//#define CMD02_f   ()   // 函数返回值类型必须为void，并且没有形参
//#define CMD03_f   ()   // 函数返回值类型必须为void，并且没有形参
//#define CMD04_f   ()   // 函数返回值类型必须为void，并且没有形参


#endif

