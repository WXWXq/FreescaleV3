
#include "head.h"
#include "outputdata.h"
float OutData[4] = { 0 };
 
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++)
		{      
        CRC_Temp ^= Buf[i];//????  ???
        for (j=0;j<8;j++) 
				{
            if (CRC_Temp & 0x01)//?????1
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
// 		Buf[8] = CRC_Temp & 0x00ff;  //??9?????CRC????
//     Buf[9] = CRC_Temp >>8;//?10????CRC????
    return(CRC_Temp);
}
unsigned char databuf[10] = {0};
unsigned int temp1[4] = {0};
 int temp[4] = {0};
void OutPut_Data(void)
{
   
  
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
  {
    
    temp[i]  = (int)OutData[i];
    temp1[i] = (unsigned int)temp[i];
    
  }
   
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }
  
  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  
  for(i=0;i<10;i++)
	{
		while(!((UART4->S1) & UART_S1_TDRE_MASK));
		UART4->D=databuf[i];
	}
}
