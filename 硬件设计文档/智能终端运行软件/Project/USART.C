#include "USART.H"
#include "Usart1Deal.H"

uint8_t Usart1Rxcount=0;         
FlagStatus Usart1RxOkFlag=RESET;        
uint8_t Usart1OverTimeFlagCount=0;    
FlagStatus Usart1OverTimeFlag=RESET;      
uint8_t Usart1RxBuffer[RxC]; 

uint8_t Usart2Rxcount=0;         
FlagStatus Usart2RxOkFlag=RESET;        
uint8_t Usart2OverTimeFlagCount=0;    
FlagStatus Usart2OverTimeFlag=RESET;      
uint8_t Usart2RxBuffer[RxC];  

uint8_t Usart3Rxcount=0;         
FlagStatus Usart3RxOkFlag=RESET;        
uint8_t Usart3OverTimeFlagCount=0;    
FlagStatus Usart3OverTimeFlag=RESET;      
uint8_t Usart3RxBuffer[RxC];

uint8_t Uart4Rxcount=0;         
FlagStatus Uart4RxOkFlag=RESET;        
uint8_t Uart4OverTimeFlagCount=0;    
FlagStatus Uart4OverTimeFlag=RESET;      
uint8_t Uart4RxBuffer[RxC]; 

void USART_Config(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;   //USART1
    USART_InitTypeDef USART_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure2;  //USART2
    USART_InitTypeDef USART_InitStructure2;
	  /* 使能STM32的USART1对应GPIO的Clock时钟 使能STM32的USART1的Clock时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	 /* 使能STM32的USART2 对应GPIO的Clock时钟 使能STM32的USART2的管脚重映射  重映射  使能STM32的USART2的Clock时钟 */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
   	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4|RCC_APB1Periph_UART5, ENABLE);
    /* 初始化STM32的USART1的TX管脚，配置为复用功能推挽输出 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);

    /* 初始化STM32的USART1的RX管脚，配置为复用功能输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
    /* 初始化STM32的USART2的TX管脚，配置为复用功能推挽输出 */
	  GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure2);

    /* 初始化STM32的USART2的RX管脚，配置为复用功能输入 */
    GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure2.GPIO_Pin =  GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure2);

	 
	/* 初始化STM32的USART3的TX管脚，配置为复用功能推挽输出 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD , &GPIO_InitStructure);

    /* 初始化STM32的USART1的RX管脚，配置为复用功能输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOD , &GPIO_InitStructure);
	/* UART4 configure*/
      /* Configure UART4 Rx (PC.11) as input floating */
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      /* Configure USART4 Tx (PC.10) as alternate function push-pull */
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOC, &GPIO_InitStructure);


		    /* 配置USART1*/
   	USART_InitStructure.USART_BaudRate = 115200;              //串口的波特率，例如115200 最高达4.5Mbits/s
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据字长度(8位或9位)
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //可配置的停止位-支持1或2个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;         //无奇偶校验  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //双工模式，使能发送和接收
    		/* 配置USART2*/
		USART_InitStructure2.USART_BaudRate = 115200;              //串口的波特率，例如115200 最高达4.5Mbits/s
    USART_InitStructure2.USART_WordLength = USART_WordLength_8b; //数据字长度(8位或9位)
    USART_InitStructure2.USART_StopBits = USART_StopBits_1;      //可配置的停止位-支持1或2个停止位
    USART_InitStructure2.USART_Parity = USART_Parity_No;         //无奇偶校验  
    USART_InitStructure2.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控制
    USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //双工模式，使能发送和接收
    /* 根据传入的参数初始化STM32的USART1配置 */
    USART_Init(USART1, &USART_InitStructure);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //RECEIVE NVIC
    /* 使能STM32的USART1功能模块 */
    USART_Cmd(USART1, ENABLE);     
		 /* 根据传入的参数初始化STM32的USART2配置 */
		USART_Init(USART2, &USART_InitStructure2);
	   USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//RECEIVE NVIC
    /* 使能STM32的USART2功能模块 */
    USART_Cmd(USART2, ENABLE);

	USART_Init(USART3, &USART_InitStructure);
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //RECEIVE NVIC
    /* 使能STM32的USART1功能模块 */
    USART_Cmd(USART3, ENABLE); 

	   USART_InitStructure.USART_BaudRate = 115200;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        /* Configure UART4 as TTL USART used */
        USART_Init(UART4, &USART_InitStructure);
        /* Enable UART4 Receive and Transmit interrupts */
        USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
        /* Enable the UART4 */
        USART_Cmd(UART4, ENABLE);

}

