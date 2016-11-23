#include "WIN_24C02.h"
void delay2(uint x)
{
   uint i;
   for(i=0;i<x;i++);
}

void delay_nop(void)
{
  uint8_t i=10; //i=10??1.5us//???????? ,??????5????
   while(i--);

}

 

void WIN24C02_init()
{
   
   //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
  delay_nop();
   //SDA=1;
   GPIO_SetBits(GPIOB,SDA);
   delay_nop();
}

void start()
{
   //SDA=1;
   GPIO_SetBits(GPIOB,SDA);
   delay_nop();
   //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
   delay_nop();
   //SDA=0;
    GPIO_ResetBits(GPIOB, SDA);
   delay_nop();
   //SCL=0;
   GPIO_ResetBits(GPIOB, SCL);
   delay_nop();
}

void stop()

{
   //SDA=0;
   GPIO_ResetBits(GPIOB, SDA);
   delay_nop();
   //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
   delay_nop();
   //SDA=1;
   GPIO_SetBits(GPIOB,SDA);
   delay_nop();
}

void writex(uchar j)

{
   uchar i,temp,temp1;

   temp=j;
  //AT24C02_SDA_IO_SET(0);
   for (i=0;i<8;i++)
   {
      temp1=temp & 0x80;
      temp=temp<<1;
     
      //SCL=0;
   GPIO_ResetBits(GPIOB, SCL);
      delay_nop();

   //SDA=CY;
  if(temp1==0x80)
   {GPIO_SetBits(GPIOB, SDA);}
  else
   {GPIO_ResetBits(GPIOB, SDA);}
     

      delay_nop();
     // SCL=1;
  GPIO_SetBits(GPIOB,SCL);
      delay_nop();
   }
 //AT24C02_SDA_IO_SET(0);
   //SCL=0;
    GPIO_ResetBits(GPIOB, SCL);
   delay_nop();
   //SDA=1;
   GPIO_SetBits(GPIOB,SDA);
   delay_nop();

}

uchar readx()
{
   uchar i,j,k=0;

   //SCL=0;
    GPIO_ResetBits(GPIOB, SCL);
    delay_nop(); 
 //SDA=1;
 GPIO_SetBits(GPIOB,SDA);

 AT24C02_SDA_IO_SET(1);
   for (i=0;i<8;i++)
   {
      delay_nop();
      //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
      delay_nop();
      //if (SDA==1) j=1;
  if( GPIO_ReadInputDataBit(GPIOB,SDA)==1 ) 
   {j=1;}
      else 
   {j=0;}
      k=(k<<1)|j;
      //SCL=0;
    GPIO_ResetBits(GPIOB, SCL);
   }
    AT24C02_SDA_IO_SET(0);
   delay_nop();
   return(k);

}

void clock()

{
   uint i=0;
      AT24C02_SDA_IO_SET(1);
   //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
   delay_nop();
   while ((GPIO_ReadInputDataBit(GPIOB,SDA)==1)&&(i<5000))i++;
   //SCL=0;
    GPIO_ResetBits(GPIOB, SCL);
   delay_nop();
   AT24C02_SDA_IO_SET(0);

}

uchar WIN24C02_read(uchar address)

{
   uchar i;
   start();
   writex(0xa0);
   clock();
   writex(address);
   clock();
   start();
   writex(0xa1);
   clock();
   i=readx();
   stop();
   //delay2(10);
   delay2(50);
   return(i);

}

void WIN24C02_BufferRead(u8* pBuffer, u8 ReadAddr, u8 NumByteToRead)
{
    u8 i=0;
	for(i=0;i<NumByteToRead;i++)
	{
		*pBuffer=WIN24C02_read(ReadAddr);
		ReadAddr++;
		pBuffer++;
	}
}

void WIN24C02_write(uchar address,uchar info)

{

   start();
   writex(0xa0);
   clock();
   writex(address);
   clock();
   writex(info);
   clock();
   stop();
   //delay2(50);
   delay2(250);

}
void WIN24C02_BufferWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
    u8 i=0;
	for(i=0;i<NumByteToWrite;i++)
	{
		WIN24C02_write(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
//		delay2(10000);
		delay_ms(1);
	}
}
void AT24C02_SDA_IO_SET(unsigned char io_set) //SDA????????
{ 
	GPIO_InitTypeDef GPIO_InitStructure; 
    if(io_set==0)
  {
  GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA ????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  }
 else if(io_set==1)
  {
  GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA ????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //????
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  }
 else
  {;}
}

 void AT24C02_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

  GPIO_InitStructure.GPIO_Pin = SCL;          //24C02 SCL
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA ????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
    WIN24C02_init();
     //delayms(5000);//??
}


