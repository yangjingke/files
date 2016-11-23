#include "ds1302.h"
/***************************************
***************************************/
struct TIME realtime={0,0,0,0,0,0};
struct TIME settime={0x15,0x33,0x15,0x04,0x03,0x16};//此处可直接设置系统的实时时间
struct TIME amendtime={0,0,0,0,0,0};
/*********************************************************

**********************************************************/
void DS1302_IO_PORT_DDR_SET(unsigned char dirFlag)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = DS1302_IO;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    if(dirFlag==1)
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;// 设置为输出
    else
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//设置为输入
    GPIO_Init(DS1302_CON_GPIO, &GPIO_InitStructure);
}
/**********************************************************
**********************************************************/
void DS1302_Write_Byte(unsigned char dat)
{
	unsigned char i;
        DS1302_IO_PORT_DDR_SET(1);
	for(i=0;i<8;i++)
	{
		if(dat&0x01)
			DS1302IO_SET;
		else
			DS1302IO_RST;
		DS1302SCLK_SET;
		DS1302SCLK_RST;
		dat>>=1;
	}
}
/**********************************************************
**********************************************************/
unsigned char 	DS1302_Read_Byte(void)
{
	unsigned char i;
	unsigned char dat=0;
        DS1302_IO_PORT_DDR_SET(0);
	for(i=0;i<8;i++)
	{
		 dat>>=1;
		 if(READDS1302IO)
		 	dat |= 0x80;
		 DS1302SCLK_SET;
		 DS1302SCLK_RST;
	}
        DS1302_IO_PORT_DDR_SET(1);;
	return dat;
}	
/**********************************************************
**********************************************************/
void DS1302_Write(unsigned char reg ,unsigned char dat)
{
	  DS1302RST_RST;
	  DS1302SCLK_RST;
	  DS1302RST_SET;
	  DS1302_Write_Byte(reg);
	  DS1302_Write_Byte(dat);
	  DS1302SCLK_SET;
	  DS1302RST_RST;
}
/**********************************************************
**********************************************************/
unsigned char DS1302_Read(unsigned char reg)
{
	unsigned char temp;
	reg |= 0x01;
	DS1302RST_RST;
	DS1302SCLK_RST;
	DS1302RST_SET;
	DS1302_Write_Byte(reg);
	temp = 	DS1302_Read_Byte();
	DS1302SCLK_SET;
	DS1302RST_RST;
	return temp;
}
/**********************************************************
**********************************************************/
void Set_DS1302(struct TIME * ptr)
{
	unsigned char flag;
	unsigned char addr = 0x80;
	DS1302_Write(ds1302_control_reg ,0x00) ;
	DS1302_Write(addr,0x80)	;
	addr +=2;
	DS1302_Write(addr,ptr->minute)	;
	addr +=2;
	DS1302_Write(addr,ptr->hour)	;
	addr +=2;
	DS1302_Write(addr,ptr->day)	;
	addr +=2;
	DS1302_Write(addr,ptr->month)	;
	addr +=4;
	DS1302_Write(addr,ptr->year)	;
	do
	{
		DS1302_Write(ds1302_sec_reg,ptr->second);
		flag = DS1302_Read(ds1302_sec_reg);
	}while(flag&0x80);
	DS1302_Write(ds1302_control_reg ,0x80) ;
}
/**********************************************************
**********************************************************/
void Get_DS1302(struct TIME * ptr)
{
	    unsigned char addr = 0x81;
		ptr->second = DS1302_Read(addr);
		addr += 2;
		ptr->minute = DS1302_Read(addr);
		addr += 2;
		ptr->hour = DS1302_Read(addr);
		addr += 2;
		ptr->day = DS1302_Read(addr);
		addr += 2;
		ptr->month = DS1302_Read(addr);
		addr += 4;
		ptr->year = DS1302_Read(addr);

}
/***************************************************
****************************************************/
unsigned char  BCD_convert(unsigned char x)
{
	unsigned char temp;
	temp = x/10;
	x=x%10;
	return ((temp<<4)+x);
}
/***************************************************
****************************************************/
unsigned char  BCD_convert_Reverse(unsigned char x)
{
	unsigned char temp;
	temp = (x>>4)*10;
	temp+=(x&0x0f);
	return temp;
}

//u8 Get_TimeString(u8 *timebuf,u8 tempcnt)
//{
//  u8 time=0;
//  Get_DS1302(&realtime);
//  tempcnt=tempcnt+ChangeNumToChar(BCD_convert_Reverse(realtime.month),tempcnt,0);
//  timebuf[tempcnt++]='-';
//  tempcnt=tempcnt+ChangeNumToChar(BCD_convert_Reverse(realtime.day),tempcnt,0);
//  timebuf[tempcnt++]=' ';
//  time=BCD_convert_Reverse(realtime.hour);
//  if(time)
//	tempcnt=tempcnt+ChangeNumToChar(time,tempcnt,0);
//  else
//	{timebuf[tempcnt++]='0';timebuf[tempcnt++]='0'+time;}
//  timebuf[tempcnt++]=':';
//  time=BCD_convert_Reverse(realtime.minute);
//  if(time>9)
//	tempcnt=tempcnt+ChangeNumToChar(time,tempcnt,0);
//  else
//	{timebuf[tempcnt++]='0';timebuf[tempcnt++]='0'+time;}
//  return tempcnt;
//}
