/**
  ******************************************************************************
  * @file    sys.h
  * @author  我想我笑
  * @version V1.0
  * @date    2014.11.9
  * @note    对各类类型进行定义
	*	@chip		 龙邱开发板
	* @lib		 CHv2.5
  ******************************************************************************
  */
#ifndef _SYS_H
#define _SYS_H
#include "Mk60D10.h"
typedef unsigned          char u8;
typedef unsigned short     int u16;
typedef unsigned           int u32;
typedef long long          int u64;
void Debug_TimeCount_On(void);
u32 Debug_TimeCount_Off(void);

#endif

