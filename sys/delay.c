/**
  ******************************************************************************
  * @flie    head.h
  * @author  ������Ц
  * @version V1.0
  * @date    2014.11.9
  * @note    ��ʱ����
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  ******************************************************************************
  */
#include "head.h"
/*************************************************
Function:   		LPTMR_Delayms(u32 ms)
Description:  	����LPTMRΪ��ʱ����
Calls: 					none
Called By:    	Any
Input:  
								no

Output:  				ʹϵͳ����һ��������ж�
Return:  				void
Others: 
register: 			
								SIM->SCGC5 �üĴ����Ŀ���LPTMRʱ�ӿ���
								LPTMR0->CMR  �üĴ���ֵΪ��ʱ��װ��ֵ 
								LPTMR0->PSR	 Ԥ����Ĵ��������÷�Ƶֵ��ʱ��
								LPTMR0->CNR(ֻ��)    ��ǰ��������ֵ
								LPTMR0->CSR		���ƼĴ���
*************************************************/
void LPTMR_Delayms(u32 ms)
{
	SIM->SCGC5 |= SIM_SCGC5_LPTIMER_MASK;
	LPTMR0->CMR = ms;
	LPTMR0->PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;  //����LPTMR����Ƶ������1KHZ��LPOʱ��Դ��ժ��M4��ѧ�ʼ�p208��
	LPTMR0->CSR |= LPTMR_CSR_TEN_MASK; 										 //����ʱ��
	while (!(LPTMR0->CSR & LPTMR_CSR_TCF_MASK)); 					 //��ѯ�Ƿ�����ʱʱ��
	LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK; 									 //����Ƚϱ�־
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

