/**
  ******************************************************************************
  * @file    led.h
  * @author  ������Ц
  * @version V1
  * @date    2014.11.9
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  * @note    LED�Ƶ�C�ļ�
	* @resoruce PA17,PC0,PE26,PD15
	* @history 						
					 1,	Date:						2014.11.13
							Author:					wxwx
							Modification:		�޸�Ϊ�Ĵ����汾
					 2,	Date:						2014.11.30
							Author:					wxwx
							Modification:		ͳһ��ע�������淶�޸�



		 ����˵����LED1 PA17
							 LED2 PE26
							 LED3 PC0
							 LED4 D15
											 
											 
  ******************************************************************************
  */
#include "head.h"

/*************************************************
Function:   		��ʼ���ĸ�LED�ƣ��ֱ�����Ϊ�������
Calls: 					none
Called By:    	Sys_Init();
Input:  
								u8 led ��LED1-4  LED_ALL
Output:  				enable the led
Return:  				void
Others: 
resource:				PA17,PC0,PE26,PD15
register: 			
								SIM->SCGC5 �üĴ�����9-13λ�ֱ��ӦPTA-PTEʱ�ӣ�1����0��		
								PORTx->PCR �üĴ�������ѡ�����ŵ�ģʽ������ģʽͷ�ļ������ܹ��к�
								PTx>PDDR   x�ܽŵķ���Ĵ�����1Ϊ�����0Ϊ����
*************************************************/
void Led_Init(u8 led)
{
	if(led&LED1)
	{
		SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;			//��A��ʱ��							
		PORTA->PCR[17] &= ~PORT_PCR_MUX(0);   //MUX 000
		PORTA->PCR[17] |= PORT_PCR_MUX(1);    //MUX 001 GPIO  
		PTA->PDDR |= ((1<<17));  							//17������Ϊ��� 
	}
	if(led&LED2)
	{
		SIM->SCGC5|=SIM_SCGC5_PORTE_MASK;									
		PORTE->PCR[26] &= ~PORT_PCR_MUX(0);    		
		PORTE->PCR[26] |= PORT_PCR_MUX(1);     
		PTE->PDDR |= ((1<<26));  							
	}
	if(led&LED3)
	{
		SIM->SCGC5|=SIM_SCGC5_PORTC_MASK;									
		PORTC->PCR[0] &= ~PORT_PCR_MUX(0);    		
		PORTC->PCR[0] |= PORT_PCR_MUX(1);     
		PTC->PDDR |= ((1<<0));  			
	}
	if(led&LED4)
	{
		SIM->SCGC5|=SIM_SCGC5_PORTD_MASK;									
		PORTD->PCR[15] &= ~PORT_PCR_MUX(0);    		
		PORTD->PCR[15] |= PORT_PCR_MUX(1);     
		PTD->PDDR |= ((1<<15));  			
	}			
}

/*************************************************
Function:   		Led_On
Description			���������LED��
Calls: 					none
Called By:    	Any
Input:  
								u8 led ��LED1-4  LED_ALL
												LED1 PA17
						            LED2 PE26
											  LED3 PC0
											  LED4 D15

Output:  				����Ӧ�ܽŸ��͵�ƽ
Return:  				void
Others: 
register: 			
								PTx->PDOR		�˿�������ݼĴ���
*************************************************/
void Led_On(u8 led)
{
	if(led&LED1)
	{
		PTC->PDOR&=~(1<<0);
	}
	if(led&LED2)
	{
		PTA->PDOR&=~(1<<17);
	}
	if(led&LED3)
	{
		PTE->PDOR&=~(1<<26);
	}
	if(led&LED4)
	{
		PTD->PDOR&=~(1<<15);
	}	
}
/*Ϩ��һ��LED*/
void Led_Off(u8 led)
{
	if(led&LED1)
	{
		PTC->PDOR|=1<<0;
	}
	if(led&LED2)
	{
		PTA->PDOR|=1<<17;
	}
	if(led&LED3)
	{
		PTE->PDOR|=1<<26;
	}
	if(led&LED4)
	{
		PTD->PDOR|=1<<15;
	}		
}










