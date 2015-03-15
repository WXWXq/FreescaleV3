/**
  ******************************************************************************
  * @file    wxwxdatasend.h
  * @author  ������Ц
  * @version V1 
  * @date    2015.01.21
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  * @note    ������Ц��λ��V2�汾ͨ��Э��
	* @source  
	* @history 			

				 
  ******************************************************************************
	*/
#include "head.h"
/****!!!���±���Ϊ������ʱ��ʱ����***/
double ANGLE_CONTROL_P=0;
double ANGLE_CONTROL_D=0;
double SPEED_CONTROL_P=0;
double SPEED_CONTROL_I=0;
double SPEED_CONTROL_D=0;
double LeftDIR_CONTROL_P=0;
double LeftDIR_CONTROL_D=0;
double RightDIR_CONTROL_P=0;
double RightDIR_CONTROL_D=0;
double CarSpeed=0;


/**************************/
bool ZhenReceiveFlag=false;								//һ֡�����Ƿ�������
/*�������ݽ��ܵı�־*/
bool ZP_Flag=true;
bool ZD_Flag=true;
bool SP_Flag=true;
bool SI_Flag=true;
bool SD_Flag=true;
bool FP_Flag=true;//��ת
bool FD_Flag=true;
bool fp_Flag=true;//��ת
bool fd_Flag=true;
bool Speed_Flag=true;
char Wave_Flag=0;
char WaveWhich1,WaveWhich2;
//���±������ڱ������´���һ
bool AngleNewFlag=0;
bool AngleSpeedNewFlag=0;
bool LeftOutNewFlag=0;
bool RightOutNewFlag=0;
bool CCD1NewFlag=0;
bool CCD2NewFlag=0;
bool Other1NewFlag=0;
bool Other2NewFlag=0;
bool Other3NewFlag=0;
bool Other4NewFlag=0;

