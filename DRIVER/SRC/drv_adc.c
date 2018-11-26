#include "drv_adc.h"
#include "delay.h"
#include "common.h"
//////////////////////////////////////////////////////////////////////////////////	 
//Creat by xiao
//Copyright(C) Shanghai University
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//文件名：drv_adc.c
//文件说明：ADC 驱动，用来获取压力，电流值
//V1.1 2018.11.3
//GPOIO使用了11个：PA2, PA3, PC0~4, PF3, PF4, PF5, PF10				  
////////////////////////////////////////////////////////////////////////////////// 	

void HAND_DAMC_Init(void);
void HAND_DAMP_Init(void);

ADC_HandleTypeDef ADCC_Handler;  //ADC电流句柄
ADC_HandleTypeDef ADCP_Handler;  //ADC压力句柄

uint32_t ADC_Current_Channel[FINGER_SEL_TOTAL] = {ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_8, ADC_CHANNEL_9, ADC_CHANNEL_14, ADC_CHANNEL_15};
uint32_t ADC_Pressure_Channel[FINGER_SEL_TOTAL - 1] = {ADC_CHANNEL_10, ADC_CHANNEL_11, ADC_CHANNEL_12, ADC_CHANNEL_13, ADC_CHANNEL_14};

//初始化ADC
//ch: ADC_channels 
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
void HAND_ADCP_Init(void)
{ 
	  int i;
	  ADC_ChannelConfTypeDef ADCP_ChanConf;
    ADCP_Handler.Instance = ADC1;
    ADCP_Handler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;   //4分频，ADCCLK=PCLK2/4=108/4=27MHZ
    ADCP_Handler.Init.Resolution = ADC_RESOLUTION_12B;             //12位模式
    ADCP_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;             //右对齐
    ADCP_Handler.Init.ScanConvMode = ENABLE;                       //非扫描模式
    ADCP_Handler.Init.EOCSelection = DISABLE;                      //关闭EOC中断
    ADCP_Handler.Init.ContinuousConvMode = ENABLE;                 //连续转换
    ADCP_Handler.Init.NbrOfConversion = 5;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADCP_Handler.Init.DiscontinuousConvMode = DISABLE;             //禁止不连续采样模式
    ADCP_Handler.Init.NbrOfDiscConversion = 0;                     //不连续采样通道数为0
    ADCP_Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;       //软件触发
    ADCP_Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADCP_Handler.Init.DMAContinuousRequests = ENABLE;              //关闭DMA请求
    HAL_ADC_Init(&ADCP_Handler);                                   //初始化 
    for (i = 1; i < FINGER_SEL_TOTAL + 1; i ++) {
		  ADCP_ChanConf.Channel = ADC_Pressure_Channel[i-1];                                   //通道
      ADCP_ChanConf.Rank = i;                                       //1个序列
      ADCP_ChanConf.SamplingTime = ADC_SAMPLETIME_480CYCLES;        //采样时间
      ADCP_ChanConf.Offset = 0;                 
      HAL_ADC_ConfigChannel(&ADCP_Handler,&ADCP_ChanConf);  
		}
		HAND_DAMP_Init();
}

void HAND_DAMP_Init(void) {
    static DMA_HandleTypeDef  DMAP_Handler;
	  DMAP_Handler.Instance = DMA2_Stream0;

    DMAP_Handler.Init.Channel  =  DMA_CHANNEL_0;
    DMAP_Handler.Init.Direction = DMA_PERIPH_TO_MEMORY;
    DMAP_Handler.Init.PeriphInc = DMA_PINC_DISABLE;
    DMAP_Handler.Init.MemInc = DMA_MINC_ENABLE;
    DMAP_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    DMAP_Handler.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
    DMAP_Handler.Init.Mode = DMA_CIRCULAR;
    DMAP_Handler.Init.Priority = DMA_PRIORITY_HIGH;
    DMAP_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    DMAP_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMAP_Handler.Init.MemBurst = DMA_MBURST_SINGLE;
    DMAP_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;
	  __HAL_LINKDMA(&ADCP_Handler, DMA_Handle, DMAP_Handler);
	  HAL_DMA_DeInit(&DMAP_Handler);
    HAL_DMA_Init(&DMAP_Handler);

    
}

