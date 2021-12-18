//#include "stm32f10x_gpio.h"

#define SYS_CLK SystemCoreClock    /* in this example we use SPEED_HIGH = 72 MHz */
#define DELAY_TIM_FREQUENCY 1000000 /* = 1MHZ -> timer runs in microseconds */

//SET BIT motor1
#define Set_Dir_Motor1() GPIO_SetBits(GPIOC, GPIO_Pin_0);
#define Set_Enable_Motor1() GPIO_SetBits(GPIOC, GPIO_Pin_1);
#define Set_STEP_Motor1() GPIO_SetBits(GPIOC, GPIO_Pin_2);
#define Set_Sleep_Motor1() GPIO_SetBits(GPIOC, GPIO_Pin_3);
#define Set_Reset_Motor1() GPIO_SetBits(GPIOC, GPIO_Pin_4);
#define Set_mc2_Motor1() GPIO_SetBits(GPIOC, GPIO_Pin_5);
#define Set_mc1_Motor1() GPIO_SetBits(GPIOC, GPIO_Pin_6);

//RESET BIT motor1
#define Reset_Dir_Motor1() GPIO_ResetBits(GPIOC, GPIO_Pin_0);
#define Reset_Enable_Motor1() GPIO_ResetBits(GPIOC, GPIO_Pin_1);
#define Reset_STEP_Motor1() GPIO_ResetBits(GPIOC, GPIO_Pin_2);
#define Reset_Sleep_Motor1() GPIO_ResetBits(GPIOC, GPIO_Pin_3);
#define Reset_Reset_Motor1() GPIO_ResetBits(GPIOC, GPIO_Pin_4);
#define Reset_mc2_Motor1() GPIO_ResetBits(GPIOC, GPIO_Pin_5);
#define Reset_mc1_Motor1() GPIO_ResetBits(GPIOC, GPIO_Pin_6);

//SET BIT motor2
#define Set_Dir_Motor2() GPIO_SetBits(GPIOA, GPIO_Pin_1);
#define Set_Enable_Motor2() GPIO_SetBits(GPIOA, GPIO_Pin_2);
#define Set_STEP_Motor2() GPIO_SetBits(GPIOA, GPIO_Pin_3);
#define Set_Sleep_Motor2() GPIO_SetBits(GPIOA, GPIO_Pin_4);
#define Set_Reset_Motor2() GPIO_SetBits(GPIOA, GPIO_Pin_5);
#define Set_mc2_Motor2 () GPIO_SetBits(GPIOA, GPIO_Pin_6);
#define Set_mc1_Motor2() GPIO_SetBits(GPIOA, GPIO_Pin_7);

//RESET BIT motor2
#define Reset_Dir_Motor2() GPIO_ResetBits(GPIOA, GPIO_Pin_1);
#define Reset_Enable_Motor2() GPIO_ResetBits(GPIOA, GPIO_Pin_2);
#define Reset_STEP_Motor2() GPIO_ResetBits(GPIOA, GPIO_Pin_3);
#define Reset_Sleep_Motor2() GPIO_ResetBits(GPIOA, GPIO_Pin_4);
#define Reset_Reset_Motor2() GPIO_ResetBits(GPIOA, GPIO_Pin_5);
#define Reset_mc2_Motor2() GPIO_ResetBits(GPIOA, GPIO_Pin_6);
#define Reset_mc1_Motor2() GPIO_ResetBits(GPIOA, GPIO_Pin_7);

//SET BIT motor3
#define Set_Dir_Motor3() GPIO_SetBits(GPIOA, GPIO_Pin_15);
#define Set_Enable_Motor3() GPIO_SetBits(GPIOA, GPIO_Pin_12);
#define Set_STEP_Motor3() GPIO_SetBits(GPIOA, GPIO_Pin_11);
#define Set_Sleep_Motor3() GPIO_SetBits(GPIOA, GPIO_Pin_10);
#define Set_Reset_Motor3() GPIO_SetBits(GPIOA, GPIO_Pin_9);
#define Set_mc2_Motor3 () GPIO_SetBits(GPIOA, GPIO_Pin_8);
#define Set_mc1_Motor3() GPIO_SetBits(GPIOA, GPIO_Pin_0);

//RESET BIT motor3
#define Reset_Dir_Motor3() GPIO_ResetBits(GPIOA, GPIO_Pin_15);
#define Reset_Enable_Motor3() GPIO_ResetBits(GPIOA, GPIO_Pin_12);
#define Reset_STEP_Motor3() GPIO_ResetBits(GPIOA, GPIO_Pin_11);
#define Reset_Sleep_Motor3() GPIO_ResetBits(GPIOA, GPIO_Pin_10);
#define Reset_Reset_Motor3() GPIO_ResetBits(GPIOA, GPIO_Pin_9);
#define Reset_mc2_Motor3() GPIO_ResetBits(GPIOA, GPIO_Pin_8);
#define Reset_mc1_Motor3() GPIO_ResetBits(GPIOA, GPIO_Pin_0);

