#ifndef _CALCULATE_ANGLE_H
#define _CALCULATE_ANGLE_H
#include "sys.h"
#include "drv_motor.h"
#include "common.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//定时器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define TIM_Prescaler   1000-1
#define TIM_Period      108-1
#define MOTOR_FOREWARD   0
#define MOTOR_REVERSEL   1

#ifdef MAXON_MOTOR
#define POSITION_GUID         100
#define POSITION_PROPORTION   0.08
#define SPEED_GUID            20
#else
#define POSITION_GUID         20
#define POSITION_PROPORTION   1.8
#define SPEED_GUID            10
#endif

int32_t SpeedPIDCalc(FINGER_SELECT fingersel, uint16_t NextPoint);
void Calcuate_timer_Init(void);    //定时器初始化
void Finger_Speed_PID_INIT(void);
uint64_t sys_millis(void);
extern PID_TypeDef Finger_position_Pid_paramter;
extern PID_TypeDef Finger_speed_Pid_paramter;
extern PID_TypeDef Finger_pressure_Pid_paramter;
extern FINGER_PARAMETER  m_finger_paramter;
#endif

