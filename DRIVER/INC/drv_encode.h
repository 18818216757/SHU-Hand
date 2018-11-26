#ifndef __BSP_ENCODER_H__
#define __BSP_ENCODER_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "common.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define ENCODER_TIMx                        TIM4
#define ENCODER_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM4_CLK_ENABLE()
#define ENCODER_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM4_CLK_DISABLE()

#define ENCODER_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()

#define ENCODER_TIM_CH1_PIN                 GPIO_PIN_6
#define ENCODER_TIM_CH1_GPIO                GPIOB
#define ENCODER_TIM_CH2_PIN                 GPIO_PIN_7
#define ENCODER_TIM_CH2_GPIO                GPIOB

#define TIM_ENCODERMODE_TIx                 TIM_ENCODERMODE_TI12

#define ENCODER_TIM_IRQn                    TIM4_IRQn
#define ENCODER_TIM_IRQHANDLER              TIM4_IRQHandler

#define GPIO_AFx_TIMx                       GPIO_AF2_TIM4

// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/��ENCODER_TIMx_PRESCALER+1��
#define ENCODER_TIM_PRESCALER               0  // ʵ��ʱ��Ƶ��Ϊ��36kHz

// ���嶨ʱ�����ڣ�����ʱ����ʼ������ENCODER_TIMx_PERIODֵ�Ǹ��¶�ʱ�������ɶ�Ӧ�¼����ж�
#define ENCODER_TIM_PERIOD                  0x500

/* ��չ���� ------------------------------------------------------------------*/
//extern TIM_HandleTypeDef htimx_Encoder;
extern TIM_HandleTypeDef htim1;
extern int16_t OverflowCount ;//��ʱ���������
/* �������� ------------------------------------------------------------------*/
void ENCODER_TIMx_Init(void);

#endif	/* __ENCODER_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
