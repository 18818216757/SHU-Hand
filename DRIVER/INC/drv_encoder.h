#ifndef __BSP_ENCODER_H__
#define __BSP_ENCODER_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "common.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define ENCODER_TIMx                        TIM4
#define ENCODER_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM4_CLK_ENABLE()
#define ENCODER_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM4_CLK_DISABLE()

#define ENCODER_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()


#define TIM_ENCODERMODE_TIx                 TIM_ENCODERMODE_TI12

#ifdef MAXON_MOTOR
#define ENCODER_TIM_PERIOD  34304           //67 * 256 * 2 ���ڳ�������ٱ�Ϊ67��1���ұ���������Ϊ256�����Ե��תһȦ��������Ϊ��ֵ
#define ENCODER_MAX_VALUE   17152
#else 
#define ENCODER_TIM_PERIOD  1610           // 115* 7 * 2 ���ڳ�������ٱ�Ϊ67��1���ұ���������Ϊ256�����Ե��תһȦ��������Ϊ��ֵ
#define ENCODER_MAX_VALUE   805
#endif
// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/��ENCODER_TIMx_PRESCALER+1��
#define ENCODER_TIM_PRESCALER               0  // ʵ��ʱ��Ƶ��Ϊ��36kHz

// ���嶨ʱ�����ڣ�����ʱ����ʼ������ENCODER_TIMx_PERIODֵ�Ǹ��¶�ʱ�������ɶ�Ӧ�¼����ж�

 
/* ��չ���� ------------------------------------------------------------------*/
extern TIM_TypeDef *Encoder_timer[FINGER_SEL_TOTAL];
/* �������� ------------------------------------------------------------------*/
void ENCODER_TIMx_Init(void);
void Encoder_Gpio_Init(void);

#endif	/* __ENCODER_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