void USART1_SendData(uint8_t TxData)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==0);
	USART_SendData(USART1,TxData);
}
void USART1_SendMData(uint8_t *DataPro,uint8_t Num)
{
	uint8_t i=0;
	for(i=0;i<Num;i++)
	{
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==0);
		USART1_SendData(DataPro[i]);
	}
}



void USART2_SendData(uint8_t TxData)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==0);
	USART_SendData(USART2,TxData);
}

void USART2_SendMData(uint8_t *DataPro,uint8_t Num)
{
	uint8_t i=0;
	for(i=0;i<Num;i++)
	{
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==0);
		USART2_SendData(DataPro[i]);
	}
}

void USART3_SendData(uint8_t TxData)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==0);
	USART_SendData(USART3,TxData);
}

void USART3_SendMData(uint8_t *DataPro,uint8_t Num)
{
	uint8_t i=0;
	for(i=0;i<Num;i++)
	{
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==0);
		USART3_SendData(DataPro[i]);
	}
}

void Send_Instruction(uint8_t *SendInst)
{
//	uint8_t Txc=0;
//	while(SendInst[Txc]!=PROTO_TAIL)
//	{
//		USART1_SendData(SendInst[Txc++]);
//
//	}
//	USART1_SendData(PROTO_TAIL);
//	Txc=0;
	if(DebugModFlag==1)//实时状态模式下发送状态信息	 
		USART1_SendMData(SendInst,DatagramLength);
	if(DebugModFlag!=3)
	{
		switch(ComModeSelect)
		{
			case 1:	if(LinkStatuOnlineFlag)
						USART3_SendMData(SendInst,DatagramLength);//WIFI
					break;
			case 2:	if(LinkStatuOnlineFlag)
						UART4_SendMData(SendInst,DatagramLength); //GPRS
					break;
			case 3:	if(LinkStatuOnlineFlag==1||LinkStatuOnlineFlag==3)
						USART3_SendMData(SendInst,DatagramLength);//WIFI
					else if(LinkStatuOnlineFlag==2)
						UART4_SendMData(SendInst,DatagramLength); //GPRS
					break;
			case 4:	if(LinkStatuOnlineFlag)
						USART3_SendMData(SendInst,DatagramLength);//WIFI					
						UART4_SendMData(SendInst,DatagramLength); //GPRS
					break;
		}
//		USART3_SendMData(SendInst,DatagramLength);//WIFI 
//		UART4_SendMData(SendInst,DatagramLength); //GPRS
	}
//	while(SendInst[Txc]!=PROTO_TAIL)
//	{
//		USART2_SendData(SendInst[Txc++]);
//
//	}
//	USART2_SendData(PROTO_TAIL);




//  if(!fromWifi)
//	{
//			while(SendInst[Txc]!=PROTO_TAIL)
//			{
//				USART1_SendData(SendInst[Txc++]);
//		
//			}
//		
//	}
//	else
//	{
//			while(SendInst[Txc]!=PROTO_TAIL)
//			{
//				USART2_SendData(SendInst[Txc++]);
//		
//			}
//		
//	}	
//	if(!fromWifi)
//		{
//				USART1_SendData(PROTO_TAIL);	
//		}
//	else
//		{
//				USART2_SendData(PROTO_TAIL);	
//		}
}



void UART4_SendData(uint8_t TxData)
{
	while(USART_GetFlagStatus(UART4,USART_FLAG_TXE)==0);
	USART_SendData(UART4,TxData);
}

void UART4_SendMData(uint8_t *DataPro,uint8_t Num)
{
	uint8_t i=0;
	for(i=0;i<Num;i++)
	{
		
		UART4_SendData(DataPro[i]);
	}
}

void SysTickConfig(void)
{
  SysTick_Config(72000*50);//SystemCoreClock 24000 -- 1ms   max 233ms
}

