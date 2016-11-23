#ifndef _MessageSave_H
#define _MessageSave_H
#include "stm32f10x.h"
#include "protocal.h"
//#include "i2c_ee.h"
#include "WIN_24C02.h"
#include "SZ_SPI_FLASH.h"
#include "USART.H"
#include "Usart1Deal.H"
//���ݱ���궨��
#define FlashSaveStartAddr 0
#define FlashSaveEndAddr   (512*4096-1)

#define DatagramLength	  (93+18)
//״̬��Ϣ����궨��
#define StatusMessageSaveStartAddr   0
#define StatusMessageSaveEndAddr     (StatusMessageSaveSectorNum*4096-1)
#define StatusMessageSaveSectorNum   512
#define StatusMessageSaveSingleNum   (uint32_t)(4096/DatagramLength)
#define StatusMessageSaveTotalNum    (StatusMessageSaveSectorNum*StatusMessageSaveSingleNum)

extern   u32 StatusMessageStartAddr,StatusMessageEndAddr,StatusMessageNumber;  // FLASH���ݱ����ñ���
extern   u32	  StatusReadStartAddr,StatusReadEndAddr,StatusReadNumber;  //��FLASH�����ñ���
extern   u8    StatusReadEnFlag;
extern   u8    MessageReadFlag[];//0 ���绺�����ݶ�ȡ��־   1 ������ʷ���ݶ�ȡ��־
extern   u32   MessageReadNumberBuf[];//0 ���绺�����ݶ�ȡ����   1 ������ʷ���ݶ�ȡ����
extern   u8    MessageReadStatus; //���ݶ�ȡ״̬  0  û�ж�ȡ����   1 �����ȡ��������  2������ȡ��ʷ����


//���ݶ�ȡǰ��������
void StatusMessageReadInit(void);
void StatusMessageReadFromFlash(void);
//���ݱ���
void StatusMessageSaveToFlash(uint8_t *Message);
void SaveFlashpara(void);
void ReadFlashpara(void);

#endif
