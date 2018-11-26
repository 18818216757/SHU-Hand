#ifndef _CALCULATE_ANGLE_H
#define _CALCULATE_ANGLE_H
#include "sys.h"
#include "drv_motor.h"
#include "common.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
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
void Calcuate_timer_Init(void);    //��ʱ����ʼ��
void Finger_Speed_PID_INIT(void);
uint64_t sys_millis(void);
extern PID_TypeDef Finger_position_Pid_paramter;
extern PID_TypeDef Finger_speed_Pid_paramter;
extern PID_TypeDef Finger_pressure_Pid_paramter;
extern FINGER_PARAMETER  m_finger_paramter;
#endif

