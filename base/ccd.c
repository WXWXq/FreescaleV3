/**
  ******************************************************************************
  * @file    ccd.h
  * @author  ������Ц
  * @version V1 
  * @date    2014.12.21
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  * @note    
	* @source  ADC1 ch8 PB0 PA10 PA11 
	* @history 			

				 
  ******************************************************************************
	*/
#include "head.h"

u8 CCD_Point[130];					//ccd�ɼ�ֵ
/*************************************************
Function:   	��ʼ��CCD�ɼ���������ʼ��PTI0��ADC0��1
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
	/*IO�ڳ�ʼ��*/
	SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;			//��A��ʱ��				SI			
	PORTE->PCR[0] &= ~PORT_PCR_MUX(0);   //MUX 000
	PORTE->PCR[0] |= PORT_PCR_MUX(1);    //MUX 001 GPIO  
	PTE->PDDR |= ((1<<0));  							//10������Ϊ��� 
	PORTE->PCR[1] &= ~PORT_PCR_MUX(0);   //MUX 000						CLK
	PORTE->PCR[1] |= PORT_PCR_MUX(1);    //MUX 001 GPIO  
	PTE->PDDR |= ((1<<1));  							//11������Ϊ���

	/*adc1��ʼ��*/	
	ADC_InitTypeDef ADC_InitStruct1;
	ADC_InitStruct1.instance = HW_ADC0;
	ADC_InitStruct1.clockDiv = kADC_ClockDiv2; /* ADC����ʱ��2��Ƶ */
	ADC_InitStruct1.resolutionMode = kADC_SingleDiff12or13;
	ADC_InitStruct1.triggerMode = kADC_TriggerSoftware; /* �������ת�� */
	ADC_InitStruct1.singleOrDiffMode = kADC_Single; /*����ģʽ */
	ADC_InitStruct1.continueMode = kADC_ContinueConversionEnable; /* ��������ת�� ת��һ�κ� �Զ���ʼ��һ��ת��*/
	ADC_InitStruct1.hardwareAveMode = kADC_HardwareAverage_32; /*��ֹ Ӳ��ƽ�� ���� */
	ADC_InitStruct1.vref = kADC_VoltageVREF;                       /* ʹ���ⲿVERFH VREFL ��Ϊģ���ѹ�ο� */
	ADC_Init(&ADC_InitStruct1);
	//adc0
	SIM->SCGC3 |= SIM_SCGC3_ADC1_MASK;
	ADC0->CFG1=0;													//����ָ�Ĭ��
  ADC0->CFG1 |= ADC_CFG1_ADICLK(1)  	  //����ʱ�Ӷ���ƵΪADCʱ��
             |ADC_CFG1_ADLSMP_MASK    	// ������ʱ��
             |ADC_CFG1_MODE(0)    			// MODE  00,8λ����
				     |ADC_CFG1_ADICLK(0);				//ADICLK 00,����ʱ��Ϊ����ʱ�� 
	ADC0->CFG2=0;													//����ָ�Ĭ��
  ADC0->CFG2 |= ADC_CFG2_ADHSC_MASK    	// ADHSC   1 ����ת��ʱ��
						 |ADC_CFG2_ADLSTS(0);				//������ʱ�� 

  ADC0->SC2 |=ADC_SC2_REFSEL(0);        // REFSEL  00 Ĭ�ϲο���ѹΪ�ڲ���ѹ

  ADC0->SC3 |=ADC_SC3_AVGS(3)           // AVGS 32�� ������ֵ
            |ADC_SC3_AVGE_MASK         // AVGE 1 ������ֵʹ�� 	
						|ADC_SC3_ADCO_MASK;
						
						
	ADC0->SC1[0]&=~(ADC_SC1_ADCH_MASK);
  ADC0->SC1[0]|=ADC_SC1_ADCH(1);					//ͨ��1  0p1
}

/*************************************************
Function:   	��ʼ��CCD�ɼ���������ʼ��PTI0��ADC0��1
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


