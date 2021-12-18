#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "usart.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"

#define RX_BUF_SIZE 80
volatile char RX_FLAG_END_LINE = 0;
volatile char RXi;
volatile char RXc;
char RX_BUF[RX_BUF_SIZE] = "1,1,200";//{'\0'};

char *str = "";
int countRX = 0;
char sep [10]={','};
char *istr;
int numAxis = 0;
int dirAxis[6] = {0,0,0,0,0,0};
int stepAxis[6] = {0,0,0,0,0,0};
unsigned char StopBitToPC = 192;

xTaskHandle xHandleUART;
xTaskHandle xHandleMotor1;
xTaskHandle xHandleMotor2;
xTaskHandle xHandleMotor3;
xTaskHandle xHandleMotor4;
xTaskHandle xHandleMotor5;
xTaskHandle xHandleMotor6;
xTaskHandle xHandleStopBit;

void vMotorTask1 (void *argument);
void vMotorTask2 (void *argument);
void vMotorTask3 (void *argument);
void vMotorTask4 (void *argument);
void vMotorTask5 (void *argument);
void vMotorTask6 (void *argument);
void vUSART_Task5(void *argument);
void vStopBitToPC(void *argument);

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
      delay_us(10);
}
void clear_RXBuffer(void) {
    for (RXi=0; RXi<RX_BUF_SIZE; RXi++)
        RX_BUF[RXi] = '\0';
    RXi = 0;
}
void USARTSend(char RX_BUF[RX_BUF_SIZE], USART_TypeDef* USARTx){
	
	int buf_size = 0;
	
    while (RX_BUF[buf_size]!=NULL)
    {
        USART_SendData(USARTx, RX_BUF[buf_size]);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
        {
        }
				buf_size++;
				if (buf_size==80) break;
    }
}


void Stepper_Init(){	//инициализация двигателей
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//отключение JTAG
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//отключение JTAG
	
	GPIO_InitTypeDef GPIOC_Stepper;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
  GPIOC_Stepper.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
  GPIOC_Stepper.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIOC_Stepper.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(GPIOC,&GPIOC_Stepper);
	
	GPIO_InitTypeDef GPIOA_Stepper2;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIOA_Stepper2.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIOA_Stepper2.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIOA_Stepper2.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIOA_Stepper2);
	
	GPIO_InitTypeDef GPIOA_Stepper3;	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIOA_Stepper3.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;
  GPIOA_Stepper3.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIOA_Stepper3.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIOA_Stepper3);
	
	GPIO_InitTypeDef GPIOB_Stepper4;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIOB_Stepper4.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIOB_Stepper4.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIOB_Stepper4.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&GPIOB_Stepper4);
	
	GPIO_InitTypeDef GPIOE_Stepper5;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIOE_Stepper5.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7;
  GPIOE_Stepper5.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIOE_Stepper5.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(GPIOE,&GPIOE_Stepper5);
	
	GPIO_InitTypeDef GPIOE_Stepper6;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIOE_Stepper6.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8;
  GPIOE_Stepper6.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIOE_Stepper6.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(GPIOD,&GPIOE_Stepper6);
	
	//INIT SENSORS INPUTS
	GPIO_InitTypeDef GPIOE_IO;
  GPIOE_IO.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0;
  GPIOE_IO.GPIO_Mode = GPIO_Mode_IPD;
  GPIOE_IO.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(GPIOE,&GPIOE_IO);	
	
	
	
  }

void AxisMastering(){
		 while (!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)){	
		    Set_STEP_Motor1(); //HIGH				   
		   	delay_us(1000);
	      Reset_STEP_Motor1(); //LOW
		    delay_us(1000); 		
		 }			
			while (!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1)){	
		    Set_STEP_Motor2(); //HIGH				   
		   	delay_us(1000);
	      Reset_STEP_Motor2(); //LOW
		    delay_us(1000); 		
		 }		
			while (!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)){	
		    Set_STEP_Motor3(); //HIGH				   
		   	delay_us(1000);
	      Reset_STEP_Motor3(); //LOW
		    delay_us(1000); 		
		 }				
		 	while (!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)){	
		    Set_STEP_Motor4(); //HIGH				   
		   	delay_us(1000);
	      Reset_STEP_Motor4(); //LOW
		    delay_us(1000); 		
		  }	
			while (!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)){	
		    Set_STEP_Motor5(); //HIGH				   
		   	delay_us(1000);
	      Reset_STEP_Motor5(); //LOW
		    delay_us(1000); 		
		  }	
			while (!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)){	
		    Set_STEP_Motor6(); //HIGH				   
		   	delay_us(1000);
	      Reset_STEP_Motor6(); //LOW
		    delay_us(1000); 		
		  }	
}	
//*******************************************************************************************************/
int main(void){		
	//RCC_ClocksTypeDef clocks;
	//uint32_t cl;
	
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_OFF); 
	//RCC_HSICmd(ENABLE);
	//RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_9);
  RCC_PLLCmd(ENABLE); 
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
	
