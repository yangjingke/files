#ifndef _SystemBody_H
#define _SystemBody_H
#include "stm32f10x.h"
#include "protocal.h"
//#include "i2c_ee.h"
#include "WIN_24C02.h"
#include "SZ_SPI_FLASH.h"
#include "MessageSave.h"

	//CAN口 选择  1是CAN1  0是CAN2
	#define CANSelect  0


	//引脚宏定义
	#define HC165_GPIO  GPIOC
	#define HC165_GPIO_1  GPIOA
	#define HC165_SHLD_PIN  GPIO_Pin_4
	#define HC165_CLK_PIN  GPIO_Pin_4
	#define HC165_QH_PIN  GPIO_Pin_5

	#define HC165_SHLD_H GPIO_SetBits(HC165_GPIO, HC165_SHLD_PIN)
	#define HC165_SHLD_L GPIO_ResetBits(HC165_GPIO, HC165_SHLD_PIN)
	#define HC165_CLK_H GPIO_SetBits(HC165_GPIO_1, HC165_CLK_PIN)
	#define HC165_CLK_L GPIO_ResetBits(HC165_GPIO_1, HC165_CLK_PIN)
	#define HC165_QH_DATA GPIO_ReadInputDataBit(HC165_GPIO, HC165_QH_PIN)

	



	extern u16  WarnTimeCon;
	extern u16  CanComOvertimeCnt;
	extern DEVICEINFO deviceinfo;
	extern DATETIME running_time;
	extern u16 AD_Value[30][2];
	extern u16 anaMsg[];
	extern uint8_t canMsg1[];
	extern uint8_t canMsg2[];
	extern FlagStatus flag_TIMER_RX;
	extern FlagStatus fromWifi;
	extern FlagStatus flag_Fault;
	extern FlagStatus flag_Sendontime;
	extern uint32_t DEVICEADD;
	extern uint32_t DESTNATION;
	extern ALLKINDSTRUCT ScanData;
	extern uint8_t timeCounter;
	extern uint8_t warnCounter;
	extern uint8_t warnResetCounter;  
	extern u16   SendTimeCnt,RunTimeCnt;
	extern u16   TemperErrorValue;
	extern u8    ArresterErrorFlag;
    extern u8 CurrentBoundaryValue,TemperBoundaryValue;
	extern u16   CurrentHistoryValue;//电流历史值

	void CLK_Config(void);

	void  FirstSaveSystemInfo(void);
	void  getTimeandDeviceInfo(void);
	void recordTime(void); 
	void System_powerOn_Init(void);

	// void CLK_Config(void);
	void GPIO_Config(void);
	void ADC_Config(void);
	void DMA_Config(void);
	void NVIC_Configuration(void);
	void CAN_Config(void); 
	void TIM5_Init(void);  //creat scan data it
	void TIM4_Init(void); // creat record running time it
//	void ProtocolReply(uint8_t*);
	void Send_Instruction(uint8_t *SendInst);

	void SendOnceMessage(void);
	void Scan_Current_Info(void);

	void  ErrorTest(void);
	void  ErrorMessageContinueSend(void);
	void  ErrorRealtimeTest(void);

	void delay_ms(u16 time) ;
	//void Init_RxMes(CanRxMsg *RxMessage);
	void Delay (uint32_t nCount);
	void LED_Init(void);
	void TIM3_Int_Init(u16 arr,u16 psc);


 	void ReadHC165Data(void);
 
 
 
 
 
 
 
 
 
 #endif 
