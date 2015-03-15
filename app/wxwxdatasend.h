/**
  ******************************************************************************
  * @file    wxwxdatasend.h
  * @author  ������Ц
  * @version V1 
  * @date    2015.01.21
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  * @note    ������Ц��λ��V2�汾ͨ��Э��
	* @source  
	* @history 			

				 
  ******************************************************************************
	*/
#ifndef _WXWXDATASEND_H
#define _WXWXDATASEND_H

#include "head.h"
extern bool ZhenReceiveFlag;								//һ֡�����Ƿ�������
extern char Zhen_Data[9];												//�������ݴ洢
/*�������ݽ��ܵı�־*/
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

/****!!!���±���Ϊ������ʱ��ʱ����***/
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



void WXWX_Send(char,char,double);   //������Ц��λ�����ͳ���
void WXWX_ZhenReceive(u8 data);			//֡����
void WXWX_DataTran(void);						//֡����ת��
void WXWX_DataReturn(void);					//������֡У�鷵�أ�����֤�Ƿ��յ�����
#endif


