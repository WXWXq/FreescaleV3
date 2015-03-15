/**
  ******************************************************************************
  * @file    motor.h
  * @author  我想我笑
  * @version V1 
  * @date    2014.11.29
	*	@chip		 龙邱开发板
	* @lib		 CHv2.5
  * @note    电机驱动
	* @source  FTM0 PA4,PA5,PA6,PA7
	* @history
					 1,	Date:						2014.11.30
							Author:					wxwx
							Modification:		统一对注释做出规范修改
	 引脚说明：
	 FTM0_CH1_PA04   
	 FTM0_CH2_PA05   
	 FTM0_CH3_PA06   
	 FTM0_CH4_PA07
  ******************************************************************************
  */
#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"
extern float Motor_Left_Rev;				//电机转速，单位r/s
extern float Motor_Right_Rev;
void FTM_Pwm_Init(void);
void Pwm_DutyCycle_Set(u16,u16,u16,u16);
void Motor_Init(void);
#endif




