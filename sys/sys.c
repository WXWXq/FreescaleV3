#include "head.h"
/*************************************************
Name:     Debug_TimeCount_Init
Function: ���Լ����������㺯��ʱ��  		
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
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;			//ʱ��ʹ��   
    PIT->MCR = 1;													//��ֹPIT    
    PIT->CHANNEL[0].LDVAL =  0xffffffff;		//װ��ֵ

	
	  /*������ʱ��(TEN)*/
    PIT->CHANNEL[0].TCTRL |= (PIT_TCTRL_TEN_MASK);
    PIT->MCR = 0;													//����PIT     	
}
/*************************************************
Name:      Debug_TimeCount_Off
Function:  �رն�ʱ�������ʱ��	
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

