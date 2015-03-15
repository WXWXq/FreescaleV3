/**
  ******************************************************************************
  * @file    flash
  * @author  ������Ц
  * @version V2
  * @date    2015.01.29
  * @chip		 MK60D10
  * @lib		 CHv2.5
  * @note    ����Դ��ժ�����粩��
  * @source
  * @Pin	
  * @history		  
  ******************************************************************************
  */
#ifndef  _FLASH_H
#define  _FLASH_H
#include "sys.h"

#define PAGE   		100           //�������ݴ��ڵ�ҳ��
#define ZPOFFSET  0							//ֱ��P������ƫ���ֽ�
#define ZDOFFSET  4							//ֱ��I������ƫ���ֽ�
#define SPOFFSET  8							//�ٶ�P������ƫ���ֽ�
#define SIOFFSET  12					  //�ٶ�I������ƫ���ֽ�
#define SDOFFSET  16						//�ٶ�D������ƫ���ֽ�
#define FPOFFSET  20						//����P������ƫ���ֽ�
#define FDOFFSET  24						//����D������ƫ���ֽ�
#define fpOFFSET  28						//����P������ƫ���ֽ�
#define fdOFFSET  32						//����D������ƫ���ֽ�
#define SpeedOFFSET  36						//����D������ƫ���ֽ�
extern u8 Flash_Load_Data[40];	

void Flash_init(void);
u8 Flash_erase_sector(u16 sectorNo);
u8 Flash_write(u16 sectNo,u16 offset,u16 cnt,u8 buf[]);
u32 Flash_cmd_launch(void);
u8 Flash_read(u16 sectNo,u16 offset,u16 cnt,u8*bBuf);

void Data_Trans_Save(double,int);
void Data_Trans_Load(double*, int);
void Data_Save(void);
void Data_Load(void);

#endif



