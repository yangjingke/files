#ifndef _MessageSave_H
#define _MessageSave_H
#include "stm32f10x.h"
#include "protocal.h"
//#include "i2c_ee.h"
#include "WIN_24C02.h"
#include "SZ_SPI_FLASH.h"
#include "USART.H"
#include "Usart1Deal.H"
//数据保存宏定义
#define FlashSaveStartAddr 0
#define FlashSaveEndAddr   (512*4096-1)

#define DatagramLength	  (93+18)
//状态信息保存宏定义
#define StatusMessageSaveStartAddr   0
#define StatusMessageSaveEndAddr     (StatusMessageSaveSectorNum*4096-1)
#define StatusMessageSaveSectorNum   512
#define StatusMessageSaveSingleNum   (uint32_t)(4096/DatagramLength)
#define StatusMessageSaveTotalNum    (StatusMessageSaveSectorNum*StatusMessageSaveSingleNum)

extern   u32 StatusMessageStartAddr,StatusMessageEndAddr,StatusMessageNumber;  // FLASH数据保存用变量
extern   u32	  StatusReadStartAddr,StatusReadEndAddr,StatusReadNumber;  //读FLASH数据用变量
extern   u8    StatusReadEnFlag;
extern   u8    MessageReadFlag[];//0 网络缓存数据读取标志   1 单机历史数据读取标志
extern   u32   MessageReadNumberBuf[];//0 网络缓存数据读取数量   1 单机历史数据读取数量
extern   u8    MessageReadStatus; //数据读取状态  0  没有读取数据   1 网络读取缓存数据  2单机读取历史数据


//数据读取前参数设置
void StatusMessageReadInit(void);
void StatusMessageReadFromFlash(void);
//数据保存
void StatusMessageSaveToFlash(uint8_t *Message);
void SaveFlashpara(void);
void ReadFlashpara(void);

#endif
