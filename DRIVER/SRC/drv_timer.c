#include "drv_timer.h"
#include "common.h"
//////////////////////////////////////////////////////////////////////////////////	 
//Creat by xiao
//Copyright(C) Shanghai University
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//文件名：drv_timer.c
//文件说明：初始化PWM
//V1.1 2018.11.3
//新增TIM_PWM_Init函数,用于PWM初始化
//使用了3个TIM：TIM2，TIM3，TIM4，channel使用的都是channel3和channel4，channel1和2用来做为编码器的接受口
//GPOIO使用了6个：PA2，PA3，PB0，PB1，PD14，PD15
////////////////////////////////////////////////////////////////////////////////// 	 



TIM_OC_InitTypeDef TIM_CH_Handler;
uint32_t Pwm_timer_channel[FINGER_SEL_TOTAL] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_1, TIM_CHANNEL_1, TIM_CHANNEL_1, TIM_CHANNEL_2};
TIM_TypeDef *Pwm_timer[FINGER_SEL_TOTAL] = {TIM9, TIM9, TIM10, TIM11, TIM12, TIM12};
GPIO_TypeDef *Pwm_GPIO_TYPE[FINGER_SEL_TOTAL] = {GPIOE, GPIOE, GPIOF, GPIOF, GPIOB, GPIOB};
uint16_t Pwm_GPIO_Pin[FINGER_SEL_TOTAL] = {GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_14, GPIO_PIN_15};
uint32_t Pwm_GPIO_AF[FINGER_SEL_TOTAL] = {GPIO_AF3_TIM9, GPIO_AF3_TIM9, GPIO_AF3_TIM10, GPIO_AF3_TIM11, GPIO_AF9_TIM12, GPIO_AF9_TIM12};

void TIM_PWM_Init(void)
{ 
	  TIM_HandleTypeDef TIM_Handler;
	  FINGER_SELECT fingersel;
    
    __HAL_RCC_TIM9_CLK_ENABLE(); 
	  __HAL_RCC_TIM10_CLK_ENABLE();
    __HAL_RCC_TIM11_CLK_ENABLE(); 
	  __HAL_RCC_TIM12_CLK_ENABLE();
  
    TIM_Handler.Init.Prescaler = FREQ_DIV;       //定时器分频
    TIM_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;//向上计数模式
    TIM_Handler.Init.Period = LOAD_VALUE;          //自动重装载值
    TIM_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  
	  TIM_CH_Handler.OCMode = TIM_OCMODE_PWM1; //模式选择PWM1
    TIM_CH_Handler.Pulse = 0;
	  TIM_CH_Handler.OCPolarity = TIM_OCPOLARITY_LOW; //输出比较极性为低 
	  
    //TIM_Handler.Instance = TIM9;
		//HAL_TIM_PWM_Init(&TIM_Handler);
    //HAL_TIM_PWM_ConfigChannel(&TIM_Handler, &TIM_CH_Handler, TIM_CHANNEL_1);
		//HAL_TIM_PWM_Start(&TIM_Handler, TIM_CHANNEL_1);
     
	  for (fingersel = FINGER_SEL_THUMB; fingersel < FINGER_SEL_TOTAL; fingersel ++) {
			TIM_Handler.Instance = Pwm_timer[fingersel];
			HAL_TIM_PWM_Init(&TIM_Handler);
      HAL_TIM_PWM_ConfigChannel(&TIM_Handler, &TIM_CH_Handler, Pwm_timer_channel[fingersel]);
		  HAL_TIM_PWM_Start(&TIM_Handler, Pwm_timer_channel[fingersel]);			
	  }
}


void TIM_PWM_GPIO_INIT() {
    FINGER_SELECT fingersel;
    GPIO_InitTypeDef GPIO_Initure;
	  	
    __HAL_RCC_GPIOB_CLK_ENABLE();
	  __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
  
	  GPIO_Initure.Mode = GPIO_MODE_AF_PP;  	   //复用推完输出
    GPIO_Initure.Pull = GPIO_PULLUP;           //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;      //高速
    //GPIO_Initure.Pin = GPIO_PIN_5;
	  //GPIO_Initure.Alternate = GPIO_AF3_TIM9;
		//HAL_GPIO_Init(GPIOE, &GPIO_Initure);
	  
	  for (fingersel = FINGER_SEL_THUMB; fingersel < FINGER_SEL_TOTAL; fingersel ++) {
			GPIO_Initure.Pin = Pwm_GPIO_Pin[fingersel];
			GPIO_Initure.Alternate = Pwm_GPIO_AF[fingersel];
			HAL_GPIO_Init(Pwm_GPIO_TYPE[fingersel], &GPIO_Initure);
		}
}


/************************************************
Function: init Pwm Gpio
Resources: GPIOA1, GPIOA2, GPIOB0, GPIOB1, GPIOD14, GPIOD15
Author: xiao
************************************************/
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{  
}




/************************************************
Function: Set pwm duty cycle 
Resources: 
Author: xiao
************************************************/
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1)
{
  /* Check the parameters */
  assert_param(IS_TIM_LIST1_PERIPH(TIMx));

  /* Set the Capture Compare1 Register value */
  TIMx->CCR1 = Compare1;
}

void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare2)
{
  /* Check the parameters */
  assert_param(IS_TIM_LIST2_PERIPH(TIMx));

  /* Set the Capture Compare2 Register value */
  TIMx->CCR2 = Compare2;
}

void TIM_SetCompare3(TIM_TypeDef* TIMx, uint32_t Compare3)
{
  /* Check the parameters */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));

  /* Set the Capture Compare3 Register value */
  TIMx->CCR3 = Compare3;
}

void TIM_SetCompare4(TIM_TypeDef* TIMx, uint32_t Compare4)
{
  /* Check the parameters */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));

  /* Set the Capture Compare4 Register value */
  TIMx->CCR4 = Compare4;
}




