

#include "io.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

u8 CpldEN = 0;
void CPLD_GPIO_Config(void)  
{
  GPIO_InitTypeDef GPIO_InitStructure;  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 		//初始化别忘了时钟		
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 		//初始化别忘了时钟			 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8|
							GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|
								GPIO_Pin_15|GPIO_Pin_8|
								GPIO_Pin_10|GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_8|
											GPIO_Pin_11|GPIO_Pin_12;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	CPLD_PWM_SD = 1;
	LED_OFF;
		
}

//#define PWM_OE       PAout(11)
//#define PWM_CFG_CS   PAout(12)
//#define PWM_CFG_SCL  PBout(10)
//#define PWM_CFG_SDA  PBout(11)
#define DENOMINATOR 100
#define RESERVED_BIT 0

u8 CpldPwmEnable = 1;
void SetCpldPwmPara(uint16_t time,uint8_t timesOfLow)
{
	unsigned char i;
	static u16 time_last;
	static u8 timesOfLow_last;
	if(CpldEN > 0)
		CPLD_PWM_SD = 0;
	else
		CPLD_PWM_SD = 1;
	
	if((time == time_last) && (timesOfLow == timesOfLow_last))
		return;
	else
	{
		time_last = time;
		timesOfLow_last = timesOfLow;
	}
	
	PWM_OE = 0;
	PWM_CFG_CS = 0;
	PWM_CFG_SCL = 0;
	PWM_CFG_SDA = 0;

	PWM_CFG_CS = 1;

	for(i = 0; i < 16; i++) 
	{ 				
		int temp = (time & 0x8000) ? 1 : 0;	
		PWM_CFG_SCL = 0;	
		PWM_CFG_SDA = temp;
		delay_us(100);
		PWM_CFG_SCL = 1; 
		time <<= 1; 	
		delay_us(100);
	}

	for(i = 0;i<3;++i)
	{
		int temp = RESERVED_BIT;
		PWM_CFG_SCL = 0;	
		PWM_CFG_SDA = temp;
		delay_us(100);
		PWM_CFG_SCL = 1; 
		delay_us(100);
	}
	
	timesOfLow = timesOfLow >= 16 ? 16 : timesOfLow;
	for(i = 0;i<8;++i)
	{
		int temp = (timesOfLow & 0x10) ? 1 : 0;
		PWM_CFG_SCL = 0;	
		PWM_CFG_SDA = temp;
		delay_us(100);
		PWM_CFG_SCL = 1; 
		timesOfLow <<= 1; 	
		delay_us(100);
	}
	
	PWM_CFG_CS = 0;
	PWM_OE = 1;
}


