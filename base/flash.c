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
	
#include "head.h"

u8 Flash_Load_Data[40];							//�洢��Ҫд��flash�е�ȫ��ȫ�ֱ���

void Flash_init(void)
{
    FMC->PFB0CR |= FMC_PFB0CR_S_B_INV_MASK;       //���FlashԤ��ȡ������
    
    while(!(FTFL->FSTAT & FTFL_FSTAT_CCIF_MASK)); // �ȴ�ָ�����
    
    FTFL->FSTAT = (0 | FTFL_FSTAT_ACCERR_MASK      // ������ʴ����־λ
                    | FTFL_FSTAT_FPVIOL_MASK);    
}

//���ҳ
u8 Flash_erase_sector(u16 sectorNo)
{
    union
    {
        u32  word;
        u8   byte[4];
    } dest;
    
    dest.word    = (u32)(sectorNo*(1<<11));

    FTFL->FCCOB0 = 0x09; //��������
    
    //����Ŀ���ַ
    FTFL->FCCOB1 = dest.byte[2];
    FTFL->FCCOB2 = dest.byte[1];
    FTFL->FCCOB3 = dest.byte[0];
    
    // ִ����������
    if(1 == Flash_cmd_launch())    //���ִ��������ִ���
        return 1;     //��������
   
    if(dest.word <= 0x800)
    {
        
        FTFL->FCCOB0 = 0x06; // д�����ֽ�
        // ����Ŀ���ַ
        FTFL->FCCOB1 = 0x00;
        FTFL->FCCOB2 = 0x04;
        FTFL->FCCOB3 = 0x0C;
        // ����
        FTFL->FCCOB4 = 0xFF;
        FTFL->FCCOB5 = 0xFF;
        FTFL->FCCOB6 = 0xFF;
        FTFL->FCCOB7 = 0xFE;
        //ִ����������
        if(1 == Flash_cmd_launch())  //��ִ�г���
            return 2;   //�����������
    }  
    
    return 0;  //�ɹ�����
}



u8 Flash_read(u16 sectNo,u16 offset,u16 cnt,u8*bBuf)
{
    u32 wAddr = 0;
	//��ַ��ҳ��ƫ��
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
        return 1;   //ƫ�Ʊ�����4�ı���
    
    // ����д��ָ��
    FTFL->FCCOB0 = 0x06;
    destaddr = (u32)(sectNo*(1<<11) + offset);//�����ַ
    dest.word = destaddr;
    for(size=0; size<cnt; size+=4, dest.word+=4, buf+=4)
    {
        // ����Ŀ���ַ
        FTFL->FCCOB1 = dest.byte[2];
        FTFL->FCCOB2 = dest.byte[1];
        FTFL->FCCOB3 = dest.byte[0];
 
        // ��������
        FTFL->FCCOB4 = buf[3];
        FTFL->FCCOB5 = buf[2];
        FTFL->FCCOB6 = buf[1];
        FTFL->FCCOB7 = buf[0];
        
        if(1 == Flash_cmd_launch())
            return 2;  //д���������
    }
    
    return 0;  //�ɹ�����
}


u32 Flash_cmd_launch(void)
{
    // ������ʴ����־λ�ͷǷ����ʱ�־λ
    FTFL->FSTAT = (1<<5) | (1<<4);
    
    // ��������
    FTFL->FSTAT = (1<<7);

    // �ȴ��������
    while(!(FTFL->FSTAT &(1<<7)));

    // �������־
    if(FTFL->FSTAT & ((1<<5) | (1<<4) | 1))
        return 1 ; //ִ���������
 
    return 0; //ִ������ɹ�
}
	
/*************************************************
Name:    void Data_Trans_Save(double,int);
Function: ��double���͵ı���д��ת�������������  		
Calls: 					 
Called By:    	 
Input:    data Ҫд������ݣ� offset ƫ��
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
void Data_Trans_Save(double data,int offset)
{
	u32 data_change;
	data_change=data*100000;				//ȥ��С��λ
	for(int i=0;i<4;i++)
	{
		Flash_Load_Data[i+offset]=((0xff000000>>(i*8))&data_change)>>((3-i)*8);
	}	
}
	
/*************************************************
Name:			Data_Save
Function: �������е�����д��flash  		
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
	Flash_erase_sector(PAGE);   //����ҳ
	Flash_write(PAGE,0,40,Flash_Load_Data);
}

/*************************************************
Name:     Data_Trans_Load
Function: ��ĳ���������ڴ��ж�����
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
Function: ���������ڴ��ж����ŵ�������  		
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
	for(int i=0;(i<40)&&(temp==0xff);i++)			//����ڲ��洢flash�ǲ��Ǵ�δ����ϴ
	{
		temp&=Flash_Load_Data[i];
	}
	if(temp!=0xff)						//�������д�����ʹ����������
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
	else										  //û�д����ݣ��ʹ�������
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


