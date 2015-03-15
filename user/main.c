  /**
  ***************2015��ʮ���˼�������ܳ��������ƽ�������*********************
  * @author  ������Ц
  * @version V3
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
	* @history V1,V2			
	* @data    2015/3/12
  ******************************************************************************
  */
#include "head.h"
void Sys_Init(void)
{
	/* ��ϵͳ �ж����ȼ����� �������� 16�� ��ռ���ȼ� ��16�� �����ȼ� */
	
  NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
  NVIC_SetPriority(PIT0_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_2, 2, 2));
  NVIC_SetPriority(UART4_RX_TX_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_2, 1, 1));
  Usart_Init(115200);
	Delay_Init();
	Led_Init(LED_ALL);
	Led_Off(LED1|LED2|LED3|LED4);
	Flash_init();
	Data_Load();
	Angle_Init();//��ʼ�������Ǻͼ��ٶȼ�	
	CCD_Init();
	Delay_ms(777);
	Delay_ms(777);
	Delay_ms(777);
	Delay_ms(777);
	PIT0_Init();
	
}
int main()
{
	Sys_Init();
	///!!!
//	extern float R_angle;
//	R_angle=SPEED_CONTROL_P;
//	extern float Q_angle;
//	Q_angle=SPEED_CONTROL_I;
//	extern float Q_gyro;
//	Q_gyro=SPEED_CONTROL_D;   ��ͨ���778561930854
	while(1)
	{
		if(T_1ms_Flag)
		{
			float   Gry;
			float   Acc;	
			T_1ms_Flag=false;	
			GryAcc_Get(&Gry,&Acc);		
			Kalman_Filter(Acc,Gry);
			OutData[0]=Angle*10;
			OutData[1]=Angle_Speed*10;
			OutData[2]=Acc*10;
			OutData[3]=Gry*10;
			OutPut_Data();
		}
		WXWX_DataReturn();	
	}
}








