#include "sys.h"
#include "delay.h"
#include "drv_timer.h"
#include "common.h"
#include "drv_encoder.h"
#include "drv_usart.h"
#include "calculate_angle.h"
#include "drv_adc.h"
#include "drv_flash.h"
#include "main_function.h"

void Delay(__IO uint32_t nCount);
void Delay(__IO uint32_t nCount)
{
while(nCount--){}
}

u8 TEXT_Buffer[]={0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30, 0x31};
#define SIZE sizeof(TEXT_Buffer)

uint16_t count_1[100000] = {0};
uint16_t uhADCxConvertedValueC[FINGER_SEL_TOTAL];
uint16_t uhADCxConvertedValueP[FINGER_SEL_TOTAL - 1];
//uint16_t test_adc;
//u8 datatemp[SIZE];

int main(void)
{
  
	//int i;
	
	Cache_Enable();                 //打开L1-Cache
	HAL_Init();				              //初始化HAL库
	Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz
  delay_init(216);
	//Gpio_init();
  //__HAL_RCC_GPIOB_CLK_ENABLE();
  SHU_HAND_uart_init(115200);	
	TIM_PWM_Init();
  TIM_PWM_GPIO_INIT();
	Encoder_Gpio_Init();
  ENCODER_TIMx_Init();
  Gpio_motor_dir_init();
  Finger_Speed_PID_INIT();
	W25QXX_Init();
	//MY_ADC_Init();
	HAND_ADCC_Init();
	HAND_ADCP_Init();

	Calcuate_timer_Init();
	
	HAL_ADC_Start_DMA(&ADCC_Handler, (uint32_t*)&uhADCxConvertedValueC, FINGER_SEL_TOTAL);
	HAL_ADC_Start_DMA(&ADCP_Handler, (uint32_t*)&uhADCxConvertedValueP, FINGER_SEL_TOTAL - 1);
	
	//W25QXX_Read(datatemp,100,SIZE);	
	/*if (W25QXX_ReadID() != W25Q128)								//检测不到W25Q256
	{
		
	} else {
	  	W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE,SIZE);
		  delay_ms(1000);
		  W25QXX_Read(datatemp,FLASH_SIZE,SIZE);	
	}*/
	//Set_motor_direction(FINGER_SEL_INDEX, SERVO_DIR_UP);
  //Set_motor_speed(FINGER_SEL_INDEX, 5300);
  //Set_motor_speed(FINGER_SEL_INDEX, 0);
  //Finger_speed_Pid_paramter.position[FINGER_SEL_INDEX] = 600;
  
  //Finger_position_Pid_paramter.position[FINGER_SEL_INDEX] = -200000;
	//TIM2->CNT = 0;
	while(1) {
		hand_message_loop();
		//test_adc = Get_Adc(ADC_CHANNEL_4);
		//delay_ms(1000);
    //Set_motor_direction(FINGER_SEL_INDEX, SERVO_DIR_UP);
    //count_1[i++] =  m_finger_paramter.finger_speed[FINGER_SEL_INDEX];
    //if (i == 10000)
    //  i = 0;
    //delay_ms(10);
		/*
   for (i = 100; i < 5300; i = i + 100) {
	   Set_motor_speed(FINGER_SEL_INDEX, i);
		 delay_ms(100);
	 }
	 
	 for (i = 5200; i > 100; i = i - 100) {
	   Set_motor_speed(FINGER_SEL_INDEX, i);
		 delay_ms(100);
	 }
	 Set_motor_direction(FINGER_SEL_INDEX, SERVO_DIR_DOWN);
	 
	 for (i = 5200; i > 100; i = i - 100) {
	   Set_motor_speed(FINGER_SEL_INDEX, i);
		 delay_ms(100);
	 }
	 
	 for (i = 100; i < 5300; i = i + 100) {
	   Set_motor_speed(FINGER_SEL_INDEX, i);
		 delay_ms(100);
	 }*/
	  //Finger_position_Pid_paramter.position[FINGER_SEL_INDEX] = 250000;
  
    //delay_ms(10000);
    //Finger_position_Pid_paramter.position[FINGER_SEL_INDEX] = 5000;
    //delay_ms(10000);
    //for(i = 0; i < 5400; i = i + 100)
    //{
    //  delay_ms(1000);
    //  Set_motor_speed(FINGER_SEL_INDEX, i);
    //}
		//for (i = 0; i < 1000; i ++) {
		//  delay_ms(1000);
	  //  count[i] = i;
		//}
	}
     
	return 0;
}
