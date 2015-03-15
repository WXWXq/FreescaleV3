/**
  ******************************************************************************
  * @file    timer.c
  * @author  我想我笑
  * @version V1 
  * @date    2014.12.21
	*	@chip		 龙邱开发板
	* @lib		 CHv2.5
  * @note    定时器函数
	* @source  PIT0中断
	* @history 			

				 
  ******************************************************************************
	*/
#include "head.h"	
bool T_1ms_Flag=false;
bool T_10ms_Flag=false;
bool T_100ms_Flag=false;
/*************************************************
Function:   		初始化Pit模块,产生1ms的中断，对角度计算
Calls: 					none
Called By:    	PIT0_Init();
Input:  				NONE
Output:  				使系统产生一定间隔的中断
Return:  				void
Others: 
register: 			
								SIM->SCGC6 该寄存器的控制PIT时钟开关
								PIT->CHANNEL[0].LDVAL  该寄存器值为定时器装载值，CHANNEL[0]为哪个通道
								PIT->MCR   							0使能PIT
								PIT->CHANNEL[0].CVAL    当前计数器的值
*************************************************/
void PIT0_Init(void)
{
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;			//时钟使能 50M
   
    PIT->MCR = 0;													//使能PIT
    
    PIT->CHANNEL[0].LDVAL =  50000;		    //装载值  1Ms一次中断
		
	  /*开启定时器(TEN)和开启定时器中断(TIE)*/
    PIT->CHANNEL[0].TCTRL |= (PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK); 
   
    NVIC_EnableIRQ(PIT0_IRQn);						//NVIC管理PIT0中断，标号68

}


/*************************************************
Function:   		中断服务函数 
Calls: 					none
Called By:    	Angle_Init();
Input:  				NONE
Output:  				使系统产生一定间隔的中断
Return:  				void
Others: 
register: 			
								SIM->SCGC6 该寄存器的控制PIT时钟开关
								PIT->CHANNEL[0].LDVAL  该寄存器值为定时器装载值，CHANNEL[0]为哪个通道
								PIT->MCR   							0使能PIT
								PIT->CHANNEL[0].CVAL    当前计数器的值
*************************************************/

void PIT0_IRQHandler(void)
{
	static u8 cnt=0;			//5ms计数
	static u8 cnt1=0;			//10ms计数
	static u16 cnt2=0;    // 100ms计数
	PIT->CHANNEL[0].TFLG|= PIT_TFLG_TIF_MASK;			//清楚中断
	PIT->MCR  = 0; 		
	cnt1++;
	cnt2++;
	T_1ms_Flag=true;
	if(cnt1==10)	//10ms中断，判断赛道信息
	{		 
		cnt1=0;
		T_10ms_Flag=true;		
	}	
	if(cnt2==100)	//100ms
	{	
		cnt2=0;
		T_100ms_Flag=true;
	}
	
}


