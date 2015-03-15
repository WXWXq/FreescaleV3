/**
  ******************************************************************************
  * @file    angle.h
  * @author  ������Ц
  * @version V3 
  * @date    2015/3/13
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  * @note    ��ó�����б��
	* @source  
	* @history 				
	 ����˵����
							ADC1_SE1_DP1  ���ٶȼ�
				 
  ******************************************************************************
	*/
#include "head.h"
#include "adc.h"

float Angle;						//���ӵ�ʵ�����
float Angle_Acc;				//�ɼ��ٶȼƵõ��ĳ������
float Angle_Speed;			//���ӵĽ��ٶ�
double I_Gry=65;//21.1;	//�����ǵĻ���ֵ,����ĳ�ʼֵ��Ϊ����ʱ���ӵĽǶȡ�
u16 Gry_Init;						//���嶯̬���������ֵ

#define Tz  3.27					//����ʱ�䳣��
float Rgry=0.657;
//-------------------------------------------------------
float Q_angle=0.004,//0.001,//0.001,//0.2, ???????   ???????
			Q_gyro=0.00003,//0.003,               ????????  ??????????
			R_angle=0.5,//0.4,//0.5, //???+     ????
			dt=0.001;
float P[2][2] = {
								{ 1, 0 },
								{ 0, 1 }
								};
float Pdot[4] ={0,0,0,0};
const char C_0 = 1;
float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
void Kalman_Filter(float angle_m,float gyro_m)
{
    Angle+=(gyro_m-q_bias) * dt;
    
    Pdot[0]=Q_angle - P[0][1] - P[1][0];
    Pdot[1]=(-P[1][1]);
    Pdot[2]=(-P[1][1]);
    Pdot[3]=Q_gyro;
    
    P[0][0] += Pdot[0] * dt;
    P[0][1] += Pdot[1] * dt;
    P[1][0] += Pdot[2] * dt;
    P[1][1] += Pdot[3] * dt;
    
    
    angle_err = angle_m - Angle;
    
    

    PCt_0 = C_0 * P[0][0];
    PCt_1 = C_0 * P[1][0];
    
    E = R_angle + C_0 * PCt_0;
    
    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;
    
    t_0 = PCt_0;
    t_1 = C_0 * P[0][1];

    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;
    
    
    Angle   += K_0 * angle_err;
    q_bias  += K_1 * angle_err;
    Angle_Speed = gyro_m-q_bias;	
}

