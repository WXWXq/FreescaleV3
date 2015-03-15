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
Function:   初始化ADC1		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 		  这个程序我开始的部分调用了超核的库配置了一些东西，因为改下过于麻烦			
Resource:			 	
register: 			
							 
*************************************************/
void ADC1_Init(void)
{

    ADC_InitTypeDef ADC_InitStruct1;		    
    ADC_InitStruct1.instance = HW_ADC1;
    ADC_InitStruct1.clockDiv = kADC_ClockDiv2; /* ADC采样时钟2分频 */
    ADC_InitStruct1.resolutionMode = kADC_SingleDiff12or13;
    ADC_InitStruct1.triggerMode = kADC_TriggerSoftware; /* 软件触发转换 */
    ADC_InitStruct1.singleOrDiffMode = kADC_Single; /*单端模式 */
    ADC_InitStruct1.continueMode = kADC_ContinueConversionEnable; /* 启动连续转换 转换一次后 自动开始下一次转换*/
    ADC_InitStruct1.hardwareAveMode = kADC_HardwareAverage_32; /*禁止 硬件平均 功能 */
    ADC_InitStruct1.vref = kADC_VoltageVREF;                       /* 使用外部VERFH VREFL 作为模拟电压参考 */
    ADC_Init(&ADC_InitStruct1);
	//adc1
	SIM->SCGC3 |= SIM_SCGC3_ADC1_MASK;
	ADC1->CFG1=0;													//清零恢复默认
  ADC1->CFG1 |= ADC_CFG1_ADICLK(1)  	  //输入时钟二分频为ADC时钟
             |ADC_CFG1_ADLSMP_MASK    	// 长采样时间
             |ADC_CFG1_MODE(1)    			// MODE  01,12位精度
				     |ADC_CFG1_ADICLK(0);				//ADICLK 00,输入时钟为总线时钟 
	ADC1->CFG2=0;													//清零恢复默认
  ADC1->CFG2 |= ADC_CFG2_ADHSC_MASK    	// ADHSC   1 高速转换时序
						 |ADC_CFG2_ADLSTS(0);				//长采样时间 

  ADC1->SC2 |=ADC_SC2_REFSEL(0);        // REFSEL  00 默认参考电压为内部电压

  ADC1->SC3 |=ADC_SC3_AVGS(3)           // AVGS 32个 采样均值
            |ADC_SC3_AVGE_MASK         // AVGE 1 采样均值使能 	
						|ADC_SC3_ADCO_MASK;							
}
/*************************************************
Name:			void IIC1_Init(void)
Function: 将IIC1模块初始化 		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 		配置IIC1 SCL C10  SDA C11	 			
Resource:	 		 	
register: 			
							 
*************************************************/
void IIC1_Init(u32 baud)
{
	SIM->SCGC5 |=SIM_SCGC5_PORTC_MASK;			//enable PORTC clock
	PORTC->PCR[10]=PORT_PCR_MUX(2)						//复用为IIC CLK,开漏，上拉
								|PORT_PCR_ODE_MASK					
								|PORT_PCR_PE_MASK	
								|PORT_PCR_PS_MASK;	
	PORTC->PCR[11]=PORT_PCR_MUX(2)						//复用为IIC SDA,开漏，上拉
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

// 使IIC产生一开始信号  		
void IIC_Start(void)
{
	I2C1->C1|=(I2C_C1_TX_MASK|I2C_C1_MST_MASK);
}

//停止信号
void IIC_Stop(void)
{
	I2C1->C1&=~(I2C_C1_TX_MASK|I2C_C1_MST_MASK); 
}
//重复启动
void IIC_RepeatedStart(void)
{
	I2C1->C1|= I2C_C1_RSTA_MASK;
}
//等待IIC_S
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
Function:  对l3g陀螺仪初始化 	 	
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
    IIC_Start();                  //起始信号
    IIC_Write(SlaveAddress);   //发送设备地址+写信号
    IIC_Write(addr);    //内部寄存器地址，请参考中文pdf22页 
    IIC_Write(data);       //内部寄存器数据，请参考中文pdf22页 
    IIC_Stop();                   //发送停止信号	
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
  IIC_Start();                          //起始信号
	IIC_Write(SlaveAddress);           //发送设备地址+写信号
	IIC_Write(addr);            //发送存储单元地址，从0开始	
	IIC_RepeatedStart();                          //起始信号
	IIC_Write(SlaveAddress+1);         //发送设备地址+读信号
	//进入接受模式
	I2C1->C1&= ~I2C_C1_TX_MASK;
	I2C1->C1|= I2C_C1_TXAK_MASK;
	
	data=IIC_Read();              //虚假接受
	IIC_Wait();
	IIC_Stop();                           //停止信号
	data=IIC_Read();              //接受数据
	IIC_Delay();
	return data; 
}
//初始化陀螺仪和加速度计
void Angle_Init(void)
{
	ADC1_Init();
	L3g_Init();
}
/*************************************************
Name:			GryAcc_Get
Function: 获得加速度计和陀螺仪的原始数据  		
Calls: 					 
Called By:    	 
Input:  
Output:  				
Return:  				
Note: 					
Resource:			 	
register: 			
							 
*************************************************/
#define AccZ_Max   2730 		//杆朝下
#define AccZ_Min   846   	//杆朝上
#define AccZ_Init  1788
#define AccX_Max   2936 		//头朝下
#define AccX_Min   957   	//头朝上
#define AccX_Init  1947
void GryAcc_Get(float *Gry,float *Acc)
{
	short temp_l,temp_h;  
	short z,x;
	ADC1->SC1[0]&=~(ADC_SC1_ADCH_MASK);
  ADC1->SC1[0]|=ADC_SC1_ADCH(1);					//通道1，加速度计z
  while (!(ADC1->SC1[0]&ADC_SC1_COCO_MASK));
  z = ADC1->R[0];
	z-=(AccZ_Init);
	ADC1->SC1[0]&=~(ADC_SC1_ADCH_MASK);
  ADC1->SC1[0]|=ADC_SC1_ADCH(3);					//通道1，加速度计x	
  while (!(ADC1->SC1[0]&ADC_SC1_COCO_MASK));
  x= ADC1->R[0];
	x-=AccX_Init;	
	*Acc=(atan2((double)z,(double)x))/(6.28318)*360;		//反正切运算出来的是弧度，这里化成°
	
//获得陀螺仪的值
	temp_l= L3g_Read(OUT_X_L);
	temp_h= L3g_Read(OUT_X_H);
	temp_h=temp_h<<8;
	temp_h=temp_h|temp_l;
	*Gry=temp_h*0.07;
}




