/**
  ******************************************************************************
  * @file    angle.h
  * @author  我想我笑
  * @version V3 
  * @date    2015/3/13
	*	@chip		 龙邱开发板
	* @lib		 CHv2.5
  * @note    获得车的倾斜角
	* @history
	* @source
	* @Pin
						 引脚说明：
							ADC1_SE1_DP1  加速度计				 
  ******************************************************************************
  */

#ifndef _ANGLE_H
#define _ANGLE_H

#include "sys.h"
extern float Angle;						//车子的实际倾角
extern float Angle_Acc;				//由加速度计得到的车子倾角
extern float Rgry;
extern float Angle_Speed;			//角速度，由融合后的角度值计算出来
extern u16 Gry_Init;						//定义动态陀螺仪零点值

void Kalman_Filter(float,float);
float Angle_Get(void);
void Angle_Data_Init(void);

#endif



