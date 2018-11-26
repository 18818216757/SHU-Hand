#include "calculate_angle.h"
#include "drv_encoder.h"
#include "sys.h"
#include "drv_timer.h"
#include "drv_motor.h"



TIM_HandleTypeDef TIM_Handler;
uint16_t encoder_num[FINGER_SEL_TOTAL] = {0};
FINGER_PARAMETER  m_finger_paramter;
uint8_t m_motor_dir[FINGER_SEL_TOTAL] = {0};
uint16_t i, j;
uint16_t count[1000] = {0};
int speed_pwm[10000] = {0};
uint8_t timer_count = 0;

volatile uint64_t g_ulMsCounters;

extern __IO uint32_t uwTick;
PID_TypeDef Finger_speed_Pid_paramter;
PID_TypeDef Finger_position_Pid_paramter;
PID_TypeDef Finger_pressure_Pid_paramter;

int32_t tmpPWM_Duty = 0;
int pwm_potion = 0;
uint8_t is_flag = 0;
int overturn_flag[FINGER_SEL_TOTAL] = {0};

void get_motor_speed_value(FINGER_SELECT fingersel);
int32_t PositionPIDCalc(FINGER_SELECT fingersel, int32_t NextPoint);

void speed_loop(void);
void position_loop(void);

void Finger_Speed_PID_INIT(void) {
	  FINGER_SELECT fingersel;
    for (fingersel = FINGER_SEL_THUMB; fingersel < FINGER_SEL_TOTAL; fingersel ++) {
			Finger_speed_Pid_paramter.LastError[fingersel] = 0;
			Finger_speed_Pid_paramter.PrevError[fingersel] = 0;
			Finger_speed_Pid_paramter.Proportion[fingersel] = 3;
			Finger_speed_Pid_paramter.Integral[fingersel] = 2;
			Finger_speed_Pid_paramter.Derivative[fingersel] = 1;
			Finger_speed_Pid_paramter.SumError[fingersel] = 0;
			
			Finger_position_Pid_paramter.LastError[fingersel] = 0;
			Finger_position_Pid_paramter.PrevError[fingersel] = 0;
			Finger_position_Pid_paramter.Proportion[fingersel] = 0.08;
			Finger_position_Pid_paramter.Integral[fingersel] = 0;
			Finger_position_Pid_paramter.Derivative[fingersel] = 0;
			Finger_position_Pid_paramter.SumError[fingersel] = 0;
		}
}

//Tout=((arr+1)*(psc+1))/Ft us.
void Calcuate_timer_Init(void)
{ 
    	
    TIM_Handler.Instance = TIM7;                          //通用定时器3
    TIM_Handler.Init.Prescaler = TIM_Prescaler;                       //分频
    TIM_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;    //向上计数器
    TIM_Handler.Init.Period = TIM_Period;                        //自动装载值
    TIM_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM_Handler);
   
    HAL_TIM_Base_Start_IT(&TIM_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE
    
}


//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance==TIM7) {
		  __HAL_RCC_TIM7_CLK_ENABLE();            
		  HAL_NVIC_SetPriority(TIM7_IRQn,1,3);
		  HAL_NVIC_EnableIRQ(TIM7_IRQn); 
	  }  
}

void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM_Handler);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    int32_t number;
	  FINGER_SELECT fingersel;
    if(htim == (&TIM_Handler)) {	
			for (fingersel = FINGER_SEL_THUMB; fingersel < FINGER_SEL_TOTAL; fingersel ++) {
			  m_finger_paramter.now_encoder_num[fingersel] = Encoder_timer[fingersel]->CNT;
        number = m_finger_paramter.now_encoder_num[fingersel] - m_finger_paramter.last_encoder_num[fingersel];
        if (number > ENCODER_MAX_VALUE) {
          number = ENCODER_TIM_PERIOD - number;
          overturn_flag[fingersel] = overturn_flag[fingersel] - 1;
					m_motor_dir[fingersel] = MOTOR_REVERSEL;
        } else if (number < - ENCODER_MAX_VALUE) {
          number = ENCODER_TIM_PERIOD + number;
					m_motor_dir[fingersel] = MOTOR_FOREWARD;
		      overturn_flag[fingersel] = overturn_flag[fingersel] + 1;
        } else {
					if (number < 0) {
            number = - number;
            m_motor_dir[fingersel] = MOTOR_REVERSEL;
          } else {
            m_motor_dir[fingersel] = MOTOR_FOREWARD;
          }
				}
          
        m_finger_paramter.finger_speed[fingersel] = number << 3;
        m_finger_paramter.total_encoder_num[fingersel] = m_finger_paramter.now_encoder_num[fingersel] + overturn_flag[fingersel] * ENCODER_TIM_PERIOD;
        m_finger_paramter.last_encoder_num[fingersel] = m_finger_paramter.now_encoder_num[fingersel];
			}
    }
}


