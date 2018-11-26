#ifndef HAND_MAIN_FUCTION_H
#define HAND_MAIN_FUCTION_H

#include "common.h"

#define Controlled_Min_Vlaue 50


//extern unsigned char New_Reiceive_Flag;
//extern unsigned char Current_Over_Flag[6];

extern uint16_t uhADCxConvertedValueC[FINGER_SEL_TOTAL];
extern uint16_t uhADCxConvertedValueP[FINGER_SEL_TOTAL - 1];

void hand_message_loop(void);
extern void get_hand_sensor_value(void);
extern void Current_Protect(void);
extern void Main_Struct(void);
#endif
