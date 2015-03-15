/**
  ******************************************************************************
  * @file    head.h
  * @author  我想我笑
  * @version V1.0
  * @date    2014.11.9
  * @note    延时函数头文件
	*	@chip		 龙邱开发板
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





