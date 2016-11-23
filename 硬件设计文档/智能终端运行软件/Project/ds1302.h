#ifndef __U_DS1302_H__
#define __U_DS1302_H__
#include "stm32f10x.h"

struct TIME
{
    unsigned char second;
    unsigned char minute;	
    unsigned char hour;		
    unsigned char day;		
    unsigned char month;		
    unsigned char year;		
};


extern struct TIME realtime;
extern struct TIME settime;//此处可直接设置系统的实时时间
extern struct TIME amendtime;
#define DS1302DELAY  10
/******************************************************************************
*******************************************************************************/
/*******************************************************
控制端口
SCLK -- PE3  RST -- PE1 IO -- PE2
********************************************************/
#define DS1302_CON_GPIO   GPIOE

#define DS1302_RST    GPIO_Pin_13
#define DS1302_IO     GPIO_Pin_12
#define DS1302_SCLK   GPIO_Pin_11

#define	DS1302RST_SET   GPIO_SetBits(DS1302_CON_GPIO, DS1302_RST)
#define	DS1302RST_RST   GPIO_ResetBits(DS1302_CON_GPIO, DS1302_RST)
#define	DS1302IO_SET    GPIO_SetBits(DS1302_CON_GPIO, DS1302_IO)	
#define	DS1302IO_RST    GPIO_ResetBits(DS1302_CON_GPIO, DS1302_IO)
#define	DS1302SCLK_SET  GPIO_SetBits(DS1302_CON_GPIO, DS1302_SCLK)	
#define	DS1302SCLK_RST  GPIO_ResetBits(DS1302_CON_GPIO, DS1302_SCLK)

#define READDS1302IO    GPIO_ReadInputDataBit(DS1302_CON_GPIO, DS1302_IO)

/**********************************************************
**********************************************************/
#define ds1302_sec_reg 0x80
#define ds1302_min_reg 0x82
#define ds1302_hr_reg 0x84 	
#define ds1302_date_reg 0x86
#define ds1302_month_reg 0x88
#define ds1302_day_reg 0x8a 	
#define ds1302_year_reg 0x8c 	
#define ds1302_control_reg 0x8e
#define ds1302_charger_reg 0x90
#define ds1302_clkburst_reg 0xbe
/***********************************
***********************************/
void DS1302_Write_Byte(unsigned char dat);
unsigned char 	DS1302_Read_Byte(void);
void DS1302_Write(unsigned char reg ,unsigned char dat);
unsigned char DS1302_Read(unsigned char reg);
void Set_DS1302(struct TIME * ptr);
void Get_DS1302(struct TIME * ptr);
unsigned char  BCD_convert(unsigned char x);
unsigned char  BCD_convert_Reverse(unsigned char x);
//u8 Get_TimeString(u8 *timebuf,u8 tempcnt);
#endif