void HAND_ADCC_Init(void)
{ 
	  int i;
	  ADC_ChannelConfTypeDef ADCC_ChanConf;
    ADCC_Handler.Instance = ADC3;
    ADCC_Handler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;   //4分频，ADCCLK=PCLK2/4=108/4=27MHZ
    ADCC_Handler.Init.Resolution = ADC_RESOLUTION_12B;             //12位模式
    ADCC_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;             //右对齐
    ADCC_Handler.Init.ScanConvMode = ENABLE;                       //非扫描模式
    ADCC_Handler.Init.EOCSelection = DISABLE;                      //关闭EOC中断
    ADCC_Handler.Init.ContinuousConvMode = ENABLE;                 //关闭连续转换
    ADCC_Handler.Init.NbrOfConversion = 6;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADCC_Handler.Init.DiscontinuousConvMode = DISABLE;             //禁止不连续采样模式
    ADCC_Handler.Init.NbrOfDiscConversion = 0;                     //不连续采样通道数为0
    ADCC_Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;       //软件触发
    ADCC_Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADCC_Handler.Init.DMAContinuousRequests = ENABLE;              //开启DMA请求
    HAL_ADC_Init(&ADCC_Handler);                                   //初始化 
    for (i = 1; i < FINGER_SEL_TOTAL + 1; i ++) {
		  ADCC_ChanConf.Channel = ADC_Current_Channel[i-1];                                   //通道
      ADCC_ChanConf.Rank = i;                                       //1个序列
      ADCC_ChanConf.SamplingTime = ADC_SAMPLETIME_480CYCLES;        //采样时间
      ADCC_ChanConf.Offset = 0;                 
      HAL_ADC_ConfigChannel(&ADCC_Handler,&ADCC_ChanConf);  
		}
		HAND_DAMC_Init();
}

void HAND_DAMC_Init(void) {
    static DMA_HandleTypeDef  DMAC_Handler;
	  DMAC_Handler.Instance = DMA2_Stream1;

    DMAC_Handler.Init.Channel  =  DMA_CHANNEL_2;
    DMAC_Handler.Init.Direction = DMA_PERIPH_TO_MEMORY;
    DMAC_Handler.Init.PeriphInc = DMA_PINC_DISABLE;
    DMAC_Handler.Init.MemInc = DMA_MINC_ENABLE;
    DMAC_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    DMAC_Handler.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
    DMAC_Handler.Init.Mode = DMA_CIRCULAR;
    DMAC_Handler.Init.Priority = DMA_PRIORITY_HIGH;
    DMAC_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    DMAC_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMAC_Handler.Init.MemBurst = DMA_MBURST_SINGLE;
    DMAC_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;
	  __HAL_LINKDMA(&ADCC_Handler, DMA_Handle, DMAC_Handler);
	  HAL_DMA_DeInit(&DMAC_Handler);
    HAL_DMA_Init(&DMAC_Handler);

    
}

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
	  __HAL_RCC_ADC3_CLK_ENABLE();            //使能ADC3时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			      //开启GPIOA时钟
	  __HAL_RCC_GPIOC_CLK_ENABLE();			      //开启GPIOC时钟
	  __HAL_RCC_GPIOF_CLK_ENABLE();			      //开启GPIOF时钟
	  __HAL_RCC_DMA2_CLK_ENABLE();
	
    GPIO_Initure.Pin = GPIO_PIN_2 | GPIO_PIN_3;            //PA5
    GPIO_Initure.Mode = GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure.Pull = GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	  GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
	  GPIO_Initure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}

//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_Adc(u32 ch)   
{
    HAL_ADC_Start(&ADCC_Handler);                               //开启ADC
	
    HAL_ADC_PollForConversion(&ADCC_Handler,1);                //轮询转换
   
	return (u16)HAL_ADC_GetValue(&ADCC_Handler);	            //返回最近一次ADC1规则组的转换结果
}
//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

/*
ADC_HandleTypeDef ADC1_Handler;//ADC句柄

//初始化ADC
//ch: ADC_channels 
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC_Init(void)
{ 
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=108/4=27MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //非扫描模式
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
    ADC1_Handler.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //关闭DMA请求
    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 
}

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			      //开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_4;            //PA5
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //1个序列
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
	
    HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //轮询转换
   
	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	            //返回最近一次ADC1规则组的转换结果
}
//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
*/
