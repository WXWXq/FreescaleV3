/**
  ******************************************************************************
  * @file    usart.h
  * @author  我想我笑
  * @version V1 
  * @date    2014.11.24
	*	@chip		 龙邱开发板
	* @lib		 CHv2.5
  * @note    串口程序头文件
	* @resource UART4,PE25,PE24,UART4Interrupt
	*	@Pin			UART4_RX_PE25_TX_PE24
	* @history										   
										 							
  ******************************************************************************
  */
	
#include "head.h"
/*************************************************
Function:   		初始化串口，设置波特率，奇偶校验位，数据位 
Calls: 					UART_Init( );						
								PORT_PinMuxConfig( );
Called By:    	Sys_Init();
Input:   				u32 BaudRate  波特率
Output:  				enable UART4
Return:  				void
Others: 					
register: 			 
history :
**************************************************/
void Usart_Init(u32 BaudRate)
{	
	
	UART_InitTypeDef UART_InitStruct1 = {0};
	UART_InitStruct1.instance = HW_UART4;																	//串口号
	UART_InitStruct1.baudrate = BaudRate;																			//波特率
	UART_InitStruct1.parityMode = kUART_ParityDisabled;										//奇偶校验
	UART_InitStruct1.bitPerChar = kUART_8BitsPerChar;											//数据位
	UART_Init(&UART_InitStruct1);	
	PORT_PinMuxConfig(HW_GPIOE, 24, kPinAlt3);														
	PORT_PinMuxConfig(HW_GPIOE, 25, kPinAlt3);
	UART_ITDMAConfig(HW_UART4, kUART_IT_Rx);
	
}
/*************************************************
Function:   		串口中断收发
Calls: 					None					
Called By:    	None
Input:   				None
Output:  				None
Return:  				void
Others: 				串口接受中断，收到数据自动进入	
register: 			UART4->D; 
history :
**************************************************/

void UART4_RX_TX_IRQHandler(void)
{ 	
	u8 temp;
	NVIC_DisableIRQ(PIT0_IRQn);
	while(((UART4->S1) & UART_S1_RDRF_MASK))       //检查是否接收到数据	 
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


UART_WriteByte(HW_UART4, 'h');			//串口发送函数
当使用串口初始化后 printf 被默认连接到第一个被初始化的串口上
printf("UART%d OK! Hello Kinetis\r\n", instance);




中断发送函数

    //注册发送中断回调函数 
    UART_CallbackTxInstall(HW_UART4, UART_TX_ISR);
    //打开发送完成中断 
    UART_ITDMAConfig(HW_UART4, kUART_IT_Tx);
//需要发送的数组
static const char UART_String1[] = "HelloWorld\r\n";
//中断发送函数
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




