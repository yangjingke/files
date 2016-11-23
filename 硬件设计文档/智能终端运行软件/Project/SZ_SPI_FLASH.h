/******************** (C) COPYRIGHT 2013 www.armjishu.com  ********************
 * �ļ���  ��SZ_SPI_FLASH.h
 * ����    ��ʵ��STM32F107VC����IV�ſ������SPI�洢��W25Q16_W2XQ16�ײ㺯��
 * ʵ��ƽ̨��STM32���ۿ�����
 * ��׼��  ��STM32F10x_StdPeriph_Driver V3.5.0
 * ����    ��www.armjishu.com 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SZ_SPI_FLASH_H
#define __SZ_SPI_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_spi.h"

/* ���ܺ꿪��  ���δʹ����Щ���ܣ����򿪶�Ӧ�ĺ꿪�ؿ��Լ�С�������Ĵ�С */
//#define       USE_ADC
//#define       USE_RTC
//#define       USE_TIM5
//#define       USE_TIM3
//#define       USE_PVD
//#define       USE_IWDG
//#define       USE_RTC_ALARM

/* �������궨�� bitband macro ------------------------------------------------*/
/* ʹ��bitband����������bit�����ĳ����Ч�ʣ�����GPIO�ܽŵĿ���Ч����Ϊ���� */
/* ��������� ��32MB����������ķ���ӳ��Ϊ��1MB ����bit-band���ķ���(ʵ�ʴ�С����оƬ�й�) */
#define Periph_BASE         0x40000000  // �������ַ Peripheral 
#define Periph_BB_BASE      0x42000000  // �������������ַ Peripheral bitband

/* ע�⣺���볣������ʱ���ڱ���ʱ��������������������ַ�������ܴﵽ����Ч�ʵ�Ŀ��(�Ƽ�)
         ����������������ֻ��������ʱ��STM32�Լ������������ַ��Ч�ʻ����ۿ�(���Ƽ�) */
#define Periph_BB(PeriphAddr, BitNumber)    \
          *(__IO uint32_t *) (Periph_BB_BASE | ((PeriphAddr - Periph_BASE) << 5) | ((BitNumber) << 2))
	 
#define Periph_ResetBit_BB(PeriphAddr, BitNumber)    \
          (*(__IO uint32_t *) (Periph_BB_BASE | ((PeriphAddr - Periph_BASE) << 5) | ((BitNumber) << 2)) = 0)
   
#define Periph_SetBit_BB(PeriphAddr, BitNumber)       \
          (*(__IO uint32_t *) (Periph_BB_BASE | ((PeriphAddr - Periph_BASE) << 5) | ((BitNumber) << 2)) = 1)

#define Periph_GetBit_BB(PeriphAddr, BitNumber)       \
          (*(__IO uint32_t *) (Periph_BB_BASE | ((PeriphAddr - Periph_BASE) << 5) | ((BitNumber) << 2)))

/* ����GPIO����������������壬nΪbitλ�÷�ΧΪ0��15    */
/* ���Ƕ�GPIOA.15����������Ҫ��ʼ��GPIO��Ȼ��ʹ�÷����� */
/* ��GPIOA.15����͵�ƽ��   PAOutBit(15) = 0;           */
/* ��GPIOA.15����͵�ƽ��   PAOutBit(15) = 1;           */
/* ��ȡGPIOA.15����ĵ�ƽ�� data = PAInBit(15);         */
#define PAOutBit(n)     Periph_BB((uint32_t)&GPIOA->IDR,n)  //��� 
#define PASetBit(n)     (PAOutBit(n) = 1)                   //��� ��
#define PAResetBit(n)   (PAOutBit(n) = 0)                   //��� ��
#define PAInBit(n)      Periph_BB((uint32_t)&GPIOA->IDR,n)  //���� 

