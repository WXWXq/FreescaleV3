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
	
#include "head.h"

u8 Flash_Load_Data[40];							//存储需要写入flash中的全部全局变量

void Flash_init(void)
{
    FMC->PFB0CR |= FMC_PFB0CR_S_B_INV_MASK;       //清除Flash预读取缓冲区
    
    while(!(FTFL->FSTAT & FTFL_FSTAT_CCIF_MASK)); // 等待指令完成
    
    FTFL->FSTAT = (0 | FTFL_FSTAT_ACCERR_MASK      // 清除访问错误标志位
                    | FTFL_FSTAT_FPVIOL_MASK);    
}

//清除页
u8 Flash_erase_sector(u16 sectorNo)
{
    union
    {
        u32  word;
        u8   byte[4];
    } dest;
    
    dest.word    = (u32)(sectorNo*(1<<11));

    FTFL->FCCOB0 = 0x09; //擦除扇区
    
    //设置目标地址
    FTFL->FCCOB1 = dest.byte[2];
    FTFL->FCCOB2 = dest.byte[1];
    FTFL->FCCOB3 = dest.byte[0];
    
    // 执行命令序列
    if(1 == Flash_cmd_launch())    //如果执行命令出现错误
        return 1;     //擦除错误
   
    if(dest.word <= 0x800)
    {
        
        FTFL->FCCOB0 = 0x06; // 写入四字节
        // 设置目标地址
        FTFL->FCCOB1 = 0x00;
        FTFL->FCCOB2 = 0x04;
        FTFL->FCCOB3 = 0x0C;
        // 数据
        FTFL->FCCOB4 = 0xFF;
        FTFL->FCCOB5 = 0xFF;
        FTFL->FCCOB6 = 0xFF;
        FTFL->FCCOB7 = 0xFE;
        //执行命令序列
        if(1 == Flash_cmd_launch())  //如执行出错
            return 2;   //解锁命令错误
    }  
    
    return 0;  //成功返回
}



u8 Flash_read(u16 sectNo,u16 offset,u16 cnt,u8*bBuf)
{
    u32 wAddr = 0;
	//地址是页加偏移
    wAddr = sectNo * 2048 + offset;
    while (cnt--)
        *bBuf++=*(u8*)wAddr++;
   return true;
}


u8 Flash_write(u16 sectNo,u16 offset,u16 cnt,u8 buf[])
{
    u32 size;
    u32 destaddr;
    
    union
    {
        u32   word;
        u8  byte[4];
    } dest;
    
    if(offset%4 != 0)
        return 1;   //偏移必须是4的倍数
    
    // 设置写入指令
    FTFL->FCCOB0 = 0x06;
    destaddr = (u32)(sectNo*(1<<11) + offset);//计算地址
    dest.word = destaddr;
    for(size=0; size<cnt; size+=4, dest.word+=4, buf+=4)
    {
        // 设置目标地址
        FTFL->FCCOB1 = dest.byte[2];
        FTFL->FCCOB2 = dest.byte[1];
        FTFL->FCCOB3 = dest.byte[0];
 
        // 拷贝数据
        FTFL->FCCOB4 = buf[3];
        FTFL->FCCOB5 = buf[2];
        FTFL->FCCOB6 = buf[1];
        FTFL->FCCOB7 = buf[0];
        
        if(1 == Flash_cmd_launch())
            return 2;  //写入命令错误
    }
    
    return 0;  //成功返回
}


u32 Flash_cmd_launch(void)
{
    // 清除访问错误标志位和非法访问标志位
    FTFL->FSTAT = (1<<5) | (1<<4);
    
    // 启动命令
    FTFL->FSTAT = (1<<7);

    // 等待命令结束
    while(!(FTFL->FSTAT &(1<<7)));

    // 检测错误标志
    if(FTFL->FSTAT & ((1<<5) | (1<<4) | 1))
        return 1 ; //执行命令出错
 
    return 0; //执行命令成功
}
	
/*************************************************
Name:    void Data_Trans_Save(double,int);
Function: 将double类型的变量写入转换后存入数组中  		
Calls: 					 
Called By:    	 
Input:    data 要写入的数据， offset 偏移
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
void Data_Trans_Save(double data,int offset)
{
	u32 data_change;
	data_change=data*100000;				//去掉小数位
	for(int i=0;i<4;i++)
	{
		Flash_Load_Data[i+offset]=((0xff000000>>(i*8))&data_change)>>((3-i)*8);
	}	
}
	
/*************************************************
Name:			Data_Save
Function: 将数组中的数据写入flash  		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
void Data_Save(void)
{
	Flash_erase_sector(PAGE);   //擦除页
	Flash_write(PAGE,0,40,Flash_Load_Data);
}

/*************************************************
Name:     Data_Trans_Load
Function: 将某个变量从内存中读出来
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
void Data_Trans_Load(double *data,int offset)
{
	u32 temp=0;
	int ii=0;
	for(ii=0;ii<4;ii++)
	{
		temp|=((Flash_Load_Data[ii+offset])<<((3-ii)*8));		
	}
  *data=(double)(temp);
	(*data)/=100000;
	//return tempdata;
}
	
/*************************************************
Name:     Data_Load
Function: 将变量从内存中读出放到数组中  		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
void Data_Load(void)
{
	u8 temp=0xff;
	Flash_read(PAGE,0,40,Flash_Load_Data);
	for(int i=0;(i<40)&&(temp==0xff);i++)			//检测内部存储flash是不是从未被擦洗
	{
		temp&=Flash_Load_Data[i];
	}
	if(temp!=0xff)						//如果被擦写过，就从里面读数据
	{
		Data_Trans_Load(&ANGLE_CONTROL_P,ZPOFFSET);
		Data_Trans_Load(&ANGLE_CONTROL_D,ZDOFFSET);
		Data_Trans_Load(&SPEED_CONTROL_P,SPOFFSET);
		Data_Trans_Load(&SPEED_CONTROL_I,SIOFFSET);
		Data_Trans_Load(&SPEED_CONTROL_D,SDOFFSET);
		Data_Trans_Load(&LeftDIR_CONTROL_P,FPOFFSET);
		Data_Trans_Load(&LeftDIR_CONTROL_D,FDOFFSET);
		Data_Trans_Load(&RightDIR_CONTROL_P,fpOFFSET);
		Data_Trans_Load(&RightDIR_CONTROL_D,fdOFFSET);
		Data_Trans_Load(&CarSpeed,SpeedOFFSET);
	}
	else										  //没有存数据，就存入数据
	{
		Data_Trans_Save(ANGLE_CONTROL_P,ZPOFFSET);
		Data_Trans_Save(ANGLE_CONTROL_D,ZDOFFSET);
		Data_Trans_Save(SPEED_CONTROL_P,SPOFFSET);
		Data_Trans_Save(SPEED_CONTROL_I,SIOFFSET);
		Data_Trans_Save(SPEED_CONTROL_D,SDOFFSET);
		Data_Trans_Save(LeftDIR_CONTROL_P,FPOFFSET);
		Data_Trans_Save(LeftDIR_CONTROL_D,FDOFFSET);
		Data_Trans_Save(RightDIR_CONTROL_P,fpOFFSET);
		Data_Trans_Save(RightDIR_CONTROL_D,fdOFFSET);
		Data_Trans_Save(CarSpeed,SpeedOFFSET);
				
	}
}


