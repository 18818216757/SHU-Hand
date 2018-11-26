#include "drv_timer.h"
#include "drv_adc.h"
#include "common.h"
#include "calculate_angle.h"
#include "transmit_protocol.h"
#include "main_function.h"
#include  <string.h>
#include "stdlib.h"

//////////////////////////////////////////////////////////////////////////////////	 
//Creat by xiao
//Copyright(C) Shanghai University
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//文件名：main_function.c
//文件说明：主循环程序，获取手机压力值和电机电流值
//V1.1 2018.11.3				  
////////////////////////////////////////////////////////////////////////////////// 	

/*static uint8_t  posture_demo_value[11][16] = {
	{0xAA,0x52,0x00,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x0A,0x00,0x02,0x50,0xAF,0x0D}, 						//1
	{0xAA,0x52,0x00,0x00,0x00,0x40,0x0F,0x00,0x0F,0x00,0x0A,0x00,0x02,0x50,0xE0,0x0D},						//2
	{0xAA,0x52,0x00,0x00,0x00,0x40,0x00,0x40,0x0F,0x00,0x0A,0x00,0x02,0x50,0xAF,0x0D},						//3
	{0xAA,0x52,0x00,0x00,0x00,0x40,0x00,0x40,0x00,0x40,0x0A,0x00,0x02,0x50,0xE0,0x0D},						//4
	{0xAA,0x52,0x00,0x00,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0xB8,0x0D},						//5
	{0xAA,0x52,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x00,0x40,0x00,0x40,0x00,0x40,0xB7,0x0D},						//6
	{0xAA,0x52,0x0B,0x00,0x0B,0x00,0x0F,0x00,0x0F,0x00,0x06,0x00,0x03,0x00,0xFD,0x0D},						//7
	{0xAA,0x52,0x00,0x40,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x00,0x40,0x00,0x40,0xB7,0x0D},						//8
	{0xAA,0x52,0x08,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x02,0x50,0xA2,0x0D},						//9
	{0xAA,0x52,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0xF8,0x0D},						//0
  {0xAA,0x52,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0xF8,0x0D},
};*/

static char init_flag = 0;
Hand_Value Hand_value;

static unsigned int Current_Count[FINGER_SEL_TOTAL] = {0};

//static unsigned char New_Reiceive_Flag = 0;
//static unsigned char New_Message_Flag = 0;

unsigned char Current_Over_Flag[FINGER_SEL_TOTAL] = {0};

//static unsigned char start_demo_flag = 0;

//static uint16_t  piece_together(uint8_t Parameter1,uint8_t 	Parameter2);


static void Pressure_sensor_Getvalue(void)
{
  FINGER_SELECT Fingersel;
  uint8_t i_temp;

  for (Fingersel = FINGER_SEL_THUMB ; Fingersel < FINGER_SEL_TOTAL ; Fingersel ++) {
    Hand_value.Hand_Pressure_value[Fingersel][Hand_value_new] = uhADCxConvertedValueP[Fingersel];

    Hand_value.Hand_Pressure_value[Fingersel][Hand_value_average] = (Hand_value.Hand_Pressure_value[Fingersel][Hand_value_new] + Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old1]
		                                                                 + Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old2] + Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old3])>>2;

    Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old3] = Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old2];
    Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old2] = Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old1];
    Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old1] = Hand_value.Hand_Pressure_value[Fingersel][Hand_value_new];

    if (Hand_value.Hand_Pressure_value[Fingersel][Hand_value_average] > 0xFA0)
      Hand_value.Hand_Pressure_value[Fingersel][Hand_value_average] = 0xFA0;
  }
}

static void Current_sensor_Getvalue(void)
{
  FINGER_SELECT Fingersel;
  uint8_t i_temp;

  for(Fingersel = FINGER_SEL_THUMB ; Fingersel < FINGER_SEL_TOTAL - 1; Fingersel ++) {
    Hand_value.Hand_Current_value[Fingersel][Hand_value_new] = uhADCxConvertedValueC[Fingersel];

    Hand_value.Hand_Current_value[Fingersel][Hand_value_average] = (Hand_value.Hand_Pressure_value[Fingersel][Hand_value_new] + Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old1]
			                                                              + Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old2] + Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old3])>>2;

    Hand_value.Hand_Current_value[Fingersel][Hand_value_old3] = Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old2];
    Hand_value.Hand_Current_value[Fingersel][Hand_value_old2] = Hand_value.Hand_Pressure_value[Fingersel][Hand_value_old1];
    Hand_value.Hand_Current_value[Fingersel][Hand_value_old1] = Hand_value.Hand_Pressure_value[Fingersel][Hand_value_new];

    if (Hand_value.Hand_Current_value[Fingersel][Hand_value_average] > 0xFA0)
      Hand_value.Hand_Current_value[Fingersel][Hand_value_average] = 0xFA0;
  }
}


 void get_hand_sensor_value(void)  //每隔2ms更新一次传感器数据
{
	FINGER_SELECT Fingersel;
	static unsigned long speed_record_time = 0;
	if (sys_millis() - speed_record_time > 2) {
		speed_record_time = sys_millis();
		Pressure_sensor_Getvalue();
		Current_sensor_Getvalue(); 
	}
}




