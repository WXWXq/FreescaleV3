/**
  ******************************************************************************
  * @file    led.h
  * @author  ������Ц
  * @version V1.0
  * @date    2014.11.9
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  * @note    LED�Ƶ�ͷ�ļ�
						 ����˵����LED1 PD15
						           LED2 PC0
											 LED3 PE26
											 LED4 PA17
  ******************************************************************************
  */
#ifndef _LED_H
#define _LED_H
#include "sys.h"

#define LED1 0x01
#define LED2 0x02
#define LED3 0x04
#define LED4 0x08
#define LED_ALL 0x0f


void Led_Init(u8);
void Led_On(u8 );
void Led_Off(u8 );








#endif









