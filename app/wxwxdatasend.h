/**
  ******************************************************************************
  * @file    wxwxdatasend.h
  * @author  我想我笑
  * @version V1 
  * @date    2015.01.21
	*	@chip		 龙邱开发板
	* @lib		 CHv2.5
  * @note    我想我笑上位机V2版本通信协议
	* @source  
	* @history 			

				 
  ******************************************************************************
	*/
#ifndef _WXWXDATASEND_H
#define _WXWXDATASEND_H

#include "head.h"
extern bool ZhenReceiveFlag;								//一帧数据是否接受完毕
extern char Zhen_Data[9];												//接受数据存储
/*各个数据接受的标志*/
extern bool ZP_Flag;
extern bool ZD_Flag;
extern bool SP_Flag;
extern bool SI_Flag;
extern bool SD_Flag;
extern bool FP_Flag;
extern bool FD_Flag;
extern bool GS_Flag;
extern bool GZ_Flag;
extern bool AN_Flag;
extern bool AngleNewFlag;
extern bool AngleSpeedNewFlag;
extern bool LeftOutNewFlag;
extern bool RightOutNewFlag;
extern bool CCD1NewFlag;
extern bool CCD2NewFlag;
extern bool Other1NewFlag;
extern bool Other2NewFlag;
extern bool Other3NewFlag;
extern bool Other4NewFlag;

/****!!!以下变量为建工程时临时定义***/
extern double ANGLE_CONTROL_P;
extern double ANGLE_CONTROL_D;
extern double SPEED_CONTROL_P;
extern double SPEED_CONTROL_I;
extern double SPEED_CONTROL_D;
extern double LeftDIR_CONTROL_P;
extern double LeftDIR_CONTROL_D;
extern double RightDIR_CONTROL_P;
extern double RightDIR_CONTROL_D;
extern double CarSpeed;
/**************************/



void WXWX_Send(char,char,double);   //我想我笑上位机发送程序
void WXWX_ZhenReceive(u8 data);			//帧接受
void WXWX_DataTran(void);						//帧数据转换
void WXWX_DataReturn(void);					//将数据帧校验返回，以验证是否收到数据
#endif