char Zhen_Data[9];												//�������ݴ洢
/*************************************************
Function:   		������Ц��λ�����ͳ���
Calls: 					None					
Called By:    	None
Input:   				flag1 flag2 data
Output:  				None
Return:  				void
Others: 				���ڽ����жϣ��յ������Զ�����	
register: 			UART4->D; 
history :
**************************************************/
void WXWX_Send(char flag1,char flag2,double data)
{
	u8 send[15]={'0'};	
	int div=10000;
	int zheng_cnt=0;
	int zheng=0;
	double xiaoshu=0;
//	int xiaoshu_cnt=0;
	send[0]=0xff;
	send[1]=0xfe;
	send[2]=0xfd;
	send[3]=flag1;
	send[4]=flag2;
	zheng=(int)(data);
	xiaoshu=data*100000-zheng*100000;
	zheng_cnt=0;
	while(zheng>0)
	{
		send[5+zheng_cnt]=zheng%10+'0';
		zheng_cnt++;
		zheng/=10;
	}
	if(zheng_cnt==0)
	{
		send[5]='0';
		zheng_cnt=1;
	}
	else
	{
		for(int i=0;i<zheng_cnt/2;i++)
		{
			char change;
			change=send[5+zheng_cnt-1];
			send[5+zheng_cnt-1]=send[5+i];
			send[5+i]=change;			
		}		
	}
	send[5+zheng_cnt]='.';
	for(int i=0;i<6-zheng_cnt;i++)
	{
		send[6+zheng_cnt+i]=(xiaoshu/div)+'0';
		xiaoshu=xiaoshu-((int)(xiaoshu/div))*div;
		div/=10;
	}

	send[12]=0x00;
	send[13]=0x01;
	send[14]=0x02;
	for(int i=0;i<15;i++)
	{
		while(!((UART4->S1) & UART_S1_TDRE_MASK));
		UART4->D=send[i];		
	}
	
}
/*************************************************
Function:   		������Ц��λ��֡���ܳ���
Calls: 					None					
Called By:    	None
Input:   				data
Output:  				None
Return:  				void
Others: 				���ڽ����жϣ��յ������Զ�����	
register: 			UART4->D; 
history :
**************************************************/
void WXWX_ZhenReceive(u8 data)
{
	static bool ZhenTouJiaoYanFlag=false;							//�Ƿ�ʼУ��֡ͷ
  static bool	ZhenDataReceiveFlag=false;						//֡�����Ƿ�ʼ����
	static bool StartReceiveFlag=false;											//֡�����Ƿ�ʼ
	static u8   ZhenWeiCnt;
	static u8   ZhenCnt;																			 //֡ͷ����		
  static u8   DataCnt;																			 //֡���ݲ��ּ���
	if (ZhenReceiveFlag == false)                          
	{
		/*֡ͷУ�鿪ʼ*/
		if (ZhenTouJiaoYanFlag == false)          //֡ͷδУ��ʱ�ܵ�0xff
		{
			if (data == 0xff)
			{
				ZhenTouJiaoYanFlag = true;
				ZhenCnt = 1;
			}
		}
		else if (StartReceiveFlag == false)         //֡ͷУ�鿪ʼ�����ݽ���δ��
		{
			if (ZhenCnt == 1)
			{
				if (data == 0xfe)
				{
					ZhenCnt++;
				}
				else
				{
					ZhenCnt = 0;
					ZhenTouJiaoYanFlag = false;          //δ�յ���ȷ֡ͷ������У��
				}
			}
			else if (ZhenCnt == 2)
			{
				if (data == 0xfd)
				{
					ZhenCnt++;
					StartReceiveFlag = true;             //֡���ݿ�ʼ����
					DataCnt = 0;
				}
				else
				{
					ZhenCnt = 0;
					ZhenTouJiaoYanFlag = false;        //Ϊ��ȷ�ܵ�֡ͷ
				}
			}
		}

/*******֡���ݿ�ʼ����***********/
		else if (ZhenDataReceiveFlag == false)                //???????
		{
			Zhen_Data[DataCnt++] = data;
			if (DataCnt == 9)
			{
				ZhenDataReceiveFlag = true;                  //?????????
				ZhenWeiCnt = 1;
			}
		}
		else if (ZhenReceiveFlag == false)                  //??????
		{
			if (ZhenWeiCnt == 1)
			{
				if (data == 0x00)
				{
					ZhenWeiCnt++;
				}
				else
				{
					ZhenTouJiaoYanFlag = false;
					ZhenDataReceiveFlag = false;
					StartReceiveFlag = false;
				}
			}
			else if (ZhenWeiCnt == 2)
			{
				if (data == 0x01)
				{
					ZhenWeiCnt++;
				}
				else
				{
					ZhenTouJiaoYanFlag = false;
					ZhenDataReceiveFlag = false;
					StartReceiveFlag = false;
				}
			}
			else if (ZhenWeiCnt == 3)
			{
				if (data == 0x02)
				{
					ZhenReceiveFlag = true;
					ZhenTouJiaoYanFlag=false;							//�Ƿ�ʼУ��֡ͷ
					ZhenDataReceiveFlag=false;						//֡�����Ƿ�ʼ����
					StartReceiveFlag=false;											//֡�����Ƿ�ʼ
					ZhenWeiCnt=0;
					ZhenCnt=0;																			 //֡ͷ����		
					DataCnt=0;																			 //֡���ݲ��ּ���
				}
				else
				{
					ZhenTouJiaoYanFlag = false;
					ZhenDataReceiveFlag = false;
					StartReceiveFlag = false;
					ZhenWeiCnt=0;
					ZhenCnt=0;																			 //֡ͷ����		
					DataCnt=0;	
				}
			}
		}

	}	
}
/*************************************************
Function:   		������Ц��λ��֡����ת������
Calls: 					None					
Called By:    	None
Input:   				
Output:  				None
Return:  				void
Others: 				���ڽ����жϣ��յ������Զ�����	
register: 			UART4->D; 
history :
**************************************************/
void WXWX_DataTran(void)
{
	int i=2;
	int temp_zheng=0;
	double temp_xiao=0;
	double temp=0;
	while(Zhen_Data[i]!='.')
	{
		temp_zheng=temp_zheng*10+(Zhen_Data[i++]-'0');		
	}
	i=8;
	while(Zhen_Data[i]!='.')
	{
		temp_xiao=temp_xiao*0.1+(Zhen_Data[i--]-'0');
	}
	temp_xiao*=0.1;	
	temp=temp_zheng+temp_xiao;
	switch(Zhen_Data[0]*Zhen_Data[1])
	{
		
		case 'Z'*'P':				
			ZP_Flag=true;
			ANGLE_CONTROL_P=temp;
			Data_Trans_Save(ANGLE_CONTROL_P,ZPOFFSET);
			break;
		case 'Z'*'D':
			ZD_Flag=true;
			ANGLE_CONTROL_D=temp;
			Data_Trans_Save(ANGLE_CONTROL_D,ZDOFFSET);
			break;
		case 'S'*'P':
			SP_Flag=true;
			SPEED_CONTROL_P=temp;
  		Data_Trans_Save(SPEED_CONTROL_P,SPOFFSET);
			break;
		case 'S'*'I':
			SI_Flag=true;
			SPEED_CONTROL_I=temp;
  		Data_Trans_Save(SPEED_CONTROL_I,SIOFFSET);
			break;
		case 'S'*'D':
			SD_Flag=true;
			SPEED_CONTROL_D=temp;
  		Data_Trans_Save(SPEED_CONTROL_D,SDOFFSET);
			break;
		case 'F'*'P':
			FP_Flag=true;
			LeftDIR_CONTROL_P=temp;
  		Data_Trans_Save(LeftDIR_CONTROL_P,FPOFFSET);
			break;
		case 'F'*'D':
			FD_Flag=true;
			LeftDIR_CONTROL_D=temp;
  		Data_Trans_Save(LeftDIR_CONTROL_D,FDOFFSET);
			break;	
		case 'f'*'p':
			fp_Flag=true;
			RightDIR_CONTROL_P=temp;
  		Data_Trans_Save(RightDIR_CONTROL_P,fpOFFSET);
			break;
		case 'f'*'d':
			fd_Flag=true;
			RightDIR_CONTROL_P=temp;
  		Data_Trans_Save(RightDIR_CONTROL_D,fdOFFSET);
			break;
		case 'S'*'p':			//�ٶ�
			Speed_Flag=true;
			CarSpeed=temp;
  		Data_Trans_Save(CarSpeed,SpeedOFFSET);
			break;	
		//ˢ�����ݣ�zhendataΪѡ��Ҫˢ�µ�����		
		case 'G'*'G':
			ZP_Flag=Zhen_Data[2]&0x01;
			ZD_Flag=Zhen_Data[2]&0x02;
			SP_Flag=Zhen_Data[3]&0x01;
			SI_Flag=Zhen_Data[3]&0x02;
			SD_Flag=Zhen_Data[3]&0x04;
			FP_Flag=Zhen_Data[4]&0x01;
			FD_Flag=Zhen_Data[4]&0x02;
			fp_Flag=Zhen_Data[5]&0x01;
			fd_Flag=Zhen_Data[5]&0x02;
			Speed_Flag=Zhen_Data[5]&0x04;		
			break;
		case 'C'*'K':
			WaveWhich1=Zhen_Data[2];
			WaveWhich2=Zhen_Data[3];		
			Wave_Flag=WaveWhich1|WaveWhich2;			
			break;
		case 'S'*'T':
	  	Pwm_DutyCycle_Set(0,0,0,0);
			while(1);			
	}
	Data_Save();
}
void WXWX_DataReturn(void)				//������֡У�鷵��
{
	bool vis_flag=0;			//����ʾ����
//	bool ccd_flag=0;			//ccd
	if(Wave_Flag)
	{
		if((0x01&WaveWhich1)&&AngleNewFlag)
		{
			OutData[0]=Angle;
			AngleNewFlag=0;
			vis_flag=1;
		}
		if((0x02&WaveWhich1)&&AngleSpeedNewFlag)
		{
	//		OutData[1]=Gry;
			AngleSpeedNewFlag=0;
			vis_flag=1;
		}
		if((0x04&WaveWhich1)&&LeftOutNewFlag)
		{
			OutData[2]=Motor_Left_Rev;
			LeftOutNewFlag=0;
			vis_flag=1;
		}
		if((0x04&WaveWhich1)&&RightOutNewFlag)
		{
			OutData[3]=Motor_Right_Rev;
			RightOutNewFlag=0;
			vis_flag=1;
		}
		if((0x40&WaveWhich1)&&Other1NewFlag)
		{
		//	OutData[0]=Motor_Right_Rev;
			Other1NewFlag=0;
			vis_flag=1;
		}
		if((0x80&WaveWhich1)&&Other2NewFlag)
		{
		//	OutData[1]=Motor_Right_Rev;
			Other2NewFlag=0;
			vis_flag=1;
		}
		if((0x01&WaveWhich2)&&Other3NewFlag)
		{
		//	OutData[2]=Motor_Right_Rev;
			Other3NewFlag=0;
			vis_flag=1;
		}
		if(vis_flag)//CCD������ʾ����������һ��
		{
			OutPut_Data();
			vis_flag=0;
		}
		else
		{
			if((0x10&WaveWhich1)&&CCD1NewFlag)
			{
				CCD1NewFlag=0;
		//!!!		LD_CCD_PutData(CCD_Point);
			}
		}
	}
	else
	{
		if(ZP_Flag)
		{
			ZP_Flag=false;
			WXWX_Send('Z','P',ANGLE_CONTROL_P);
			Delay_ms(17);
		}	
		if(ZD_Flag)
		{
			ZD_Flag=false;
			WXWX_Send('Z','D',ANGLE_CONTROL_D);
			Delay_ms(17);
		}	
		if(SP_Flag)
		{
			SP_Flag=false;
			WXWX_Send('S','P',SPEED_CONTROL_P);
				///!!!
//			extern float R_angle;
//			R_angle=SPEED_CONTROL_P;
			Delay_ms(17);
		}	
		if(SI_Flag)
		{
			SI_Flag=false;
							///!!!
//			extern float Q_angle;
//			Q_angle=SPEED_CONTROL_I;
			WXWX_Send('S','I',SPEED_CONTROL_I);
			Delay_ms(17);
		}	
		if(SD_Flag)
		{
			SD_Flag=false;
										///!!!
//			extern float Q_gyro;
//			Q_gyro=SPEED_CONTROL_D;
			WXWX_Send('S','D',SPEED_CONTROL_D);
			Delay_ms(17);
		}	
		if(FP_Flag)
		{
			FP_Flag=false;
			WXWX_Send('F','P',LeftDIR_CONTROL_P);
			Delay_ms(17);
		}	
		if(FD_Flag)
		{
			FD_Flag=false;
			WXWX_Send('F','D',LeftDIR_CONTROL_D);
			Delay_ms(17);
		}
		if(fp_Flag)
		{
			fp_Flag=false;
			WXWX_Send('f','p',RightDIR_CONTROL_P);
			Delay_ms(17);
		}
		if(fd_Flag)
		{
			fd_Flag=false;
			WXWX_Send('f','d',RightDIR_CONTROL_D);
			Delay_ms(17);
		}
		if(Speed_Flag)
		{
			Speed_Flag=false;
			WXWX_Send('S','p',CarSpeed);
			Delay_ms(17);
		}
	}	
	
}













