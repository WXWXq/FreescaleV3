/**
  ******************************************************************************
  * @file    motor.h
  * @author  ������Ц
  * @version V1 
  * @date    2014.11.29
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  * @note    �������
	* @source  FTM0 PA4,PA5,PA6,PA7
	* @history
					 1,	Date:						2014.11.30
							Author:					wxwx
							Modification:		ͳһ��ע�������淶�޸�
	 ����˵����
	 FTM0_CH1_PA04   
	 FTM0_CH2_PA05   
	 FTM0_CH3_PA06   
	 FTM0_CH4_PA07
  ******************************************************************************
  */
#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"
extern float Motor_Left_Rev;				//���ת�٣���λr/s
extern float Motor_Right_Rev;
void FTM_Pwm_Init(void);
void Pwm_DutyCycle_Set(u16,u16,u16,u16);
void Motor_Init(void);
#endif




