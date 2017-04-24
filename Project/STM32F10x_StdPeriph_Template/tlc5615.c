
#include "tlc5615.h"
#include "delay.h"
#include "stm32f10x_gpio.h"
#include "SystemCore.h"
#include "mb.h"

#define CPLD_DA_DIN  PAout(8)
#define CPLD_DA_CLK  PAout(9)
#define CPLD_DA_CS   PAout(10)

void TLC5615_GpioConfig()
{
	GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 		//初始化别忘了时钟			 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_8;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DAConvert(unsigned int daValue)
{
	unsigned char i;
	static int daValueLast;
	daValue = RANGE(daValue,1,1023);
	if(daValue == daValueLast)
		return;
	else
		daValueLast = daValue;
	daValue <<= 2;
	CPLD_DA_CS = 0;
	CPLD_DA_CLK = 0;	
	for(i = 0; i < 12; i++) 
	{ 				
		int temp = (daValue & 0x0800) ? 1 : 0;	
		
		CPLD_DA_DIN = temp;
		delay_us(800);
		CPLD_DA_CLK = 1; 
		daValue <<= 1; 	
		delay_us(800);
		CPLD_DA_CLK = 0;	
		delay_us(800);
	}

	CPLD_DA_CS = 1; 
	CPLD_DA_CLK = 0;	
}


