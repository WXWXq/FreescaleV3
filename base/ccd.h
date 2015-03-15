/**
  ******************************************************************************
  * @file    ccd.h
  * @author  我想我笑
  * @version V1 
  * @date    2014.12.21
	*	@chip		 龙邱开发板
	* @lib		 CHv2.5
  * @note    
	* @source   
	* @history 			

				 
  ******************************************************************************
	*/
#ifndef _CCD_H
#define _CCD_H
#include "head.h"

#define SI_L PTE->PDOR&=~(1<<0)
#define SI_H PTE->PDOR|=(1<<0)
#define CLK_L PTE->PDOR&=~(1<<1)
#define CLK_H PTE->PDOR|=(1<<1)

extern u8 CCD_Point[130];
void CCD_Init(void);
void CCD_GetLine(void);
void LD_CCD_PutData(unsigned char * ImageData);
void CCD_SendHex(unsigned char hex) ;
#endif





