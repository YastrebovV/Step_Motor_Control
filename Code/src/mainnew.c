#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "usart.h"
#include "main.h"
#include "queue.h"
#include "semphr.h"

void vMotor_Task(void *pParams);
void vMotor_Task2(void *pParams);

//char _dataWiFi[32];
//char *_dataWiFi_Sep[3][4];
//char separator [10]="/";
//char *istr;
//int count = 0;
//double angle = 0;

#define RX_BUF_SIZE 80
volatile char RX_FLAG_END_LINE = 0;
volatile char RXi;
volatile char RXc;
char RX_BUF[RX_BUF_SIZE] = {'\0'};
volatile char buffer[80] = {'\0'};
int ON = 0;
 TaskHandle_t xHandle = NULL;
 
void clear_RXBuffer(void) {
    for (RXi=0; RXi<RX_BUF_SIZE; RXi++)
        RX_BUF[RXi] = '\0';
    RXi = 0;
}

void InitRCC(){
RCC_HSEConfig(RCC_HSE_ON); //Enable HSE
while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) ; //Waiting for HSE

//Set Flash latency

//FLASH->ACR |= FLASH_ACR_PRFTBE;
//FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
//FLASH->ACR |= (uint32_t)((uint8_t)0x02);

RCC_PREDIV1Config(RCC_PREDIV1_Source_HSE,RCC_PREDIV1_Div1);//PREDIV 1 Divider = 1
RCC_PLLConfig(RCC_PLLSource_PREDIV1,RCC_PLLMul_9);//Set PREDIV1 as source for PLL,And set PLLMUL=9
RCC_PLLCmd(ENABLE);//Enable PLL
while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) ;//Waiting for PLL

RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//Set PLL as SYSCLK Soucre

RCC_HSICmd(DISABLE);//Disable HSI
}

//********************************************
//USSART send
void USARTSend(const unsigned char *pucBuffer, USART_TypeDef* USARTx){
    while (*pucBuffer)
    {
        USART_SendData(USARTx, *pucBuffer++);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
        {
        }
    }
}
//********************************************
void vApplicationIdleHook( void )
{
	
}

void vApplicationTickHook( void )
{
	
}

void vApplicationStackOverflowHook( void )
{
	
}
unsigned long ulRunTimeStatsClock;


 void delay_init(void) {
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
   TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
   TIM_TimeBaseStructure.TIM_Prescaler = (SYS_CLK / DELAY_TIM_FREQUENCY) - 1;
   TIM_TimeBaseStructure.TIM_Period = UINT16_MAX;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
   TIM_Cmd(TIM2, ENABLE);
}
 
void delay_us(uint16_t uSecs) {
   uint16_t start = TIM2->CNT;
   while ((uint16_t)(TIM2->CNT - start) < uSecs);
}

void delay_ms(uint16_t mSecs) {
   uint16_t i;
   for (i = mSecs; i > 0; i--)
      delay_us(1000);
}

void Stepper_Init(){	
  GPIO_InitTypeDef GPIOC_Stepper;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  
  GPIOC_Stepper.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
  GPIOC_Stepper.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIOC_Stepper.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(GPIOC,&GPIOC_Stepper);
  }
	
void Buttons_Init(){
		GPIO_InitTypeDef GPIO_Init_Button;
		
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	  GPIO_Init_Button.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	  GPIO_Init_Button.GPIO_Mode = GPIO_Mode_IPD;
	  GPIO_Init_Button.GPIO_Speed = GPIO_Speed_50MHz;

	  GPIO_Init (GPIOE, &GPIO_Init_Button);		
	}	



 int _angl_To_Step(double angl, double *partStep){
	 double step;
	 *partStep = modf(angl/0.45, &step);
	 
	 return (int)step;
 }
 
 void vUSART_Task4(void *pParams){	
while(1) {	
 if ((UART4->SR & USART_FLAG_RXNE) != (u16)RESET)
    {
            RXc = USART_ReceiveData(UART4);
            RX_BUF[RXi] = RXc;
            RXi++;
 
            if (RXc != '*') {
                if (RXi > RX_BUF_SIZE-1) {
                    clear_RXBuffer();
                }
            }
            else {
               // RX_FLAG_END_LINE = 1;
						
            }
			 USART_SendData(UART5, RXc);
    }
}

}	
void vUSART_Task5(void *pParams){	
while(1) {	
 if ((UART5->SR & USART_FLAG_RXNE) != (u16)RESET)
    {
            RXc = USART_ReceiveData(UART5);
            RX_BUF[RXi] = RXc;
            RXi++;
 
            if (RXc != '*') {
							
                if (RXi > RX_BUF_SIZE-1) {
                    clear_RXBuffer();
                }
							
            }
            else {
               // RX_FLAG_END_LINE = 1;
						
            }
			 USART_SendData(UART5, RXc);
						delay_ms(5);
    }
}

}	
	 
int main(void)
{	
//	RCC_ClocksTypeDef RCC_Clocks;
//  InitRCC();
//  RCC_GetClocksFreq (&RCC_Clocks);RCC_HSICmd(ENABLE);
	
	Stepper_Init();
	delay_init();
	//Reset_Enable_A4982();
	//Set_Sleep_A4982();
	//Set_Reset_A4982();
	//Set_Dir_A4982();
	
	//usart_Configuration(UART4);	
	//usart_Configuration(UART5);	
	
	xTaskCreate(vMotor_Task2, "StepMotor2", 1000, NULL, 1, &xHandle);
	xTaskCreate(vMotor_Task, "StepMotor", 1000, NULL, 1, &xHandle);	
	//xTaskCreate(vUSART_Task5, "UsartTask", 1000, NULL, 1, &xHandle);
	
	vTaskStartScheduler();
		
 while(1)
 {	

 }		
}

void vMotor_Task(void *pParams){	
	while(1)
 {	
	// USART_SendData(UART5, 'g');
	 	//delay_ms(500);
	 //vTaskDelay(400);
	 //if(RXc == '1'){	   
	      //Set_Dir_A4982();
				//Set_STEP_A4982();	 //HIGH		
		    //delay_ms(1000);
				//Reset_STEP_A4982();//LOW
	      //delay_ms(1000);
	 //}
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	  delay_ms(1000);
	 GPIO_ResetBits(GPIOC, GPIO_Pin_7);
	  delay_ms(1000);
 }
 }

void vMotor_Task2(void *pParams){	
	while(1)
 {	
	// USART_SendData(UART5, 'h');
	 // vTaskDelay(500);
	 	//delay_ms(500);
  GPIO_SetBits(GPIOC, GPIO_Pin_8); 
    delay_ms(1000);	 	
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	  delay_ms(1000);
 }
 }
