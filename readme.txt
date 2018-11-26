实验器材:
	阿波罗STM32F7开发板 
	
实验目的:
	本实验为新建工程实验，仅供大家新建工程时参考。
	新建工程详细步骤，请看《STM32F7开发指南-库函数版本》第3.3节。
	
硬件资源:
	1,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面)
	
实验现象:
	本实验下载后，DS0和DS1闪烁。 

工程文件来源：

①HAL库文件                                                                 ->   \STM32Cube_FW_F7_V1.4.0\Drivers\STM32F7xx_HAL_Driver
②startup_stm32f767xx.s                                                     ->   \STM32Cube_FW_F7_V1.4.0\Drivers\CMSIS\Device\ST\STM32F7xx\Source\Templates\arm
③cmsis_armcc.h，core_cm7.h，core_cmFunc.h，core_cmInstr.h ，core_cmSimd.h  ->   \STM32Cube_FW_F7_V1.4.0\Drivers\CMSIS\Include

④stm32f7xx.h，system_stm32f7xx.h和stm32f767xx.                               ->   \STM32Cube_FW_F7_V1.4.0\Drivers\CMSIS\Device\ST\STM32F7xx\Include

⑤stm32f7xx_it.h，stm32f7xx_hal_conf.h 和main.h                             ->   \STM32Cube_FW_F7_V1.4.0\Projects\STM32F767ZI-Nucleo\Templates\Inc
⑥system_stm32f7xx.c，stm32f7xx_it.c, stm32f7xx_hal_msp.c 和main.c          ->   \STM32Cube_FW_F7_V1.4.0\Projects\STM32F767ZI-Nucleo\Templates\Src

SYSTEM文件夹，任何一个HAL库实验工程中有
	
注意事项:
	无.


					正点原子@ALIENTEK
					2016-6-24
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://openedv.taobao.com
					http://eboard.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com