#ifndef _USART1DEAL_H
#define _USART1DEAL_H
#include "stm32f10x.h"
#include "protocal.h"
#include "USART.H"


extern u8 DebugModFlag;//1实时状态   2历史状态    3配置模式
extern u8 WIFISendFlag,GPRSSendFlag;
extern u8 DebugSendFlag;
extern u8 ComModeSelect;	//通信模式 1：WIFI  2：GPRS   3:WIFI+GPRSZ自动  4:WIFI+GPRS同时
extern u8 ArresterControlEn;	//制动器控制使能    0：不控制和检测制动器   1：使能制动器控制和检测

void Usart1RxPackgeDeal(unsigned char * UsartBuff,unsigned char dataNum);

#endif

