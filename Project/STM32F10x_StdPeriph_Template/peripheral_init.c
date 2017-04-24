
#include "peripheral_init.h"




void USART2_GPIO_Configuration(void);
void USART2_Configuration(void);
void NVIC_Configuration(void);




//////////
void Peripheral_Init()
{
	NVIC_Configuration();
}

void NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
#ifdef VECT_TAB_RAM
	// Set the Vector Table base location at 0x20000000 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else /* VECT_TAB_FLASH */
	// Set the Vector Table base location at 0x08000000 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

	//TIM2
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	//UART2 interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void func_void()
{
	//諾滲杅
}
void USART2_GPIO_Configuration()
{
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); 
	//????Tx?aalternate push-pull 
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIOInitStructure);

	//????USART2_Rx ?ainput floating
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIOInitStructure);
}
void USART2_Configuration()
{
	USART_InitTypeDef USARTInitStructure;
	USART_ClockInitTypeDef USARTClockInitStructure;
	
	USART2_GPIO_Configuration();

	USARTClockInitStructure.USART_Clock = USART_Clock_Disable;
	USARTClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USARTClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USARTClockInitStructure.USART_LastBit = USART_LastBit_Disable;

	USART_ClockInit(USART2,&USARTClockInitStructure);

	USARTInitStructure.USART_BaudRate = 115200;
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USARTInitStructure.USART_StopBits = USART_StopBits_1;
	USARTInitStructure.USART_Parity = USART_Parity_No;
	USARTInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2,&USARTInitStructure);
	//那1?邦USART1?車那邦?D??
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	//那1?邦USART1
	USART_Cmd(USART2,ENABLE);
}

void USART2PutByte(u8 data)
{
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{}
	USART_SendData(USART2, data);
}