//   RCC_DeInit(); 
//   RCC_HSICmd(DISABLE); 
//   RCC_HSEConfig(RCC_HSE_ON); 
//   RCC_PREDIV1Config(RCC_PREDIV1_Source_HSE, RCC_PREDIV1_Div12);
//   RCC_PLLConfig(RCC_PLLSource_PREDIV1 ,RCC_PLLMul_4);
//   RCC_PLLCmd(ENABLE); 
//   RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
	
  delay_init();	
	//delay_us(100);
	//RCC_GetClocksFreq(&clocks);
  //cl = clocks.SYSCLK_Frequency;

	Stepper_Init();
	  delay_us(100);
		
	Reset_Enable_Motor1();	
	Set_Sleep_Motor1();
	Set_Reset_Motor1();
	Set_Dir_Motor1();
	
	Reset_Enable_Motor2();	
	Set_Sleep_Motor2();
	Set_Reset_Motor2();
	Set_Dir_Motor2();
	
	Reset_Enable_Motor3();	
	Set_Sleep_Motor3();
	Set_Reset_Motor3();
	Set_Dir_Motor3();
	//Reset_mc1_Motor3();
	
	Reset_Enable_Motor4();	
	Set_Sleep_Motor4();
	Set_Reset_Motor4();
	Set_Dir_Motor4();
	
	Reset_Enable_Motor5();	
	Set_Sleep_Motor5();
	Set_Reset_Motor5();                                              
	Set_Dir_Motor5();
	
	Reset_Enable_Motor6();	
	Set_Sleep_Motor6();
	Set_Reset_Motor6();                                              
	Set_Dir_Motor6();
	
	//usart_Configuration(UART4);
	__enable_irq ();
	  usart_Configuration(UART5);
	  NVIC_EnableIRQ(UART5_IRQn);

	 
	//AxisMastering();
	xTaskCreate(vMotorTask1, "MotorTask1", 32, NULL, 1, &xHandleMotor1);
	xTaskCreate(vMotorTask2, "MotorTask2", 32, NULL, 1, &xHandleMotor2);
	xTaskCreate(vMotorTask3, "MotorTask3", 32, NULL, 1, &xHandleMotor3);
	xTaskCreate(vMotorTask4, "MotorTask4", 32, NULL, 1, &xHandleMotor4);
	xTaskCreate(vMotorTask5, "MotorTask5", 32, NULL, 1, &xHandleMotor5);
	xTaskCreate(vMotorTask6, "MotorTask6", 32, NULL, 1, &xHandleMotor6);
	xTaskCreate(vStopBitToPC, "vStopBitToPC", 32, NULL, 1, &xHandleStopBit);
		
	vTaskStartScheduler();

	while(1)
	{		
		
	}
}

/*******************************************************************************************************/
void vMotorTask1 (void *argument){
	while(1)
	{

		if (RX_FLAG_END_LINE == 1) {				
		while (stepAxis[0]>0){	
		    Set_STEP_Motor1(); //HIGH		
		    vTaskDelay(1);
	      Reset_STEP_Motor1(); //LOW
		    vTaskDelay(1); 
			  stepAxis[0]--;
		}		
		StopBitToPC = StopBitToPC | (1<<0);
		//vTaskResume(xHandleUART);
	}
		vTaskSuspend( xHandleMotor1 );		
	}
}

