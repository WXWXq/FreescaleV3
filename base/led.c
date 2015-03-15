/**
  ******************************************************************************
  * @file    led.h
  * @author  我想我笑
  * @version V1
  * @date    2014.11.9
	*	@chip		 龙邱开发板
	* @lib		 CHv2.5
  * @note    LED灯的C文件
	* @resoruce PA17,PC0,PE26,PD15
	* @history 						
					 1,	Date:						2014.11.13
							Author:					wxwx
							Modification:		修改为寄存器版本
					 2,	Date:						2014.11.30
							Author:					wxwx
							Modification:		统一对注释做出规范修改



		 引脚说明：LED1 PA17
							 LED2 PE26
							 LED3 PC0
							 LED4 D15
											 
											 
  ******************************************************************************
  */
#include "head.h"

/*************************************************
Function:   		初始化四个LED灯，分别设置为推挽输出
Calls: 					none
Called By:    	Sys_Init();
Input:  
								u8 led ：LED1-4  LED_ALL
Output:  				enable the led
Return:  				void
Others: 
resource:				PA17,PC0,PE26,PD15
register: 			
								SIM->SCGC5 该寄存器的9-13位分别对应PTA-PTE时钟，1开，0关		
								PORTx->PCR 该寄存器可以选择引脚的模式，各个模式头文件后者能够有宏
								PTx>PDDR   x管脚的方向寄存器，1为输出，0为输入
*************************************************/
void Led_Init(u8 led)
{
	if(led&LED1)
	{
		SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;			//打开A口时钟							
		PORTA->PCR[17] &= ~PORT_PCR_MUX(0);   //MUX 000
		PORTA->PCR[17] |= PORT_PCR_MUX(1);    //MUX 001 GPIO  
		PTA->PDDR |= ((1<<17));  							//17脚设置为输出 
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
Description			点亮任意个LED灯
Calls: 					none
Called By:    	Any
Input:  
								u8 led ：LED1-4  LED_ALL
												LED1 PA17
						            LED2 PE26
											  LED3 PC0
											  LED4 D15

Output:  				将对应管脚给低电平
Return:  				void
Others: 
register: 			
								PTx->PDOR		端口输出数据寄存器
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
/*熄灭一个LED*/
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










