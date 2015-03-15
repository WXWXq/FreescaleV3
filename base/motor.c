/**
  ******************************************************************************
  * @file    motor.h
  * @author  我想我笑
  * @version V1 
  * @date    2014.11.29
	*	@chip		 龙邱开发板
	* @lib		 CHv2.5
  * @note    电机驱动
	* @source  FTM0 PA4,PA5,PA6,PA7
	* @history
					 1,	Date:						2014.11.30
							Author:					wxwx
							Modification:		统一对注释做出规范修改
	 引脚说明：
	 FTM0_CH1_PA04   
	 FTM0_CH2_PA05   
	 FTM0_CH3_PA06   
	 FTM0_CH4_PA07
  ******************************************************************************
  */
#include "head.h"

float Motor_Left_Rev;				//电机转速，单位r/s
float Motor_Right_Rev;
/*************************************************
Function:   		初始化四路PWM输出
Calls: 					
Called By:    	Motor_Init();
Input:  				NONE
Output:  				四路PWM
Return:  				void
Others: 
resource:				FTM0，PA4567
register: 			
								 
*************************************************/
void FTM_Pwm_Init(void)
{
	SIM->SCGC6|=SIM_SCGC6_FTM0_MASK;						//开FTM0时钟
	SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;						//开A口时钟	
	FTM0->MODE|=FTM_MODE_WPDIS_MASK;						//开写保护  ！！！这里不配置也可以，不知道为啥
//	FTM0->SC=0;																//控制器清零
	//端口复用为PWM模式，高驱动能力
	PORTA->PCR[4] = PORT_PCR_MUX(0x03) | PORT_PCR_DSE_MASK;
	PORTA->PCR[5] = PORT_PCR_MUX(0x03) | PORT_PCR_DSE_MASK;
	PORTA->PCR[6] = PORT_PCR_MUX(0x03) | PORT_PCR_DSE_MASK;
	PORTA->PCR[7] = PORT_PCR_MUX(0x03) | PORT_PCR_DSE_MASK;   
	FTM0->CNTIN = 0;                      //FTM 初值
	FTM0->MOD   = 50000-1;                //周期为(MOD - CNTIN + 1) 50M/50K=1K总线时钟50M
	
	FTM0->PWMLOAD=0xffff;									//最大计数值
	
	/*！！！！！这里的时钟有问题，选择的是系统时钟，可是系统时钟是100M，这里只有50M*/				
	/*！！！！据石油大学学长说这个时钟就是用的BUS时钟*/	
	/*						系统时钟			不分频     */ 
	FTM0->SC |=FTM_SC_CLKS(1)|FTM_SC_PS(0); 
	FTM0->SC &= ~FTM_SC_CPWMS_MASK;
	
	//配置为高电平为占空比模式，详见datasheet P958
	FTM0->CONTROLS[1].CnSC |= (FTM_CnSC_MSB_MASK      // MSB 1													 
														|FTM_CnSC_ELSB_MASK);   // ELSB 1 
	FTM0->CONTROLS[1].CnSC &=~FTM_CnSC_ELSA_MASK;			// ELSA 0					
	
	FTM0->CONTROLS[2].CnSC |= (FTM_CnSC_MSB_MASK      // MSB 1													 
														|FTM_CnSC_ELSB_MASK);   // ELSB 1 
	FTM0->CONTROLS[2].CnSC &=~FTM_CnSC_ELSA_MASK;			// ELSA 0	
	
	FTM0->CONTROLS[3].CnSC |= (FTM_CnSC_MSB_MASK      // MSB 1													 
														|FTM_CnSC_ELSB_MASK);   // ELSB 1 
	FTM0->CONTROLS[3].CnSC &=~FTM_CnSC_ELSA_MASK;			// ELSA 0	
	
	FTM0->CONTROLS[4].CnSC |= (FTM_CnSC_MSB_MASK      // MSB 1													 
														|FTM_CnSC_ELSB_MASK);   // ELSB 1 
	FTM0->CONTROLS[4].CnSC &=~FTM_CnSC_ELSA_MASK;			// ELSA 0		
 //FTM0->OUTMASK &=~(FTM_OUTMASK_CH1OM_MASK);                 // 打开通道1，其他的屏蔽掉


}	



/*************************************************
Function:   		更新PWM的占空比
Calls: 					NONE
Called By:    	 
Input:  				NONE
Output:  				四路PWM
Return:  				void
Others: 
resource:				FTM0，PA4567
register: 			
								 
*************************************************/
void Pwm_DutyCycle_Set(u16 PWM1DutyCycle,u16 PWM2DutyCycle,
											 u16 PWM3DutyCycle,u16 PWM4DutyCycle)
{
	//四路PWM占空比 100%对应50000-1
	FTM0->CONTROLS[1].CnV    = PWM1DutyCycle;                      // 占空比	
	FTM0->CONTROLS[2].CnV    = PWM2DutyCycle;                      // 占空比	
	FTM0->CONTROLS[3].CnV    = PWM4DutyCycle;                      // 占空比	
	FTM0->CONTROLS[4].CnV    = PWM3DutyCycle;                      // 占空比	
}
/*************************************************
Function:   		电机初始化
Calls: 					NONE
Called By:    	 
Input:  				NONE
Output:  				四路PWM
Return:  				void
Others: 				初始化PWM
resource:				FTM0，PA4567
register: 			
								 
*************************************************/
void Motor_Init(void)
{
	FTM_Pwm_Init();
	Pwm_DutyCycle_Set(0,0,0,0);
}


