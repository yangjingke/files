#ifndef _USART1DEAL_H
#define _USART1DEAL_H
#include "stm32f10x.h"
#include "protocal.h"
#include "USART.H"


extern u8 DebugModFlag;//1ʵʱ״̬   2��ʷ״̬    3����ģʽ
extern u8 WIFISendFlag,GPRSSendFlag;
extern u8 DebugSendFlag;
extern u8 ComModeSelect;	//ͨ��ģʽ 1��WIFI  2��GPRS   3:WIFI+GPRSZ�Զ�  4:WIFI+GPRSͬʱ
extern u8 ArresterControlEn;	//�ƶ�������ʹ��    0�������ƺͼ���ƶ���   1��ʹ���ƶ������ƺͼ��

void Usart1RxPackgeDeal(unsigned char * UsartBuff,unsigned char dataNum);

#endif

