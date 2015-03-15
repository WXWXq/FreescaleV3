/**
  ******************************************************************************
  * @file    ccd.h
  * @author  我想我笑
  * @version V1 
  * @date    2014.12.21
	*	@chip		 龙邱开发板
	* @lib		 CHv2.5
  * @note    
	* @source  ADC1 ch8 PB0 PA10 PA11 
	* @history 			

				 
  ******************************************************************************
	*/
#include "head.h"

u8 CCD_Point[130];					//ccd采集值
/*************************************************
Function:   	初始化CCD采集，包括初始化PTI0和ADC0，1
Calls: 				ADC_Init();
							Angle_Time_Init();
Called By:    Sys_Init();
Input:  			NONE 
Output:  			NONE
Return:  			void
Resource:
Others: 
register: 	

*************************************************/
void CCD_Init(void)
{
	/*IO口初始化*/
	SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;			//打开A口时钟				SI			
	PORTE->PCR[0] &= ~PORT_PCR_MUX(0);   //MUX 000
	PORTE->PCR[0] |= PORT_PCR_MUX(1);    //MUX 001 GPIO  
	PTE->PDDR |= ((1<<0));  							//10脚设置为输出 
	PORTE->PCR[1] &= ~PORT_PCR_MUX(0);   //MUX 000						CLK
	PORTE->PCR[1] |= PORT_PCR_MUX(1);    //MUX 001 GPIO  
	PTE->PDDR |= ((1<<1));  							//11脚设置为输出

	/*adc1初始化*/	
	ADC_InitTypeDef ADC_InitStruct1;
	ADC_InitStruct1.instance = HW_ADC0;
	ADC_InitStruct1.clockDiv = kADC_ClockDiv2; /* ADC采样时钟2分频 */
	ADC_InitStruct1.resolutionMode = kADC_SingleDiff12or13;
	ADC_InitStruct1.triggerMode = kADC_TriggerSoftware; /* 软件触发转换 */
	ADC_InitStruct1.singleOrDiffMode = kADC_Single; /*单端模式 */
	ADC_InitStruct1.continueMode = kADC_ContinueConversionEnable; /* 启动连续转换 转换一次后 自动开始下一次转换*/
	ADC_InitStruct1.hardwareAveMode = kADC_HardwareAverage_32; /*禁止 硬件平均 功能 */
	ADC_InitStruct1.vref = kADC_VoltageVREF;                       /* 使用外部VERFH VREFL 作为模拟电压参考 */
	ADC_Init(&ADC_InitStruct1);
	//adc0
	SIM->SCGC3 |= SIM_SCGC3_ADC1_MASK;
	ADC0->CFG1=0;													//清零恢复默认
  ADC0->CFG1 |= ADC_CFG1_ADICLK(1)  	  //输入时钟二分频为ADC时钟
             |ADC_CFG1_ADLSMP_MASK    	// 长采样时间
             |ADC_CFG1_MODE(0)    			// MODE  00,8位精度
				     |ADC_CFG1_ADICLK(0);				//ADICLK 00,输入时钟为总线时钟 
	ADC0->CFG2=0;													//清零恢复默认
  ADC0->CFG2 |= ADC_CFG2_ADHSC_MASK    	// ADHSC   1 高速转换时序
						 |ADC_CFG2_ADLSTS(0);				//长采样时间 

  ADC0->SC2 |=ADC_SC2_REFSEL(0);        // REFSEL  00 默认参考电压为内部电压

  ADC0->SC3 |=ADC_SC3_AVGS(3)           // AVGS 32个 采样均值
            |ADC_SC3_AVGE_MASK         // AVGE 1 采样均值使能 	
						|ADC_SC3_ADCO_MASK;
						
						
	ADC0->SC1[0]&=~(ADC_SC1_ADCH_MASK);
  ADC0->SC1[0]|=ADC_SC1_ADCH(1);					//通道1  0p1
}

/*************************************************
Function:   	初始化CCD采集，包括初始化PTI0和ADC0，1
Calls: 				ADC_Init();
							Angle_Time_Init();
Called By:    Sys_Init();
Input:  			NONE 
Output:  			NONE
Return:  			void
Resource:
Others: 
register: 	

*************************************************/
void CCD_GetLine(void)
{
  u8 i;
  
  SI_L;
  CLK_L;
  Delay_us(3);
  SI_H;
  Delay_us(3);
  CLK_H;
  Delay_us(3);
  SI_L;
  Delay_us(3);
 
	while (!(ADC0->SC1[0]&ADC_SC1_COCO_MASK))
	;
  CCD_Point[0] = ADC0->R[0];	
  CLK_L;
  
  //???2~128??
  for(i=1; i<128; i++)
  {
    Delay_us(3);
    CLK_H;
    Delay_us(3); 
		while (!(ADC0->SC1[0]&ADC_SC1_COCO_MASK))
		;
    CCD_Point[i] = CCD_Point[0] = ADC0->R[0];
    CLK_L;
  }
  
  //???129?clk
   Delay_us(3);
   CLK_H;
   Delay_us(3); 
   CLK_L;
   Delay_us(3);	
}


void LD_CCD_PutData(unsigned char * ImageData)
{
	unsigned char i;
	unsigned char crc = 0;

	/* Send Data */
	while(!((UART4->S1) & UART_S1_TDRE_MASK));
	UART4->D=('*');
	while(!((UART4->S1) & UART_S1_TDRE_MASK));
	UART4->D=('L');
	while(!((UART4->S1) & UART_S1_TDRE_MASK));
	UART4->D=('D');

	CCD_SendHex(0);
	CCD_SendHex(0);
	CCD_SendHex(0);
	CCD_SendHex(0);

	for(i=0; i<128; i++) {
		CCD_SendHex(*ImageData++);
	}

	CCD_SendHex(crc);
	while(!((UART4->S1) & UART_S1_TDRE_MASK));
	UART4->D=('#');
	
}

void CCD_SendHex(unsigned char hex) 
{
	unsigned char temp;
	temp = hex >> 4;
	if(temp < 10) 
	{
		while(!((UART4->S1) & UART_S1_TDRE_MASK));
		UART4->D=(temp + '0');
	} 
	else 
	{
		while(!((UART4->S1) & UART_S1_TDRE_MASK));
		UART4->D=(temp - 10 + 'A');
	}
	temp = hex & 0x0F;
	if(temp < 10) 
	{
		while(!((UART4->S1) & UART_S1_TDRE_MASK));
		UART4->D=(temp + '0');
	} 
	else 
	{
		while(!((UART4->S1) & UART_S1_TDRE_MASK));
		UART4->D=(temp - 10 + 'A');
	}
}


