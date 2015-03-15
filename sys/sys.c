#include "head.h"
/*************************************************
Name:     Debug_TimeCount_Init
Function: 调试计数器，计算函数时间  		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
void Debug_TimeCount_On(void)
{
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;			//时钟使能   
    PIT->MCR = 1;													//禁止PIT    
    PIT->CHANNEL[0].LDVAL =  0xffffffff;		//装载值

	
	  /*开启定时器(TEN)*/
    PIT->CHANNEL[0].TCTRL |= (PIT_TCTRL_TEN_MASK);
    PIT->MCR = 0;													//开启PIT     	
}
/*************************************************
Name:      Debug_TimeCount_Off
Function:  关闭定时器并输出时间	
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
u32 Debug_TimeCount_Off(void)
{
	u32 result;
	result=(PIT->CHANNEL[0].LDVAL-PIT->CHANNEL[0].CVAL)/50;
	PIT->CHANNEL[0].TCTRL &= ~(PIT_TCTRL_TEN_MASK); 	
	return result;
}

