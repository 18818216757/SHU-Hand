

/**
  ******************************************************************************
  * 文件名程: bsp_EncoderTIM.c
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2017-6-06
  * 功    能: 编码器
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  *
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "drv_encode.h"
#include "stm32f7xx_hal.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
int16_t OverflowCount = 0;//定时器溢出次数
/* Timer handler declaration */
//TIM_HandleTypeDef    htimx_Encoder;
TIM_HandleTypeDef htim1;
/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;

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
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_Encoder_InitTypeDef sConfig;
  __HAL_RCC_TIM2_CLK_ENABLE();
  /*
  htimx_Encoder.Instance = TIM4;
  htimx_Encoder.Init.Prescaler = ENCODER_TIM_PRESCALER;
  htimx_Encoder.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx_Encoder.Init.Period = ENCODER_TIM_PERIOD;
  htimx_Encoder.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;

  sEncoderConfig.EncoderMode        = TIM_ENCODERMODE_TI12;
  sEncoderConfig.IC1Polarity        = TIM_ICPOLARITY_RISING;
  sEncoderConfig.IC1Selection       = TIM_ICSELECTION_DIRECTTI;
  sEncoderConfig.IC1Prescaler       = TIM_ICPSC_DIV1;
  sEncoderConfig.IC1Filter          = 0;

  sEncoderConfig.IC2Polarity        = TIM_ICPOLARITY_RISING;
  sEncoderConfig.IC2Selection       = TIM_ICSELECTION_DIRECTTI;
  sEncoderConfig.IC2Prescaler       = TIM_ICPSC_DIV1;
  sEncoderConfig.IC2Filter          = 0;
  //__HAL_TIM_SET_COUNTER(&htimx_Encoder,0);

  HAL_TIM_Encoder_Init(&htimx_Encoder, &sEncoderConfig);
  */
  
  htim1.Instance = TIM2;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Channel = HAL_TIM_ACTIVE_CHANNEL_1 | HAL_TIM_ACTIVE_CHANNEL_2;
  //htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_BOTHEDGE;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_BOTHEDGE;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    while(1);
  }
  //TIM1->CNT = 0;
  /*
  __HAL_TIM_CLEAR_IT(&htimx_Encoder, TIM_IT_UPDATE);  // 清除更新中断标志位
  __HAL_TIM_URS_ENABLE(&htimx_Encoder);               // 仅允许计数器溢出才产生更新中断
  __HAL_TIM_ENABLE_IT(&htimx_Encoder,TIM_IT_UPDATE);  // 使能更新中断  

  HAL_NVIC_SetPriority(ENCODER_TIM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ENCODER_TIM_IRQn);
  */
  
  HAL_TIM_Encoder_Start_IT(&htim1, TIM_CHANNEL_ALL);
  TIM2->CNT = 0;
  
}

/**
  * 函数功能: 基本定时器硬件初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==TIM2)
  {
    /* 基本定时器外设时钟使能 */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* 定时器通道1功能引脚IO初始化 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* 定时器通道2功能引脚IO初始化 */
    //GPIO_InitStruct.Pin = GPIO_PIN_9;
    //GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    //GPIO_InitStruct.Pull = GPIO_NOPULL;
    //GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    //HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim1);
}

/**
  * 函数功能: 基本定时器硬件反初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==ENCODER_TIMx)
  {
    /* 基本定时器外设时钟禁用 */
    //ENCODER_TIM_RCC_CLK_DISABLE();

    //HAL_GPIO_DeInit(ENCODER_TIM_CH1_GPIO, ENCODER_TIM_CH1_PIN);
    //HAL_GPIO_DeInit(ENCODER_TIM_CH2_GPIO, ENCODER_TIM_CH2_PIN);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1))
    OverflowCount--;       //向下计数溢出
  else
    OverflowCount++;       //向上计数溢出

}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
