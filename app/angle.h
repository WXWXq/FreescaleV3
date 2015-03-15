/**
  ******************************************************************************
  * @file    angle.h
  * @author  ������Ц
  * @version V3 
  * @date    2015/3/13
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  * @note    ��ó�����б��
	* @history
	* @source
	* @Pin
						 ����˵����
							ADC1_SE1_DP1  ���ٶȼ�				 
  ******************************************************************************
  */

#ifndef _ANGLE_H
#define _ANGLE_H

#include "sys.h"
extern float Angle;						//���ӵ�ʵ�����
extern float Angle_Acc;				//�ɼ��ٶȼƵõ��ĳ������
extern float Rgry;
extern float Angle_Speed;			//���ٶȣ����ںϺ�ĽǶ�ֵ�������
extern u16 Gry_Init;						//���嶯̬���������ֵ

void Kalman_Filter(float,float);
float Angle_Get(void);
void Angle_Data_Init(void);

#endif



