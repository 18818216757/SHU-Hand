#ifndef _COMMON_H
#define _COMMON_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//Creat by xiao
//Copyright(C) Shanghai University
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define BDCMOTOR_SPEED_FULL  1000
#define FALSE 0
#define TRUE  1
#define HAND_FLASH_SIZE   1024*1024

typedef enum{
	Hand_value_new      = 0,
	Hand_value_old1,				
	Hand_value_old2,
	Hand_value_old3,
	Hand_value_average,
	Hand_value_total
} hand_value_define;

typedef enum{
	FINGER_SEL_THUMB,
	FINGER_SEL_INDEX,
	FINGER_SEL_MIDDLE,
	FINGER_SEL_RING,
	FINGER_SEL_LITTLE,
	FINGER_SEL_ROOT,
	FINGER_SEL_TOTAL        = 6,
} FINGER_SELECT;

typedef enum{
	SERVO_DIR_UP				= 0,
	SERVO_DIR_DOWN,
	SERVO_DIR_STOP
} SERVO_DIR_SEL;

typedef struct{
	int8_t data[12];
}__attribute__((packed)) PackData;

typedef struct{
			int16_t    Hand_Pressure_value[FINGER_SEL_TOTAL -1][Hand_value_total];
			int16_t    Hand_Current_value[FINGER_SEL_TOTAL][Hand_value_total];	
} Hand_Value;


typedef struct{
  int             total_encoder_num[FINGER_SEL_TOTAL];
	int             last_encoder_num[FINGER_SEL_TOTAL];
  int             now_encoder_num[FINGER_SEL_TOTAL];
	int             finger_angle[FINGER_SEL_TOTAL];
	int             finger_speed[FINGER_SEL_TOTAL];
  SERVO_DIR_SEL   finger_dir[FINGER_SEL_TOTAL];
} FINGER_PARAMETER;

typedef struct
{
  int32_t       position[FINGER_SEL_TOTAL];                                 //设定目标 Desired Value
  int32_t       SumError[FINGER_SEL_TOTAL];                                 //误差累计
  float         Proportion[FINGER_SEL_TOTAL];                               //比例常数 Proportional Const
  float         Integral[FINGER_SEL_TOTAL];                                 //积分常数 Integral Const
  float         Derivative[FINGER_SEL_TOTAL];                               //微分常数 Derivative Const
  int32_t       LastError[FINGER_SEL_TOTAL];                                //Error[-1]
  int32_t       PrevError[FINGER_SEL_TOTAL];                                //Error[-2]
} PID_TypeDef;


typedef struct{
      //uint8_t     Angle_rec_state;                       //一个包在读取过程中的状态
  uint16_t    Angle_Value_rec[FINGER_SEL_TOTAL];
	uint16_t    Pressure_Value_rec[FINGER_SEL_ROOT];
	uint16_t    Current_Value_rec[FINGER_SEL_TOTAL];
} HandValue_Rec;

typedef struct{
      //uint8_t     Angle_rec_state;                       //一个包在读取过程中的状态
  uint16_t    Angle_Value_start[FINGER_SEL_TOTAL];
			
} Angle_Value_Start;

extern void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1);
extern void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare1);
extern void TIM_SetCompare3(TIM_TypeDef* TIMx, uint32_t Compare1);
extern void TIM_SetCompare4(TIM_TypeDef* TIMx, uint32_t Compare1);
#endif