void vMotorTask2 (void *argument){
	while(1)
	{
		if (RX_FLAG_END_LINE == 1) {			
		while (stepAxis[1]>0){	
		    Set_STEP_Motor2();	 //HIGH		
			  vTaskDelay(1);
	      Reset_STEP_Motor2();//LOW	
        vTaskDelay(1);			
			  stepAxis[1]--;
		}
		StopBitToPC = StopBitToPC | (1<<1);
		}
		vTaskSuspend( xHandleMotor2 );
	}	
}
void vMotorTask3 (void *argument){
	while(1)
	{
		if (RX_FLAG_END_LINE == 1) {			
		while (stepAxis[2]>0){	
		    Set_STEP_Motor3();	 //HIGH		
			  vTaskDelay(1);
	      Reset_STEP_Motor3();//LOW
        vTaskDelay(1);			
			  stepAxis[2]--;
		}
		StopBitToPC = StopBitToPC | (1<<2);
		}
		vTaskSuspend( xHandleMotor3 );
	}	
}
void vMotorTask4 (void *argument){
	while(1)
	{
		if (RX_FLAG_END_LINE == 1) {			
		while (stepAxis[3]>0){	
		  Set_STEP_Motor4();	 //HIGH		
			vTaskDelay(1);
	    Reset_STEP_Motor4();//LOW		  	
      vTaskDelay(1);			
			  stepAxis[3]--;
		}
		StopBitToPC = StopBitToPC | (1<<3);
		}
		vTaskSuspend( xHandleMotor4 );
	}	
}
void vMotorTask5 (void *argument){
	while(1)
	{
		if (RX_FLAG_END_LINE == 1) {			
		while (stepAxis[4]>0){	
		    Set_STEP_Motor5();	 //HIGH		
			  vTaskDelay(1);
	      Reset_STEP_Motor5();//LOW	
        vTaskDelay(1);			
			  stepAxis[4]--;
		}
		StopBitToPC = StopBitToPC | (1<<4);
		StopBitToPC = StopBitToPC | (1<<5);
		}
		vTaskSuspend( xHandleMotor5 );
	}	
}
void vMotorTask6 (void *argument){
	while(1)
	{
		if (RX_FLAG_END_LINE == 1) {			
		while (stepAxis[5]>0){	
		    Set_STEP_Motor6();	 //HIGH		
		    vTaskDelay(1);
	      Reset_STEP_Motor6();//LOW
        vTaskDelay(1);			
			  stepAxis[5]--;
		}
		StopBitToPC = StopBitToPC | (1<<5);
		}
		vTaskSuspend( xHandleMotor6 );
	}	
}
void vStopBitToPC(void *argument){//отправляет на устройство управления, то что робот переместился в заданное положение
	while(1)
	{
	if(StopBitToPC==255){
	   USARTSend("STOP", UART5);
     USARTSend("\r\n", UART5);		
		 StopBitToPC = 192;
		 vTaskSuspend( xHandleStopBit );
	}
  }
}  
void UART5_IRQHandler(void)
{
 if ((UART5->SR & USART_FLAG_RXNE) != (u16)RESET)
    {
			
            RXc = USART_ReceiveData(UART5);
            RX_BUF[RXi] = RXc;
					  RXi++;			               
				
		            if (RXi > RX_BUF_SIZE-1) {
                  RXi=0;
										}
								
					
//	        if (RXc == '^') { //проверка символа конца полученной строки
//						USARTSend(RX_BUF, UART5);
//						USARTSend("\r\n", UART5);
//						vTaskDelay(4);	
//				   	for(int i =0; i<RXi-1; i++)
//	          {
//	          	USART_SendData(UART5, RX_BUF[i]);
//		          vTaskDelay(10);							
//	         	}
				//	}
          if (RXc == '^') {
																				
						istr = strtok (RX_BUF, sep);  //делим строку по сипаратору
              if(istr!=NULL){											
							   numAxis = atoi(istr);			//получаем номер оси				
								istr = strtok (NULL,sep);  //для получения следущего значения по сипоратору используем функцию со значение NULL
							switch (numAxis)
							{
								case 1:
								{
									  dirAxis[0] = atoi(istr); //напрвление вращения для первой оси
									  istr = strtok (NULL,sep);  
								    stepAxis[0] = atoi(istr); //количество шагов									   
										if(dirAxis[0]==1)	{
			                 Set_Dir_Motor1();
	                	 }else Reset_Dir_Motor1();
								} break;
								case 2:{
									   dirAxis[1] = atoi(istr);
									   istr = strtok (NULL,sep);  
									   stepAxis[1] = atoi(istr);									   
										  if(dirAxis[1]==1)	{
			                   Set_Dir_Motor2();
		                  }else Reset_Dir_Motor2();
								} break;
								case 3:{
									dirAxis[2] = atoi(istr);
									istr = strtok (NULL,sep);  
									stepAxis[2] = atoi(istr);
									 if(dirAxis[2]==1)	{
			                   Set_Dir_Motor3();
		                  }else Reset_Dir_Motor3();
								} break;
								case 4:{
									dirAxis[3] = atoi(istr);
									istr = strtok (NULL,sep);  
									stepAxis[3] = atoi(istr);
									 if(dirAxis[3]==1)	{
			                   Set_Dir_Motor4();
		                  }else Reset_Dir_Motor4();
								} break;
								case 5:{
									dirAxis[4] = atoi(istr);
									istr = strtok (NULL,sep);  
									stepAxis[4] = atoi(istr);
									 if(dirAxis[4]==1)	{
			                   Set_Dir_Motor5();
		                  }else Reset_Dir_Motor5();
								} break;
								case 6:{
									dirAxis[5] = atoi(istr);
									istr = strtok (NULL,sep);  
									stepAxis[5] = atoi(istr);
									 if(dirAxis[5]==1)	{
			                   Set_Dir_Motor6();
		                  }else Reset_Dir_Motor6();
								} break;
							}					            						
               	RXi=0;						
						}			
          }	
							if (RXc == '*') {
	              RX_FLAG_END_LINE = 1;	
								vTaskResume(xHandleMotor1);
								vTaskResume(xHandleMotor2);
								vTaskResume(xHandleMotor3);
								vTaskResume(xHandleMotor4);
								vTaskResume(xHandleMotor5);
								vTaskResume(xHandleMotor6);
								vTaskResume(xHandleStopBit);
								
								//vTaskDelay(1000);
								//vTaskSuspend( xHandleUART );
									RXi=0;
							}							
    }		
//				    if(StopBitToPC==255)
//							{
//	               USART_SendData(UART5, 'S');
//		             StopBitToPC = 192;
//								 vTaskDelay(5);
//	            }
 
}	

