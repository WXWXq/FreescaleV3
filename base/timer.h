/**
  ******************************************************************************
  * @file    timer.h
  * @author  我想我笑
  * @version V1 
  * @date    2014.12.21
	*	@chip		 龙邱开发板
	* @lib		 CHv2.5
  * @note    定时器函数
	* @source  
	* @history 			

				 
  ******************************************************************************
	*/
#ifndef _TIMER_H
#define _TIMER_H
#include "head.h"
extern bool T_1ms_Flag;
extern bool T_10ms_Flag;
extern bool T_100ms_Flag;


void PIT0_Init(void);

#endif












