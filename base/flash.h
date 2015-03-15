/**
  ******************************************************************************
  * @file    flash
  * @author  我想我笑
  * @version V2
  * @date    2015.01.29
  * @chip		 MK60D10
  * @lib		 CHv2.5
  * @note    部分源码摘自网络博客
  * @source
  * @Pin	
  * @history		  
  ******************************************************************************
  */
#ifndef  _FLASH_H
#define  _FLASH_H
#include "sys.h"

#define PAGE   		100           //定义数据存在的页数
#define ZPOFFSET  0							//直立P参数的偏移字节
#define ZDOFFSET  4							//直立I参数的偏移字节
#define SPOFFSET  8							//速度P参数的偏移字节
#define SIOFFSET  12					  //速度I参数的偏移字节
#define SDOFFSET  16						//速度D参数的偏移字节
#define FPOFFSET  20						//方向P参数的偏移字节
#define FDOFFSET  24						//方向D参数的偏移字节
#define fpOFFSET  28						//方向P参数的偏移字节
#define fdOFFSET  32						//方向D参数的偏移字节
#define SpeedOFFSET  36						//方向D参数的偏移字节
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



