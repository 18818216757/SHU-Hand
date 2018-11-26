//////////////////////////////////////////////////////////////////////////////////	 
//Creat by xiao
//Copyright(C) Shanghai University
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//文件名：drv_encoder.c
//文件说明：编码器驱动
//修改说明
//V1.1 2018.11.3
//新增ENCODER_TIMx_Init函数,用于Encoder初始化
////////////////////////////////////////////////////////////////////////////////// 	 
/* 包含头文件 ----------------------------------------------------------------*/
#include "drv_encoder.h"


/* resourse map*/
TIM_TypeDef *Encoder_timer[FINGER_SEL_TOTAL] = {TIM1, TIM2, TIM3, TIM4, TIM5, TIM8};
GPIO_TypeDef *Encoder_GPIO_TYPE[FINGER_SEL_TOTAL][2] = {{GPIOE, GPIOE}, {GPIOA, GPIOB}, {GPIOA, GPIOA}, \
                                                        {GPIOD, GPIOD}, {GPIOA, GPIOA}, {GPIOC, GPIOC}};
uint16_t Encoder_GPIO_Pin[FINGER_SEL_TOTAL][2] = {{GPIO_PIN_9, GPIO_PIN_11}, {GPIO_PIN_5, GPIO_PIN_3}, {GPIO_PIN_6, GPIO_PIN_7}, \
                                                  {GPIO_PIN_12, GPIO_PIN_13}, {GPIO_PIN_0, GPIO_PIN_1}, {GPIO_PIN_6, GPIO_PIN_7}};
uint32_t Encoder_GPIO_AF[FINGER_SEL_TOTAL] = {GPIO_AF1_TIM1, GPIO_AF1_TIM2, GPIO_AF2_TIM3, GPIO_AF2_TIM4, GPIO_AF2_TIM5, GPIO_AF3_TIM8};



/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 通用定时器初始化并配置通道PWM输出
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void ENCODER_TIMx_Init(void)
{ 
	TIM_HandleTypeDef htim;
  FINGER_SELECT fingersel;
	TIM_Encoder_InitTypeDef sConfig;
  __HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();
	__HAL_RCC_TIM5_CLK_ENABLE();
	__HAL_RCC_TIM8_CLK_ENABLE();
	htim.Init.Prescaler = 0;
  htim.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim.Init.Period = ENCODER_TIM_PERIOD;
  htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
 
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
	for (fingersel = FINGER_SEL_THUMB; fingersel < FINGER_SEL_TOTAL; fingersel ++) {
	  htim.Instance = Encoder_timer[fingersel];
    HAL_TIM_Encoder_Init(&htim, &sConfig);
  	HAL_TIM_Encoder_Start(&htim, TIM_CHANNEL_ALL);
	}
}

void Encoder_Gpio_Init() {
	FINGER_SELECT fingersel;
  GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; 
	for (fingersel = FINGER_SEL_THUMB; fingersel < FINGER_SEL_TOTAL; fingersel ++) {
		GPIO_InitStruct.Pin = Encoder_GPIO_Pin[fingersel][0];
		GPIO_InitStruct.Alternate = Encoder_GPIO_AF[fingersel];
		HAL_GPIO_Init(Encoder_GPIO_TYPE[fingersel][0], &GPIO_InitStruct);
		GPIO_InitStruct.Pin = Encoder_GPIO_Pin[fingersel][1];
		HAL_GPIO_Init(Encoder_GPIO_TYPE[fingersel][1], &GPIO_InitStruct);
  }
}

/**
  * 函数功能: 基本定时器硬件初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_base)
{
}

/**
  * 函数功能: 基本定时器硬件反初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_base)
{
} 

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
