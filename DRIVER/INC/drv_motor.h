#ifndef _DRV_MOTOR_H
#define _DRV_MOTOR_H
#include "drv_timer.h"
#include "common.h"

//////////////////////////////////////////////////////////////////////////////////	 
//creat by xiao
//driver DC motor 
//Copyright(C) ShangHai University
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
extern TIM_TypeDef *Pwm_timer[FINGER_SEL_TOTAL];
void Set_motor_speed(FINGER_SELECT fingersel, uint16_t speed);
void Set_motor_direction(FINGER_SELECT f_select, SERVO_DIR_SEL servo_dir);
void Gpio_motor_dir_init(void);

#endif

