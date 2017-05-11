/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>
#include "peripheral_init.h"
#include "LowerComputer_V2.h"
#include "tim.h"
#include "pll.h"
#include "io.h"
#include "tlc5615.h"
#include "EEPROM_24c0x.h"
#include "Config.h"
#include "ad.h"
#include "mb.h"
#include "MyModbusData.h"
#include "cuttingpara.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Public variables ---------------------------------------------------------*/

s16 temp1,temp2,temp3;
uint32_t SetDA = 1;
float SetDA_A = 0.02;
uint16_t PWMTime_us = 20;
uint16_t PWMTime = 50;
uint8_t TimesOfLow = 7;
uint8_t RotateHz = 40;
/* Private function prototypes -----------------------------------------------*/
void USART2_GPIO_Configuration(void);
void USART2_Configuration(void);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */


int main(void)
{
	PLL_Configuration();      //��Ƶ��72M
	Peripheral_Init();        //NVIC Init
	delay_init();             //��ʱ������ʼ��
	DMA_ADC1_Init();          //DMA ADC��ʼ��
	CPLD_GPIO_Config();       //��CPLD���ӵ�IO�ڳ�ʼ��
	I2C_EE_Init();            //EEPROM 24C08��ʼ��
	TIM3_Init();              //TIM3 ��ʼ��
	ReadAllDataFormEEPROM();
	SetCpldPwmPara(50,2);     //�趨��Ƶ������,����
	#ifndef ModbusEnable
	USART2_Configuration();
	HIT_LC_Init();
	#else
	eMBInit( MB_RTU, 0x01, 0, 9600, MB_PAR_NONE );
	eMBEnable( );
	#endif
	while (1)
	{	
		temp2++;
		ADCdeal();
		SetDA = SetDA_A * 25;   
		DAConvert(SetDA);     //��DAоƬtlc5615����
	  PWMTime = PWMTime_us * 50;	
	  SetCpldPwmPara(PWMTime,TimesOfLow);
		Write8421FromRotateSpeed(RotateHz);//����Ƶ������
		#ifndef ModbusEnable  //ʹ��Qin ��λ��
		LC_SendData();
		LC_ReceivedData();
		#else                 //ʹ��modbus
		MyModbusBindingToHoldReg();
		(void)eMBPoll();
		MyModbusDataFromHoldReg();
		#endif
	  delay_ms(40);
	}
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
