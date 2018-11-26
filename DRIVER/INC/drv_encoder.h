#ifndef __BSP_ENCODER_H__
#define __BSP_ENCODER_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "common.h"
/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define ENCODER_TIMx                        TIM4
#define ENCODER_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM4_CLK_ENABLE()
#define ENCODER_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM4_CLK_DISABLE()

#define ENCODER_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()


#define TIM_ENCODERMODE_TIx                 TIM_ENCODERMODE_TI12

#ifdef MAXON_MOTOR
#define ENCODER_TIM_PERIOD  34304           //67 * 256 * 2 由于齿轮箱减速比为67：1，且编码器线数为256，所以电机转一圈的脉冲数为此值
#define ENCODER_MAX_VALUE   17152
#else 
#define ENCODER_TIM_PERIOD  1610           // 115* 7 * 2 由于齿轮箱减速比为67：1，且编码器线数为256，所以电机转一圈的脉冲数为此值
#define ENCODER_MAX_VALUE   805
#endif
// 定义定时器预分频，定时器实际时钟频率为：72MHz/（ENCODER_TIMx_PRESCALER+1）
#define ENCODER_TIM_PRESCALER               0  // 实际时钟频率为：36kHz

// 定义定时器周期，当定时器开始计数到ENCODER_TIMx_PERIOD值是更新定时器并生成对应事件和中断

 
/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_TypeDef *Encoder_timer[FINGER_SEL_TOTAL];
/* 函数声明 ------------------------------------------------------------------*/
void ENCODER_TIMx_Init(void);
void Encoder_Gpio_Init(void);

#endif	/* __ENCODER_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
