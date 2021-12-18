#ifndef _USART_H
#define _USART_H

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "stm32f10x_usart.h"


void Rcc_Configuration(int numPort);
void UsartGPIO_Configuration(int numPort);
void usart_Configuration(USART_TypeDef* USARTx);
//void UsartGPIO_CTRT_Configuration(void);
//void USART_CTRT_Configuartion(void);

#endif /*_USART_H*/