void HAL_SYSTICK_Callback(void) {
	
  if (uwTick % 20 == 0) {
	  //position_loop();
	} else if (uwTick % 2 == 0) {
    //speed_loop();
	}
}

uint64_t sys_millis(void) {
  return uwTick;
}


void position_loop(void) {
	FINGER_SELECT fingersel;
  for (fingersel = FINGER_SEL_THUMB; fingersel < FINGER_SEL_TOTAL; fingersel ++) {
		pwm_potion = PositionPIDCalc(fingersel, m_finger_paramter.total_encoder_num[fingersel]);
		
		if (pwm_potion < -30) {
			if (pwm_potion <= -1000)
        pwm_potion = -1000;
			if (m_finger_paramter.finger_dir[fingersel] == SERVO_DIR_STOP)
				return;
			Set_motor_direction(fingersel, SERVO_DIR_DOWN);
		  pwm_potion = - pwm_potion;
      m_finger_paramter.finger_dir[fingersel] = SERVO_DIR_DOWN;
			Finger_speed_Pid_paramter.position[fingersel] = pwm_potion;
		} else if (pwm_potion > 30) {
			if (pwm_potion >= 1000)
        pwm_potion = 1000;
			if (m_finger_paramter.finger_dir[fingersel] == SERVO_DIR_STOP)
				return;
      m_finger_paramter.finger_dir[fingersel] = SERVO_DIR_UP;
		  Set_motor_direction(fingersel, SERVO_DIR_UP);
			Finger_speed_Pid_paramter.position[fingersel] = pwm_potion;
		} else {
			Finger_speed_Pid_paramter.position[fingersel] = 0;
			if (m_motor_dir[fingersel] == MOTOR_FOREWARD)
        Set_motor_speed(fingersel, 0);
      else 
        Set_motor_speed(fingersel, BDCMOTOR_DUTY_FULL);
		}
	}
}

void speed_loop(void) {
	
	FINGER_SELECT fingersel;
  if (!is_flag) return;
  for (fingersel = FINGER_SEL_THUMB; fingersel < FINGER_SEL_TOTAL; fingersel ++) {
		tmpPWM_Duty += SpeedPIDCalc(fingersel, m_finger_paramter.finger_speed[fingersel]);
		if (tmpPWM_Duty >= BDCMOTOR_DUTY_FULL)
      tmpPWM_Duty = BDCMOTOR_DUTY_FULL;
    if (tmpPWM_Duty <= -BDCMOTOR_DUTY_FULL)
      tmpPWM_Duty = -BDCMOTOR_DUTY_FULL;
	  if (tmpPWM_Duty < 0)
		  tmpPWM_Duty = -tmpPWM_Duty;
    if (m_finger_paramter.finger_dir[fingersel] == SERVO_DIR_DOWN) {
      Set_motor_speed(fingersel, BDCMOTOR_DUTY_FULL - tmpPWM_Duty);
    } else if (m_finger_paramter.finger_dir[fingersel] == SERVO_DIR_UP) {
      Set_motor_speed(fingersel, tmpPWM_Duty);
    }        
	}
}


int32_t SpeedPIDCalc(FINGER_SELECT fingersel, uint16_t NextPoint) {
  float iError,iIncpid;
  iError = Finger_speed_Pid_paramter.position[fingersel] - NextPoint; //偏差

  /* 死区限定 */
  if((iError < SPEED_GUID ) && (iError > -SPEED_GUID))
    iError = 0.0f;
	iIncpid = Finger_speed_Pid_paramter.Proportion[fingersel] * iError - Finger_speed_Pid_paramter.Integral[fingersel] * Finger_speed_Pid_paramter.LastError[fingersel] + 
	          Finger_speed_Pid_paramter.Derivative[fingersel] * Finger_speed_Pid_paramter.PrevError[fingersel];
	Finger_speed_Pid_paramter.PrevError[fingersel] = Finger_speed_Pid_paramter.LastError[fingersel];
	Finger_speed_Pid_paramter.LastError[fingersel] = iError;
	return iIncpid;
}

int32_t PositionPIDCalc(FINGER_SELECT fingersel, int32_t NextPoint) {
  int iError,iIncpid;
  
	iError = Finger_position_Pid_paramter.position[fingersel] - NextPoint;
  if((iError < POSITION_GUID ) && (iError > -POSITION_GUID))
    iError = 0;
	iIncpid = (int)((Finger_position_Pid_paramter.Proportion[fingersel] * (float)iError)                  //E[k]项
                  - (Finger_position_Pid_paramter.Integral[fingersel] * (float)Finger_position_Pid_paramter.LastError[fingersel])     //E[k-1]项
                  + (Finger_position_Pid_paramter.Derivative[fingersel] * (float)Finger_position_Pid_paramter.PrevError[fingersel]));
	Finger_position_Pid_paramter.PrevError[fingersel] = Finger_speed_Pid_paramter.LastError[fingersel];
  Finger_position_Pid_paramter.LastError[fingersel] = iError;
  is_flag = 1;
  return (iIncpid);
}

