//SET BIT motor4
#define Set_Dir_Motor4() GPIO_SetBits(GPIOB, GPIO_Pin_0);
#define Set_Enable_Motor4() GPIO_SetBits(GPIOB, GPIO_Pin_1);
#define Set_STEP_Motor4() GPIO_SetBits(GPIOB, GPIO_Pin_3);
#define Set_Sleep_Motor4() GPIO_SetBits(GPIOB, GPIO_Pin_4);
#define Set_Reset_Motor4() GPIO_SetBits(GPIOB, GPIO_Pin_5);
#define Set_mc2_Motor4 () GPIO_SetBits(GPIOB, GPIO_Pin_6);
#define Set_mc1_Motor4() GPIO_SetBits(GPIOB, GPIO_Pin_7);

//RESET BIT motor4
#define Reset_Dir_Motor4() GPIO_ResetBits(GPIOB, GPIO_Pin_0);
#define Reset_Enable_Motor4() GPIO_ResetBits(GPIOB, GPIO_Pin_1);
#define Reset_STEP_Motor4() GPIO_ResetBits(GPIOB, GPIO_Pin_3);
#define Reset_Sleep_Motor4() GPIO_ResetBits(GPIOB, GPIO_Pin_4);
#define Reset_Reset_Motor4() GPIO_ResetBits(GPIOB, GPIO_Pin_5);
#define Reset_mc2_Motor4() GPIO_ResetBits(GPIOB, GPIO_Pin_6);
#define Reset_mc1_Motor4() GPIO_ResetBits(GPIOB, GPIO_Pin_7);

//SET BIT motor5
#define Set_Dir_Motor5() GPIO_SetBits(GPIOE, GPIO_Pin_13);
#define Set_Enable_Motor5() GPIO_SetBits(GPIOE, GPIO_Pin_12);
#define Set_STEP_Motor5() GPIO_SetBits(GPIOE, GPIO_Pin_11);
#define Set_Sleep_Motor5() GPIO_SetBits(GPIOE, GPIO_Pin_10);
#define Set_Reset_Motor5() GPIO_SetBits(GPIOE, GPIO_Pin_9);
#define Set_mc2_Motor5 () GPIO_SetBits(GPIOE, GPIO_Pin_8);
#define Set_mc1_Motor5() GPIO_SetBits(GPIOE, GPIO_Pin_7);

//RESET BIT motor5
#define Reset_Dir_Motor5() GPIO_ResetBits(GPIOE, GPIO_Pin_13);
#define Reset_Enable_Motor5() GPIO_ResetBits(GPIOE, GPIO_Pin_12);
#define Reset_STEP_Motor5() GPIO_ResetBits(GPIOE, GPIO_Pin_11);
#define Reset_Sleep_Motor5() GPIO_ResetBits(GPIOE, GPIO_Pin_10);
#define Reset_Reset_Motor5() GPIO_ResetBits(GPIOE, GPIO_Pin_9);
#define Reset_mc2_Motor5() GPIO_ResetBits(GPIOE, GPIO_Pin_8);
#define Reset_mc1_Motor5() GPIO_ResetBits(GPIOE, GPIO_Pin_7);

//SET BIT motor6
#define Set_Dir_Motor6() GPIO_SetBits(GPIOD, GPIO_Pin_14);
#define Set_Enable_Motor6() GPIO_SetBits(GPIOD, GPIO_Pin_13);
#define Set_STEP_Motor6() GPIO_SetBits(GPIOD, GPIO_Pin_12);
#define Set_Sleep_Motor6() GPIO_SetBits(GPIOD, GPIO_Pin_11);
#define Set_Reset_Motor6() GPIO_SetBits(GPIOD, GPIO_Pin_10);
#define Set_mc2_Motor6 () GPIO_SetBits(GPIOD, GPIO_Pin_9);
#define Set_mc1_Motor6() GPIO_SetBits(GPIOD, GPIO_Pin_8);

//RESET BIT motor6
#define Reset_Dir_Motor6() GPIO_ResetBits(GPIOD, GPIO_Pin_14);
#define Reset_Enable_Motor6() GPIO_ResetBits(GPIOD, GPIO_Pin_13);
#define Reset_STEP_Motor6() GPIO_ResetBits(GPIOD, GPIO_Pin_12);
#define Reset_Sleep_Motor6() GPIO_ResetBits(GPIOD, GPIO_Pin_11);
#define Reset_Reset_Motor6() GPIO_ResetBits(GPIOD, GPIO_Pin_10);
#define Reset_mc2_Motor6() GPIO_ResetBits(GPIOD, GPIO_Pin_9);
#define Reset_mc1_Motor6() GPIO_ResetBits(GPIOD, GPIO_Pin_8);
