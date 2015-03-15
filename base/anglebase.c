/**
  ******************************************************************************
  * @file    	anglebase
  * @author  	WXWX
  * @version  V3
  * @date     2015/3/12
  * @chip		  MK60Dn512
  * @lib		  CHv2.5
  * @note     
  * @source   IIC0,ADC1SE0 DP0
  * @Pin	     
  * @history		  
  ******************************************************************************
  */
#include "head.h"
#include "math.h"
/*************************************************
Name:				ADC1_Init
Function:   ��ʼ��ADC1		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 		  ��������ҿ�ʼ�Ĳ��ֵ����˳��˵Ŀ�������һЩ��������Ϊ���¹����鷳			
Resource:			 	
register: 			
							 
*************************************************/
void ADC1_Init(void)
{

    ADC_InitTypeDef ADC_InitStruct1;		    
    ADC_InitStruct1.instance = HW_ADC1;
    ADC_InitStruct1.clockDiv = kADC_ClockDiv2; /* ADC����ʱ��2��Ƶ */
    ADC_InitStruct1.resolutionMode = kADC_SingleDiff12or13;
    ADC_InitStruct1.triggerMode = kADC_TriggerSoftware; /* �������ת�� */
    ADC_InitStruct1.singleOrDiffMode = kADC_Single; /*����ģʽ */
    ADC_InitStruct1.continueMode = kADC_ContinueConversionEnable; /* ��������ת�� ת��һ�κ� �Զ���ʼ��һ��ת��*/
    ADC_InitStruct1.hardwareAveMode = kADC_HardwareAverage_32; /*��ֹ Ӳ��ƽ�� ���� */
    ADC_InitStruct1.vref = kADC_VoltageVREF;                       /* ʹ���ⲿVERFH VREFL ��Ϊģ���ѹ�ο� */
    ADC_Init(&ADC_InitStruct1);
	//adc1
	SIM->SCGC3 |= SIM_SCGC3_ADC1_MASK;
	ADC1->CFG1=0;													//����ָ�Ĭ��
  ADC1->CFG1 |= ADC_CFG1_ADICLK(1)  	  //����ʱ�Ӷ���ƵΪADCʱ��
             |ADC_CFG1_ADLSMP_MASK    	// ������ʱ��
             |ADC_CFG1_MODE(1)    			// MODE  01,12λ����
				     |ADC_CFG1_ADICLK(0);				//ADICLK 00,����ʱ��Ϊ����ʱ�� 
	ADC1->CFG2=0;													//����ָ�Ĭ��
  ADC1->CFG2 |= ADC_CFG2_ADHSC_MASK    	// ADHSC   1 ����ת��ʱ��
						 |ADC_CFG2_ADLSTS(0);				//������ʱ�� 

  ADC1->SC2 |=ADC_SC2_REFSEL(0);        // REFSEL  00 Ĭ�ϲο���ѹΪ�ڲ���ѹ

  ADC1->SC3 |=ADC_SC3_AVGS(3)           // AVGS 32�� ������ֵ
            |ADC_SC3_AVGE_MASK         // AVGE 1 ������ֵʹ�� 	
						|ADC_SC3_ADCO_MASK;							
}
/*************************************************
Name:			void IIC1_Init(void)
Function: ��IIC1ģ���ʼ�� 		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 		����IIC1 SCL C10  SDA C11	 			
Resource:	 		 	
register: 			
							 
*************************************************/
void IIC1_Init(u32 baud)
{
	SIM->SCGC5 |=SIM_SCGC5_PORTC_MASK;			//enable PORTC clock
	PORTC->PCR[10]=PORT_PCR_MUX(2)						//����ΪIIC CLK,��©������
								|PORT_PCR_ODE_MASK					
								|PORT_PCR_PE_MASK	
								|PORT_PCR_PS_MASK;	
	PORTC->PCR[11]=PORT_PCR_MUX(2)						//����ΪIIC SDA,��©������
								|PORT_PCR_ODE_MASK					
								|PORT_PCR_PE_MASK	
								|PORT_PCR_PS_MASK;					
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;			//enable IIC1 clock
  u16 ICR_2_SCL_divider[0x40]  =
    {
        20, 22, 24, 26, 28, 30, 34, 40, 28, 32, 36, 40, 44, 48, 56, 68,
        48, 56, 64, 72, 80, 88, 104, 128, 80, 96, 112, 128, 144, 160, 192, 240,
        160, 192, 224, 256, 288, 320, 384, 480, 320, 384, 448, 512, 576, 640, 768, 960,
        640, 768, 896, 1024, 1152, 1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840
    };

   
    u16 scldiv =  50000 * 1000 / ( (1<<0) * baud );   
    u8 icr, n = 0x40;
    u16 min_Dvalue = ~0, Dvalue;

    while(n)                                         
    {
        n--;
        Dvalue = ABS(scldiv - ICR_2_SCL_divider[n]);
        if(Dvalue == 0)
        {
            icr = n;
            break;                            
        }

        if(Dvalue < min_Dvalue)
        {
            icr = n;
            min_Dvalue = Dvalue;
        }
    }

    I2C1->F  = I2C_F_MULT(0)| I2C_F_ICR(icr); 
    I2C1->C1 = I2C_C1_IICEN_MASK;	
}

