#include "drv_motor.h"

//////////////////////////////////////////////////////////////////////////////////	 
//Creat by xiao
//Copyright(C) Shanghai University
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//文件名：drv_motor.c
//文件说明：初始化电机方向控制GPIO，设置电机转动速度，调整电机转向
//V1.1 2018.11.3
//新增Gpio_motor_dir_init函数,用于电机正反转GPIO初始化
//GPOIO使用了6个：PF0~5
////////////////////////////////////////////////////////////////////////////////// 	


GPIO_TypeDef *GPIO_TYPE = GPIOF;
uint16_t GPIO_POSITIVE_PIN[FINGER_SEL_TOTAL] = {GPIO_PIN_0, GPIO_PIN_10, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5};



void Gpio_motor_dir_init(void) {
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOF_CLK_ENABLE();
  GPIO_Initure.Pin = GPIO_POSITIVE_PIN[0] | GPIO_POSITIVE_PIN[1] | GPIO_POSITIVE_PIN[2] | GPIO_POSITIVE_PIN[3] | GPIO_POSITIVE_PIN[4] | GPIO_POSITIVE_PIN[5] ;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  //推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;     //高速
	HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}


void Set_motor_direction(FINGER_SELECT f_select, SERVO_DIR_SEL servo_dir) {
    switch (servo_dir) {
		  case SERVO_DIR_UP:
			  HAL_GPIO_WritePin(GPIO_TYPE, GPIO_POSITIVE_PIN[f_select], GPIO_PIN_SET);
		    break;
		  case SERVO_DIR_DOWN:
			  HAL_GPIO_WritePin(GPIO_TYPE, GPIO_POSITIVE_PIN[f_select],GPIO_PIN_RESET);
		    break;
		  case SERVO_DIR_STOP:
				//Set_motor_speed(f_select, 0);
			  HAL_GPIO_WritePin(GPIO_TYPE, GPIO_POSITIVE_PIN[f_select],GPIO_PIN_RESET);
		    break;
		  default:
		    break;
	  }
}

void Set_motor_speed(FINGER_SELECT fingersel, uint16_t speed) {
	if (speed < 200)
		speed = 0;
  switch (fingersel)
	{
		case FINGER_SEL_THUMB:
			TIM_SetCompare1(Pwm_timer[fingersel],speed);//need to be changed
			break;
		case FINGER_SEL_INDEX:
			TIM_SetCompare2(Pwm_timer[fingersel],speed);
			break;
		case FINGER_SEL_MIDDLE:
			TIM_SetCompare1(Pwm_timer[fingersel],speed);
			break;
		case FINGER_SEL_RING:
			TIM_SetCompare1(Pwm_timer[fingersel],speed);
			break;
		case FINGER_SEL_LITTLE:
			TIM_SetCompare1(Pwm_timer[fingersel],speed);
			break;
		case FINGER_SEL_ROOT:
			TIM_SetCompare2(Pwm_timer[fingersel],speed);
			break;
		default:
			break;
	}
}

