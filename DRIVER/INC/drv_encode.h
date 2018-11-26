#ifndef __BSP_ENCODER_H__
#define __BSP_ENCODER_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "common.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
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

// 定义定时器预分频，定时器实际时钟频率为：72MHz/（ENCODER_TIMx_PRESCALER+1）
#define ENCODER_TIM_PRESCALER               0  // 实际时钟频率为：36kHz

// 定义定时器周期，当定时器开始计数到ENCODER_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define ENCODER_TIM_PERIOD                  0x500

/* 扩展变量 ------------------------------------------------------------------*/
//extern TIM_HandleTypeDef htimx_Encoder;
extern TIM_HandleTypeDef htim1;
extern int16_t OverflowCount ;//定时器溢出次数
/* 函数声明 ------------------------------------------------------------------*/
void ENCODER_TIMx_Init(void);

#endif	/* __ENCODER_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