// ʹIIC����һ��ʼ�ź�  		
void IIC_Start(void)
{
	I2C1->C1|=(I2C_C1_TX_MASK|I2C_C1_MST_MASK);
}

//ֹͣ�ź�
void IIC_Stop(void)
{
	I2C1->C1&=~(I2C_C1_TX_MASK|I2C_C1_MST_MASK); 
}
//�ظ�����
void IIC_RepeatedStart(void)
{
	I2C1->C1|= I2C_C1_RSTA_MASK;
}
//�ȴ�IIC_S
void IIC_Wait(void)
{
    while(( I2C1->S & I2C_S_IICIF_MASK)==0) {} 
    I2C1->S |= I2C_S_IICIF_MASK;
	
}
void IIC_Delay(void)
{
  Delay_us(10);
}
/*************************************************
Name:			void IIC_Write(u8);
Function:   		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
void IIC_Write(u8 temp)
{
	I2C1->D=temp;
	IIC_Wait();
}

/*************************************************
Name:
Function:   		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
u8 IIC_Read(void)
{
	u8 temp;
	temp = I2C1->D;
	return temp;	
}

/*************************************************
Name:			void L3g_Init(void);
Function:  ��l3g�����ǳ�ʼ�� 	 	
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
void L3g_Init(void)
{
	IIC1_Init(400*1000);
	L3g_Write(CTRL_REG1, 0x0f);   //
	L3g_Write(CTRL_REG2, 0x00);   //
	L3g_Write(CTRL_REG3, 0x08);   //
	L3g_Write(CTRL_REG4, 0x30);  //+-2000dps
	L3g_Write(CTRL_REG5, 0x00);	
}

/*************************************************
Name:
Function:   		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/

void L3g_Write(u8 addr,u8 data)
{
    IIC_Start();                  //��ʼ�ź�
    IIC_Write(SlaveAddress);   //�����豸��ַ+д�ź�
    IIC_Write(addr);    //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
    IIC_Write(data);       //�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
    IIC_Stop();                   //����ֹͣ�ź�	
    IIC_Delay();                                        //??????,?????
}


/*************************************************
Name:
Function:   		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
u8 L3g_Read(u8 addr)
{
	u8 data;
  IIC_Start();                          //��ʼ�ź�
	IIC_Write(SlaveAddress);           //�����豸��ַ+д�ź�
	IIC_Write(addr);            //���ʹ洢��Ԫ��ַ����0��ʼ	
	IIC_RepeatedStart();                          //��ʼ�ź�
	IIC_Write(SlaveAddress+1);         //�����豸��ַ+���ź�
	//�������ģʽ
	I2C1->C1&= ~I2C_C1_TX_MASK;
	I2C1->C1|= I2C_C1_TXAK_MASK;
	
	data=IIC_Read();              //��ٽ���
	IIC_Wait();
	IIC_Stop();                           //ֹͣ�ź�
	data=IIC_Read();              //��������
	IIC_Delay();
	return data; 
}
//��ʼ�������Ǻͼ��ٶȼ�
void Angle_Init(void)
{
	ADC1_Init();
	L3g_Init();
}
/*************************************************
Name:			GryAcc_Get
Function: ��ü��ٶȼƺ������ǵ�ԭʼ����  		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
#define AccZ_Max   2730 		//�˳���
#define AccZ_Min   846   	//�˳���
#define AccZ_Init  1788
#define AccX_Max   2936 		//ͷ����
#define AccX_Min   957   	//ͷ����
#define AccX_Init  1947
void GryAcc_Get(float *Gry,float *Acc)
{
	short temp_l,temp_h;  
	short z,x;
	ADC1->SC1[0]&=~(ADC_SC1_ADCH_MASK);
  ADC1->SC1[0]|=ADC_SC1_ADCH(1);					//ͨ��1�����ٶȼ�z
  while (!(ADC1->SC1[0]&ADC_SC1_COCO_MASK));
  z = ADC1->R[0];
	z-=(AccZ_Init);
	ADC1->SC1[0]&=~(ADC_SC1_ADCH_MASK);
  ADC1->SC1[0]|=ADC_SC1_ADCH(3);					//ͨ��1�����ٶȼ�x	
  while (!(ADC1->SC1[0]&ADC_SC1_COCO_MASK));
  x= ADC1->R[0];
	x-=AccX_Init;	
	*Acc=(atan2((double)z,(double)x))/(6.28318)*360;		//����������������ǻ��ȣ����ﻯ�ɡ�
	
//��������ǵ�ֵ
	temp_l= L3g_Read(OUT_X_L);
	temp_h= L3g_Read(OUT_X_H);
	temp_h=temp_h<<8;
	temp_h=temp_h|temp_l;
	*Gry=temp_h*0.07;
}




