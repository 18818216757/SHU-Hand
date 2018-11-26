#include "drv_adc.h"
#include "delay.h"
#include "common.h"
//////////////////////////////////////////////////////////////////////////////////	 
//Creat by xiao
//Copyright(C) Shanghai University
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//�ļ�����drv_adc.c
//�ļ�˵����ADC ������������ȡѹ��������ֵ
//V1.1 2018.11.3
//GPOIOʹ����11����PA2, PA3, PC0~4, PF3, PF4, PF5, PF10				  
////////////////////////////////////////////////////////////////////////////////// 	

void HAND_DAMC_Init(void);
void HAND_DAMP_Init(void);

ADC_HandleTypeDef ADCC_Handler;  //ADC�������
ADC_HandleTypeDef ADCP_Handler;  //ADCѹ�����

uint32_t ADC_Current_Channel[FINGER_SEL_TOTAL] = {ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_8, ADC_CHANNEL_9, ADC_CHANNEL_14, ADC_CHANNEL_15};
uint32_t ADC_Pressure_Channel[FINGER_SEL_TOTAL - 1] = {ADC_CHANNEL_10, ADC_CHANNEL_11, ADC_CHANNEL_12, ADC_CHANNEL_13, ADC_CHANNEL_14};

//��ʼ��ADC
//ch: ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
void HAND_ADCP_Init(void)
{ 
	  int i;
	  ADC_ChannelConfTypeDef ADCP_ChanConf;
    ADCP_Handler.Instance = ADC1;
    ADCP_Handler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;   //4��Ƶ��ADCCLK=PCLK2/4=108/4=27MHZ
    ADCP_Handler.Init.Resolution = ADC_RESOLUTION_12B;             //12λģʽ
    ADCP_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADCP_Handler.Init.ScanConvMode = ENABLE;                       //��ɨ��ģʽ
    ADCP_Handler.Init.EOCSelection = DISABLE;                      //�ر�EOC�ж�
    ADCP_Handler.Init.ContinuousConvMode = ENABLE;                 //����ת��
    ADCP_Handler.Init.NbrOfConversion = 5;                         //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADCP_Handler.Init.DiscontinuousConvMode = DISABLE;             //��ֹ����������ģʽ
    ADCP_Handler.Init.NbrOfDiscConversion = 0;                     //����������ͨ����Ϊ0
    ADCP_Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;       //�������
    ADCP_Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    ADCP_Handler.Init.DMAContinuousRequests = ENABLE;              //�ر�DMA����
    HAL_ADC_Init(&ADCP_Handler);                                   //��ʼ�� 
    for (i = 1; i < FINGER_SEL_TOTAL + 1; i ++) {
		  ADCP_ChanConf.Channel = ADC_Pressure_Channel[i-1];                                   //ͨ��
      ADCP_ChanConf.Rank = i;                                       //1������
      ADCP_ChanConf.SamplingTime = ADC_SAMPLETIME_480CYCLES;        //����ʱ��
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
    ADCC_Handler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;   //4��Ƶ��ADCCLK=PCLK2/4=108/4=27MHZ
    ADCC_Handler.Init.Resolution = ADC_RESOLUTION_12B;             //12λģʽ
    ADCC_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADCC_Handler.Init.ScanConvMode = ENABLE;                       //��ɨ��ģʽ
    ADCC_Handler.Init.EOCSelection = DISABLE;                      //�ر�EOC�ж�
    ADCC_Handler.Init.ContinuousConvMode = ENABLE;                 //�ر�����ת��
    ADCC_Handler.Init.NbrOfConversion = 6;                         //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADCC_Handler.Init.DiscontinuousConvMode = DISABLE;             //��ֹ����������ģʽ
    ADCC_Handler.Init.NbrOfDiscConversion = 0;                     //����������ͨ����Ϊ0
    ADCC_Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;       //�������
    ADCC_Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    ADCC_Handler.Init.DMAContinuousRequests = ENABLE;              //����DMA����
    HAL_ADC_Init(&ADCC_Handler);                                   //��ʼ�� 
    for (i = 1; i < FINGER_SEL_TOTAL + 1; i ++) {
		  ADCC_ChanConf.Channel = ADC_Current_Channel[i-1];                                   //ͨ��
      ADCC_ChanConf.Rank = i;                                       //1������
      ADCC_ChanConf.SamplingTime = ADC_SAMPLETIME_480CYCLES;        //����ʱ��
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

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
	  __HAL_RCC_ADC3_CLK_ENABLE();            //ʹ��ADC3ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			      //����GPIOAʱ��
	  __HAL_RCC_GPIOC_CLK_ENABLE();			      //����GPIOCʱ��
	  __HAL_RCC_GPIOF_CLK_ENABLE();			      //����GPIOFʱ��
	  __HAL_RCC_DMA2_CLK_ENABLE();
	
    GPIO_Initure.Pin = GPIO_PIN_2 | GPIO_PIN_3;            //PA5
    GPIO_Initure.Mode = GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull = GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	  GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
	  GPIO_Initure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
u16 Get_Adc(u32 ch)   
{
    HAL_ADC_Start(&ADCC_Handler);                               //����ADC
	
    HAL_ADC_PollForConversion(&ADCC_Handler,1);                //��ѯת��
   
	return (u16)HAL_ADC_GetValue(&ADCC_Handler);	            //�������һ��ADC1�������ת�����
}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
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
ADC_HandleTypeDef ADC1_Handler;//ADC���

//��ʼ��ADC
//ch: ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC_Init(void)
{ 
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4��Ƶ��ADCCLK=PCLK2/4=108/4=27MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12λģʽ
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //��ɨ��ģʽ
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //�ر�EOC�ж�
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //�ر�����ת��
    ADC1_Handler.Init.NbrOfConversion=1;                         //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //�ر�DMA����
    HAL_ADC_Init(&ADC1_Handler);                                 //��ʼ�� 
}

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			      //����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_4;            //PA5
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //ͨ��
    ADC1_ChanConf.Rank=1;                                       //1������
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //����ʱ��
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
	
    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //��ѯת��
   
	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	            //�������һ��ADC1�������ת�����
}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
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
