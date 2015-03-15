/**
  ******************************************************************************
  * @file    usart.h
  * @author  ������Ц
  * @version V1 
  * @date    2014.11.24
	*	@chip		 ���񿪷���
	* @lib		 CHv2.5
  * @note    ���ڳ���ͷ�ļ�
	* @resource UART4,PE25,PE24,UART4Interrupt
	*	@Pin			UART4_RX_PE25_TX_PE24
	* @history										   
										 							
  ******************************************************************************
  */
	
#include "head.h"
/*************************************************
Function:   		��ʼ�����ڣ����ò����ʣ���żУ��λ������λ 
Calls: 					UART_Init( );						
								PORT_PinMuxConfig( );
Called By:    	Sys_Init();
Input:   				u32 BaudRate  ������
Output:  				enable UART4
Return:  				void
Others: 					
register: 			 
history :
**************************************************/
void Usart_Init(u32 BaudRate)
{	
	
	UART_InitTypeDef UART_InitStruct1 = {0};
	UART_InitStruct1.instance = HW_UART4;																	//���ں�
	UART_InitStruct1.baudrate = BaudRate;																			//������
	UART_InitStruct1.parityMode = kUART_ParityDisabled;										//��żУ��
	UART_InitStruct1.bitPerChar = kUART_8BitsPerChar;											//����λ
	UART_Init(&UART_InitStruct1);	
	PORT_PinMuxConfig(HW_GPIOE, 24, kPinAlt3);														
	PORT_PinMuxConfig(HW_GPIOE, 25, kPinAlt3);
	UART_ITDMAConfig(HW_UART4, kUART_IT_Rx);
	
}
/*************************************************
Function:   		�����ж��շ�
Calls: 					None					
Called By:    	None
Input:   				None
Output:  				None
Return:  				void
Others: 				���ڽ����жϣ��յ������Զ�����	
register: 			UART4->D; 
history :
**************************************************/

void UART4_RX_TX_IRQHandler(void)
{ 	
	u8 temp;
	NVIC_DisableIRQ(PIT0_IRQn);
	while(((UART4->S1) & UART_S1_RDRF_MASK))       //����Ƿ���յ�����	 
	{
		temp=UART4->D;
	}
	WXWX_ZhenReceive(temp);
	if(ZhenReceiveFlag==true)
	{
		ZhenReceiveFlag=false;
		WXWX_DataTran();
	}
	NVIC_EnableIRQ(PIT0_IRQn);
}








/*example


UART_WriteByte(HW_UART4, 'h');			//���ڷ��ͺ���
��ʹ�ô��ڳ�ʼ���� printf ��Ĭ�����ӵ���һ������ʼ���Ĵ�����
printf("UART%d OK! Hello Kinetis\r\n", instance);




�жϷ��ͺ���

    //ע�ᷢ���жϻص����� 
    UART_CallbackTxInstall(HW_UART4, UART_TX_ISR);
    //�򿪷�������ж� 
    UART_ITDMAConfig(HW_UART4, kUART_IT_Tx);
//��Ҫ���͵�����
static const char UART_String1[] = "HelloWorld\r\n";
//�жϷ��ͺ���
static void UART_TX_ISR(uint16_t * byteToSend)
{
    static const char *p = UART_String1;
    *byteToSend = *p++;
    if((p - UART_String1) == sizeof(UART_String1))
    {
        p = UART_String1;
        UART_ITDMAConfig(HW_UART4, kUART_IT_Tx_Disable);  
    }
}










*/




