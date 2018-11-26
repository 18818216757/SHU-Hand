#ifndef _DRV_TIMER_H
#define _DRV_TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//Creat by xiao
//Copyright(C) Shanghai University
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define FREQ_DIV  1
#define LOAD_VALUE 5399

#define BDCMOTOR_DUTY_FULL 5300

void TIM_PWM_Init(void);
void TIM_PWM_GPIO_INIT(void);
#endif

