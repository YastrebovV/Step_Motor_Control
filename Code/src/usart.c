#include "usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void Rcc_Configuration(int numPort)
{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	switch (numPort) 
{
		case 4:{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
		} break;
    case 5:{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
		} break;
}
}

void UsartGPIO_Configuration(int numPort)
{
	switch (numPort) 
{
		case 4:{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;	
	GPIO_Init(GPIOC, &GPIO_InitStruct);}break;
			case 5:{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;	
	GPIO_Init(GPIOD, &GPIO_InitStruct);}break;
}
}

void usart_Configuration(USART_TypeDef* USARTx)
{
	USART_InitTypeDef USART_InitStruct;
	//NVIC_InitTypeDef  NVIC_InitStructure;

	int num;
	
	if (USARTx == UART4)
	 {
			Rcc_Configuration(4);
		  num = 4;
	 }else if (USARTx == UART5)
   {
			Rcc_Configuration(5);
		  num = 5;
   }
		 
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USARTx, &USART_InitStruct);
//	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);	
 // USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);	 
//	NVIC_EnableIRQ(UART5_IRQn);
	//USART_Cmd(USARTx, ENABLE);
	
	USART_ITConfig(USARTx,USART_IT_RXNE,ENABLE);
	USART_Cmd(USARTx, ENABLE);
	 
	UsartGPIO_Configuration(num);
}

void UsartGPIO_CTRT_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_12;/*GPIO_Pin_1 | GPIO_Pin_2;*/
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;/*GPIO_Pin_0 | GPIO_Pin_3;*/
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void USART_CTRT_Configuartion(void)
{
	USART_InitTypeDef USART_InitStruct;

	Rcc_Configuration(1);

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStruct);

	USART_Cmd(USART1, ENABLE);

	UsartGPIO_CTRT_Configuration();
}


#if 1 //USE microLIB
#pragma import(__use_no_semihosting)
struct __FILE  
{  
	int handle;  
};  
FILE __stdout;  

void _sys_exit(int x)  
{  
	x = x;  
}
#endif

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1,(u8)ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

	return ch;
}
