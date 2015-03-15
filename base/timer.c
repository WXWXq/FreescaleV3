/**
  ******************************************************************************
  * @file    timer.c
  * @author  ������Ц
  * @version V1 
  * @date    2014.12.21
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  * @note    ��ʱ������
	* @source  PIT0�ж�
	* @history 			

				 
  ******************************************************************************
	*/
#include "head.h"	
bool T_1ms_Flag=false;
bool T_10ms_Flag=false;
bool T_100ms_Flag=false;
/*************************************************
Function:   		��ʼ��Pitģ��,����1ms���жϣ��ԽǶȼ���
Calls: 					none
Called By:    	PIT0_Init();
Input:  				NONE
Output:  				ʹϵͳ����һ��������ж�
Return:  				void
Others: 
register: 			
								SIM->SCGC6 �üĴ����Ŀ���PITʱ�ӿ���
								PIT->CHANNEL[0].LDVAL  �üĴ���ֵΪ��ʱ��װ��ֵ��CHANNEL[0]Ϊ�ĸ�ͨ��
								PIT->MCR   							0ʹ��PIT
								PIT->CHANNEL[0].CVAL    ��ǰ��������ֵ
*************************************************/
void PIT0_Init(void)
{
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;			//ʱ��ʹ�� 50M
   
    PIT->MCR = 0;													//ʹ��PIT
    
    PIT->CHANNEL[0].LDVAL =  50000;		    //װ��ֵ  1Msһ���ж�
		
	  /*������ʱ��(TEN)�Ϳ�����ʱ���ж�(TIE)*/
    PIT->CHANNEL[0].TCTRL |= (PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK); 
   
    NVIC_EnableIRQ(PIT0_IRQn);						//NVIC����PIT0�жϣ����68

}


/*************************************************
Function:   		�жϷ����� 
Calls: 					none
Called By:    	Angle_Init();
Input:  				NONE
Output:  				ʹϵͳ����һ��������ж�
Return:  				void
Others: 
register: 			
								SIM->SCGC6 �üĴ����Ŀ���PITʱ�ӿ���
								PIT->CHANNEL[0].LDVAL  �üĴ���ֵΪ��ʱ��װ��ֵ��CHANNEL[0]Ϊ�ĸ�ͨ��
								PIT->MCR   							0ʹ��PIT
								PIT->CHANNEL[0].CVAL    ��ǰ��������ֵ
*************************************************/

void PIT0_IRQHandler(void)
{
	static u8 cnt=0;			//5ms����
	static u8 cnt1=0;			//10ms����
	static u16 cnt2=0;    // 100ms����
	PIT->CHANNEL[0].TFLG|= PIT_TFLG_TIF_MASK;			//����ж�
	PIT->MCR  = 0; 		
	cnt1++;
	cnt2++;
	T_1ms_Flag=true;
	if(cnt1==10)	//10ms�жϣ��ж�������Ϣ
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


