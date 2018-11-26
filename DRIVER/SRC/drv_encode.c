

/**
  ******************************************************************************
  * �ļ�����: bsp_EncoderTIM.c
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-6-06
  * ��    ��: ������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  *
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "drv_encode.h"
#include "stm32f7xx_hal.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
int16_t OverflowCount = 0;//��ʱ���������
/* Timer handler declaration */
//TIM_HandleTypeDef    htimx_Encoder;
TIM_HandleTypeDef htim1;
/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ͨ�ö�ʱ����ʼ��������ͨ��PWM���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  __HAL_TIM_CLEAR_IT(&htimx_Encoder, TIM_IT_UPDATE);  // ��������жϱ�־λ
  __HAL_TIM_URS_ENABLE(&htimx_Encoder);               // ���������������Ų��������ж�
  __HAL_TIM_ENABLE_IT(&htimx_Encoder,TIM_IT_UPDATE);  // ʹ�ܸ����ж�  

  HAL_NVIC_SetPriority(ENCODER_TIM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ENCODER_TIM_IRQn);
  */
  
  HAL_TIM_Encoder_Start_IT(&htim1, TIM_CHANNEL_ALL);
  TIM2->CNT = 0;
  
}

/**
  * ��������: ������ʱ��Ӳ����ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==TIM2)
  {
    /* ������ʱ������ʱ��ʹ�� */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* ��ʱ��ͨ��1��������IO��ʼ�� */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ��ʱ��ͨ��2��������IO��ʼ�� */
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
  * ��������: ������ʱ��Ӳ������ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==ENCODER_TIMx)
  {
    /* ������ʱ������ʱ�ӽ��� */
    //ENCODER_TIM_RCC_CLK_DISABLE();

    //HAL_GPIO_DeInit(ENCODER_TIM_CH1_GPIO, ENCODER_TIM_CH1_PIN);
    //HAL_GPIO_DeInit(ENCODER_TIM_CH2_GPIO, ENCODER_TIM_CH2_PIN);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1))
    OverflowCount--;       //���¼������
  else
    OverflowCount++;       //���ϼ������

}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
