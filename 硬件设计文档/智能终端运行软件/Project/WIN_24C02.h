#ifndef __WIN24C02_H__
#define __WIN24C02_H__

 #include "stm32f10x.h"	
 #include "SystemBody.h"

#define uchar unsigned char
#define uint  unsigned int
#define SCL           GPIO_Pin_10 //24C02 SCL
#define SDA           GPIO_Pin_11 //24C02 SDA
void AT24C02_Init(void);
uchar WIN24C02_read(uchar address);  //?24c02???address?????????

void WIN24C02_BufferRead(u8* pBuffer, u8 ReadAddr, u8 NumByteToRead);

void WIN24C02_write(uchar address,uchar info);  //?24c02?address??????????info

void WIN24C02_BufferWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);

void WIN24C02_init(void);   //24c02??????

void delay_nop(void);

void delay2(uint x);

void start(void);

void stop(void);

void writex(uchar j);

uchar readx(void);

void clock(void);
void AT24C02_SDA_IO_SET(unsigned char io_set);

#endif