#define PBOutBit(n)     Periph_BB((uint32_t)&GPIOB->ODR,n)  //��� 
#define PBSetBit(n)     (PBOutBit(n) = 1)                   //��� ��
#define PBResetBit(n)   (PBOutBit(n) = 0)                   //��� ��
#define PBInBit(n)      Periph_BB((uint32_t)&GPIOB->IDR,n)  //���� 

#define PCOutBit(n)     Periph_BB((uint32_t)&GPIOC->ODR,n)  //��� 
#define PCSetBit(n)     (PCOutBit(n) = 1)                   //��� ��
#define PCResetBit(n)   (PCOutBit(n) = 0)                   //��� ��
#define PCInBit(n)      Periph_BB((uint32_t)&GPIOC->IDR,n)  //���� 

#define PDOutBit(n)     Periph_BB((uint32_t)&GPIOD->ODR,n)  //��� 
#define PDSetBit(n)     (PDOutBit(n) = 1)                   //��� ��
#define PDResetBit(n)   (PDOutBit(n) = 0)                   //��� ��
#define PDInBit(n)      Periph_BB((uint32_t)&GPIOD->IDR,n)  //���� 

#define PEOutBit(n)     Periph_BB((uint32_t)&GPIOE->ODR,n)  //��� 
#define PESetBit(n)     (PEOutBit(n) = 1)                   //��� ��
#define PEResetBit(n)   (PEOutBit(n) = 0)                   //��� ��
#define PEInBit(n)      Periph_BB((uint32_t)&GPIOE->IDR,n)  //����

#define PFOutBit(n)     Periph_BB((uint32_t)&GPIOF->ODR,n)  //��� 
#define PFSetBit(n)     (PFOutBit(n) = 1)                   //��� ��
#define PFResetBit(n)   (PFOutBit(n) = 0)                   //��� ��
#define PFInBit(n)      Periph_BB((uint32_t)&GPIOF->IDR,n)  //����

#define PGOutBit(n)     Periph_BB((uint32_t)&GPIOG->ODR,n)  //��� 
#define PGSetBit(n)     (PGOutBit(n) = 1)                   //��� ��
#define PGResetBit(n)   (PGOutBit(n) = 0)                   //��� ��
#define PGInBit(n)      Periph_BB((uint32_t)&GPIOG->IDR,n)  //����


/* �ڲ�SRAM������ ��32MB SRAM�������ķ���ӳ��Ϊ��1MB SRAMbit-band���ķ���(ʵ�ʴ�С����оƬ�й�) */
#define RAM_BASE            0x20000000  // �ڲ�SRAM����ַ  
#define RAM_BB_BASE         0x22000000  // �ڲ�SRAM����������ַ

#define SRAM_ResetBit_BB(VarAddr, BitNumber)    \
          (*(__IO uint32_t *) (RAM_BB_BASE | ((VarAddr - RAM_BASE) << 5) | ((BitNumber) << 2)) = 0)
   
#define SRAM_SetBit_BB(VarAddr, BitNumber)       \
          (*(__IO uint32_t *) (RAM_BB_BASE | ((VarAddr - RAM_BASE) << 5) | ((BitNumber) << 2)) = 1)

#define SRAM_GetBit_BB(VarAddr, BitNumber)       \
          (*(__IO uint32_t *) (RAM_BB_BASE | ((VarAddr - RAM_BASE) << 5) | ((BitNumber) << 2)))

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

#define  FLASH_WriteAddress         0x00000
#define  FLASH_ReadAddress          FLASH_WriteAddress
#define  FLASH_SectorToErase        FLASH_WriteAddress
/**
  * @brief  SPI_FLASH SPI Interface pins
  */
#define  W25X16_FLASH_ID            0xEF3015
#define  W25Q16_FLASH_ID            0xEF4015

/*----- High layer function -----*/
void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_ChipErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);

/*----- Low layer function -----*/
u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength);
void SPI_FLASH_Test(void);

#endif /* __SZ_SPI_FLASH_H */

/******************* (C) COPYRIGHT 2010 www.armjishu.com *****END OF FILE****/
