

#ifndef _IO_H_
#define _IO_H_

#include "GPIOLIKE51.h"
#include <stm32f10x.h>
#include "stm32f10x_gpio.h"

#define LED_STATUS 	 PBout(8)

#define LED_ON  (LED_STATUS = 0)
#define LED_OFF (LED_STATUS = 1)

#define CPLD_EN 		 PBin(0)
#define CPLD_PARM_A  PBin(12)
#define CPLD_PARM_B  PBin(13)
#define CPLD_PARM_C  PBin(14)

#define CPLD_VFD_A   PBout(3)
#define CPLD_VFD_B   PBout(4)
#define CPLD_VFD_C   PBout(5)

#define CPLD_VFD_GPIO GPIOB
#define CPLD_VFD_A_PIN 3 
#define CPLD_VFD_B_PIN 4
#define CPLD_VFD_C_PIN 5

#define CPLD_VFD_A_HIGH  GPIO_SetBits(CPLD_VFD_GPIO,GPIO_Pin_3)
#define CPLD_VFD_A_LOW   GPIO_ResetBits(CPLD_VFD_GPIO,GPIO_Pin_3)

#define CPLD_VFD_B_HIGH  GPIO_SetBits(CPLD_VFD_GPIO,GPIO_Pin_4)
#define CPLD_VFD_B_LOW   GPIO_ResetBits(CPLD_VFD_GPIO,GPIO_Pin_4)

#define CPLD_VFD_C_HIGH  GPIO_SetBits(CPLD_VFD_GPIO,GPIO_Pin_5)
#define CPLD_VFD_C_LOW   GPIO_ResetBits(CPLD_VFD_GPIO,GPIO_Pin_5)

//#define CPLD_DA_DIN  PAout(8)
//#define CPLD_DA_CLK  PAout(9)
//#define CPLD_DA_CS   PAout(10)

#define PWM_OE       PAout(11)
#define PWM_CFG_CS   PAout(12)
#define PWM_CFG_SCL  PBout(10)
#define PWM_CFG_SDA  PBout(11)

#define CPLD_PWM_SD  PBout(15)

#define BUCK_ON  (CPLD_PWM_SD = 1)
#define BUCK_OFF (CPLD_PWM_SD = 0)

extern u8 CpldEN;

void CPLD_GPIO_Config(void);  
void SetCpldPwmPara(uint16_t time,uint8_t timesOfLow);

#endif
