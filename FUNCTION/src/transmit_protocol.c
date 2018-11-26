#include "transmit_protocol.h"
#include "common.h"

static void rev_pack_extract(void);

PackProtocol receive_Pack;
PackInterface pack_interface;



void protocol_uart_send_pc_msg( uint8_t *Dat, uint8_t Len, UART_CMD_Type cmd)
{
    uint8_t   SendBuffer[100];
    uint8_t   i = 0, j;
    uint16_t  Crc16Value;
		static uint8_t check_sum = 0;
    SendBuffer[i++] = UART_MSG_PACKAGE_STARTCHAR;
    SendBuffer[i++] = HAND_DIRVER_ADR;
    SendBuffer[i++] = cmd;
    SendBuffer[i++] = Len;
    for( j = 0; j < Len; j++ )
    {
        SendBuffer[i++] = Dat[j];
    }

		for(j=0;j<i;j++)
		{
				check_sum=check_sum^SendBuffer[j];

		}
		SendBuffer[i++] = check_sum;
    SendBuffer[i++] = UART_MSG_PACKAGE_ENDCHAR;

		check_sum = 0;
    SHU_HAND_usart_send(SendBuffer, i);
}


static u8 _serial_kinect_handle(uint8_t data)
{
    static uint8_t pos_rev = 0;
    static uint8_t buffer_index = 0;
    static uint8_t data_len = 0;
		static uint8_t check_sum = 0;
		uint8_t return_status = 0;
    switch (pos_rev) {
      case 0:                                         //0xAA
        if(data == 0xAA) pos_rev ++;
        break;
      case 1:				//cmd
        receive_Pack.pack_cmd = data;
        pos_rev ++;
        break;
      case 2:
			  if(data > 12)
				  goto pack_error;			//limit the len of data
        receive_Pack.pack_len = data;
        data_len =  data;
        pos_rev ++;
        break;
      case 3:
        if (0 == data_len) { //caution: Jump to case 6 directly when the len==0 means that no data saved to buffer
          pos_rev ++;
			  } else {
          receive_Pack.pack_data[buffer_index++] = data;
          data_len--;
          break;
        }
      case 4:
			  if (data == check_sum) {
				  pos_rev ++;
				  check_sum = 0;
					break;
				}
				else goto pack_error;
      case 5:
        if (data == 0x0d) {//&& crc_reiceive_pack_check(&receive_Pack)) {
          pos_rev = 0;
					buffer_index = 0;
					check_sum = 0;
						//USART_SendData(Uart1_BASE,0xFF);
          return receive_Pack.pack_cmd;              //return the right pack style;
        }
        else goto pack_error;
        break;
      default:
        pos_rev = 0;
        break;
    }
	  check_sum ^= data;
	  return pos_rev;

pack_error:
	  return_status = pos_rev + 0xd0;
	  pos_rev = 0;
	  buffer_index = 0;
	  //check_sum=0;
    return return_status;                             //return the error steps of pack where error ocoured;
}


static void _serial_hand_rx_t(uint8_t data)
{
  pack_interface.pack_state = _serial_kinect_handle(data);

  switch (pack_interface.pack_state) {
	  case UARTCMD_HEART_BIT:			
			if(pack_interface.pack_line_state[Heart_Getflag] != LINE_READY)
				pack_interface.pack_line_state[Heart_Getflag] = LINE_READY;
			break;
		case UARTCMD_Get_Value:			
			pack_interface.pack_line_state[Value_Getflag] = LINE_READY;
			break;
		case UARTCMD_Set_Value:     
			pack_interface.pack_line_state[Value_Setflag] = LINE_READY;
			break;
		case UARTCMD_Start_Demo:            
			pack_interface.pack_line_state[Demo_flag] = LINE_READY;
			break;
		default:   
			break;
  }
}



uint8_t init_hand_com(void)
{
	  pack_interface.pack_Protocol = &receive_Pack;
		pack_interface.pack_line_state[Heart_Getflag] = LINE_IDLE;
		pack_interface.pack_line_state[Value_Getflag] = LINE_IDLE;	
		pack_interface.pack_line_state[Value_Setflag] = LINE_IDLE;
	  pack_interface.pack_line_state[Demo_flag] = LINE_IDLE;
    uart_hand_backcall(_serial_hand_rx_t);
	  return 1;
}  

PackInterface *get_pack_interface_handle(void)
{
	  return &pack_interface;
}  