static void Hand_Parameter_Value_Get(void)
{
	static unsigned long speed_record_time = 0;
	
	if (sys_millis() - speed_record_time > 50) {
		PackData _send;
    FINGER_SELECT Fingersel;
		PackInterface *pack_interface = get_pack_interface_handle();
		speed_record_time = sys_millis();		//
		if (pack_interface->pack_line_state[Value_Getflag] == LINE_READY) {
					//uint8_t len = pack_interface->pack_Protocol->pack_len;
			uint8_t *temp = (uint8_t *)&_send;

			for (Fingersel = FINGER_SEL_THUMB; Fingersel < FINGER_SEL_TOTAL; Fingersel ++)
				_send.data[Fingersel] = Finger_position_Pid_paramter.position[Fingersel] >> 8;  //将手指压力的平均值转化为2个8位数据存储在数组中，并发送到PC端
      for (Fingersel = FINGER_SEL_THUMB; Fingersel < FINGER_SEL_TOTAL - 1; Fingersel ++)
			  _send.data[FINGER_SEL_TOTAL + Fingersel] = Finger_pressure_Pid_paramter.position[Fingersel] >> 8;  //将手指压力的平均值转化为2个8位数据存储在数组中，并发送到PC端

			protocol_uart_send_pc_msg(temp, 2 * FINGER_SEL_TOTAL - 1, pack_interface->pack_state);  //调用函数发送数据
			pack_interface->pack_line_state[Value_Getflag] = LINE_IDLE;
		}
	}
}



static void Hand_Parameter_Value_Set() {
  int i =0;
	static uint32_t speed_record_time = 0;
	if (sys_millis() - speed_record_time > 50) {
    FINGER_SELECT Fingersel;
		PackInterface *pack_interface = get_pack_interface_handle();
		speed_record_time = sys_millis();
		if (pack_interface->pack_line_state[Value_Setflag] == LINE_READY) { 
			for(Fingersel = FINGER_SEL_THUMB; Fingersel < FINGER_SEL_TOTAL; Fingersel ++)
				Finger_position_Pid_paramter.position[Fingersel] = pack_interface->pack_Protocol->pack_data[Fingersel];
			for(Fingersel = FINGER_SEL_THUMB; Fingersel < FINGER_SEL_TOTAL - 1; Fingersel ++)
				Finger_pressure_Pid_paramter.position[Fingersel] = pack_interface->pack_Protocol->pack_data[Fingersel + FINGER_SEL_TOTAL];
			pack_interface->pack_line_state[Value_Setflag] = LINE_IDLE;
		}
	}
}


/*static uint16_t  piece_together(uint8_t Parameter1,uint8_t 	Parameter2)//将两个8位数据转化为一个16位的数据
{
		uint16_t Parameter3;
	  Parameter3 = Parameter1<<8;
		Parameter3 = Parameter3 +Parameter2;
    return Parameter3;
}*/

/*static void Demo_Start(void)
{
    static unsigned long speed_record_time = 0;
		if(sys_millis() - speed_record_time > 50)
		{

				PackInterface *pack_interface = get_pack_interface_handle();
				speed_record_time = sys_millis();		//
				if(pack_interface->pack_line_state[Demo_flag] == LINE_READY)
				{
					//start_demo_flag = 1;
					pack_interface->pack_line_state[Demo_flag] = LINE_IDLE;
				}
		}

}*/

void Main_Struct(void)
{
  static int i = 0 , j = 0;
  uint8_t f_select = 0;
  FINGER_SELECT fingers_choose;
	static unsigned long speed_record_time = 0;
	/*if(sys_millis() - speed_record_time > 5000 && start_demo_flag == 1) {
		speed_record_time = sys_millis();
    for (j = 0 ; j < 12; j = j+2) {
			f_select = f_select + 1;
		}
	  New_Reiceive_Flag = 1;
		if ((i++)> 9)
			i = 0;
	}*/


  for(fingers_choose = FINGER_SEL_THUMB; fingers_choose < FINGER_SEL_ROOT; fingers_choose ++)
	{
    
  }
	
}

void Current_Protect(void)
{
  FINGER_SELECT Fingersel;
	static uint32_t speed_record_time = 0;
	if (sys_millis() - speed_record_time < 50)
		return;
	speed_record_time = sys_millis();
  for (Fingersel = FINGER_SEL_THUMB; Fingersel < FINGER_SEL_TOTAL; Fingersel ++) {
    if (Hand_value.Hand_Current_value[Fingersel][Hand_value_average] > 1000 ) {
      Current_Count[Fingersel] ++;
      if (Current_Count[Fingersel] > 150) {
				if (Finger_pressure_Pid_paramter.position[Fingersel] == 0)
          Current_Over_Flag[Fingersel] = 1;
				Current_Count[Fingersel] = 0;
      }
    } else {
      Current_Count[Fingersel] = 0;
	  }
	}
}



void hand_message_loop(void)//循环检测当前的标志位，并执行相应的命令
{
		get_hand_sensor_value();  //获取角度，压力，电流值
	  Hand_Parameter_Value_Get();
	  Hand_Parameter_Value_Set();
}
