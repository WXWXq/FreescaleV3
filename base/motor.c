/**
  ******************************************************************************
  * @file    motor.h
  * @author  ������Ц
  * @version V1 
  * @date    2014.11.29
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  * @note    �������
	* @source  FTM0 PA4,PA5,PA6,PA7
	* @history
					 1,	Date:						2014.11.30
							Author:					wxwx
							Modification:		ͳһ��ע�������淶�޸�
	 ����˵����
	 FTM0_CH1_PA04   
	 FTM0_CH2_PA05   
	 FTM0_CH3_PA06   
	 FTM0_CH4_PA07
  ******************************************************************************
  */
#include "head.h"

float Motor_Left_Rev;				//���ת�٣���λr/s
float Motor_Right_Rev;
/*************************************************
Function:   		��ʼ����·PWM���
Calls: 					
Called By:    	Motor_Init();
Input:  				NONE
Output:  				��·PWM
Return:  				void
Others: 
resource:				FTM0��PA4567
register: 			
								 
*************************************************/
void FTM_Pwm_Init(void)
{
	SIM->SCGC6|=SIM_SCGC6_FTM0_MASK;						//��FTM0ʱ��
	SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;						//��A��ʱ��	
	FTM0->MODE|=FTM_MODE_WPDIS_MASK;						//��д����  ���������ﲻ����Ҳ���ԣ���֪��Ϊɶ
//	FTM0->SC=0;																//����������
	//�˿ڸ���ΪPWMģʽ������������
	PORTA->PCR[4] = PORT_PCR_MUX(0x03) | PORT_PCR_DSE_MASK;
	PORTA->PCR[5] = PORT_PCR_MUX(0x03) | PORT_PCR_DSE_MASK;
	PORTA->PCR[6] = PORT_PCR_MUX(0x03) | PORT_PCR_DSE_MASK;
	PORTA->PCR[7] = PORT_PCR_MUX(0x03) | PORT_PCR_DSE_MASK;   
	FTM0->CNTIN = 0;                      //FTM ��ֵ
	FTM0->MOD   = 50000-1;                //����Ϊ(MOD - CNTIN + 1) 50M/50K=1K����ʱ��50M
	
	FTM0->PWMLOAD=0xffff;									//������ֵ
	
	/*���������������ʱ�������⣬ѡ�����ϵͳʱ�ӣ�����ϵͳʱ����100M������ֻ��50M*/				
	/*����������ʯ�ʹ�ѧѧ��˵���ʱ�Ӿ����õ�BUSʱ��*/	
	/*						ϵͳʱ��			����Ƶ     */ 
	FTM0->SC |=FTM_SC_CLKS(1)|FTM_SC_PS(0); 
	FTM0->SC &= ~FTM_SC_CPWMS_MASK;
	
	//����Ϊ�ߵ�ƽΪռ�ձ�ģʽ�����datasheet P958
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
 //FTM0->OUTMASK &=~(FTM_OUTMASK_CH1OM_MASK);                 // ��ͨ��1�����������ε�


}	



/*************************************************
Function:   		����PWM��ռ�ձ�
Calls: 					NONE
Called By:    	 
Input:  				NONE
Output:  				��·PWM
Return:  				void
Others: 
resource:				FTM0��PA4567
register: 			
								 
*************************************************/
void Pwm_DutyCycle_Set(u16 PWM1DutyCycle,u16 PWM2DutyCycle,
											 u16 PWM3DutyCycle,u16 PWM4DutyCycle)
{
	//��·PWMռ�ձ� 100%��Ӧ50000-1
	FTM0->CONTROLS[1].CnV    = PWM1DutyCycle;                      // ռ�ձ�	
	FTM0->CONTROLS[2].CnV    = PWM2DutyCycle;                      // ռ�ձ�	
	FTM0->CONTROLS[3].CnV    = PWM4DutyCycle;                      // ռ�ձ�	
	FTM0->CONTROLS[4].CnV    = PWM3DutyCycle;                      // ռ�ձ�	
}
/*************************************************
Function:   		�����ʼ��
Calls: 					NONE
Called By:    	 
Input:  				NONE
Output:  				��·PWM
Return:  				void
Others: 				��ʼ��PWM
resource:				FTM0��PA4567
register: 			
								 
*************************************************/
void Motor_Init(void)
{
	FTM_Pwm_Init();
	Pwm_DutyCycle_Set(0,0,0,0);
}


