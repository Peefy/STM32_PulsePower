#ifndef _AD_H_
#define _AD_H_

#include <stm32f10x.h>

#define MAX_AD 2

extern u16 ADCConvertedValue[MAX_AD];
extern u16 AD_Value[MAX_AD];

void DMA_ADC1_Init(void);
void ADCdeal(void);

#endif
