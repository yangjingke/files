#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h"
#include "protocal.h"
#include "MessageSave.h"


#define RxC 100


extern uint8_t Usart1Rxcount;         
extern FlagStatus Usart1RxOkFlag;        
extern uint8_t Usart1OverTimeFlagCount;    
extern FlagStatus Usart1OverTimeFlag;      
extern uint8_t Usart1RxBuffer[]; 
 
extern uint8_t Usart2Rxcount;         
extern FlagStatus Usart2RxOkFlag;        
extern uint8_t Usart2OverTimeFlagCount;    
extern FlagStatus Usart2OverTimeFlag;      
extern uint8_t Usart2RxBuffer[]; 

extern uint8_t Usart3Rxcount;         
extern FlagStatus Usart3RxOkFlag;        
extern uint8_t Usart3OverTimeFlagCount;    
extern FlagStatus Usart3OverTimeFlag;      
extern uint8_t Usart3RxBuffer[]; 

extern uint8_t Uart4Rxcount;         
extern FlagStatus Uart4RxOkFlag;        
extern uint8_t Uart4OverTimeFlagCount;    
extern FlagStatus Uart4OverTimeFlag;      
extern uint8_t Uart4RxBuffer[]; 


	void USART_Config(void);

	void USART1_SendData(uint8_t TxData);
	void USART1_SendMData(uint8_t *DataPro,uint8_t Num);
	void USART2_SendData(uint8_t TxData);
	void USART2_SendMData(uint8_t *DataPro,uint8_t Num);
	void USART3_SendData(uint8_t TxData);
	void USART3_SendMData(uint8_t *DataPro,uint8_t Num);
	void UART4_SendData(uint8_t TxData);
	void UART4_SendMData(uint8_t *DataPro,uint8_t Num);

	void SysTickConfig(void);
#endif

