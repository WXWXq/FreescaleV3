/**
  ******************************************************************************
  * @flie    head.h
  * @author  我想我笑
  * @version V1.0
  * @date    2014.11.9
  * @note    延时函数
	*	@chip		 龙邱开发板
	* @lib		 CHv2.5
  ******************************************************************************
  */
#include "head.h"
/*************************************************
Function:   		LPTMR_Delayms(u32 ms)
Description:  	设置LPTMR为延时功能
Calls: 					none
Called By:    	Any
Input:  
								no

Output:  				使系统产生一定间隔的中断
Return:  				void
Others: 
register: 			
								SIM->SCGC5 该寄存器的控制LPTMR时钟开关
								LPTMR0->CMR  该寄存器值为定时器装载值 
								LPTMR0->PSR	 预分配寄存器，设置分频值和时钟
								LPTMR0->CNR(只读)    当前计数器的值
								LPTMR0->CSR		控制寄存器
*************************************************/
void LPTMR_Delayms(u32 ms)
{
	SIM->SCGC5 |= SIM_SCGC5_LPTIMER_MASK;
	LPTMR0->CMR = ms;
	LPTMR0->PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;  //设置LPTMR不分频，采用1KHZ的LPO时钟源（摘自M4自学笔记p208）
	LPTMR0->CSR |= LPTMR_CSR_TEN_MASK; 										 //开定时器
	while (!(LPTMR0->CSR & LPTMR_CSR_TCF_MASK)); 					 //查询是否到了延时时间
	LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK; 									 //清除比较标志
}

void Delay_Init(void)
{
	DelayInit();
}
void Delay_ms(u16 ms)
{
	 LPTMR_Delayms(ms);
}
void Delay_us(u16 us)
{
	DelayUs(us);
}

