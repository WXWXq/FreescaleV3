/**
  ******************************************************************************
  * @file    head.h
  * @author  ������Ц
  * @version V1.0
  * @date    2014.11.9
  * @note    ��ʱ����ͷ�ļ�
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  ******************************************************************************
  */
#ifndef _DELAY_H
#define _DELAY_H
#include "sys.h"
void Delay_Init(void);
void LPTMR_Delayms(u32 ms);
void Delay_ms(u16 ms);
void Delay_us(u16 us);
#endif 





