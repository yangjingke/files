/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "protocal.h"
#include "USART.H"
//extern DATETIME running_time;
//extern uint8_t RxCounter;
//extern uint8_t RX_Data_temp[];
//extern FlagStatus flag_Finish_RX;
 
extern uint8_t canupdateflag1;
extern uint8_t canupdateflag2;


extern FlagStatus flag_Running_time;
extern u16 AD_Value[30][2];
extern u16 anaMsg[];

extern FlagStatus beginRecv;
extern uint8_t canMsg1[];   //CAN data 1;
extern uint8_t  canMsg2[];   //CAN data 2;
extern FlagStatus flag_TIMER_RX;
extern uint8_t timeCounter;
//1216  增加
extern u16  CanComOvertimeCnt;
extern u16  WarnTimeCon;
extern uint8_t warnCounter;
extern ALLKINDSTRUCT ScanData;
extern u16 CurrentErrorCnt;

extern uint8_t LED_flag;

extern u16   SendTimeCnt,RunTimeCnt;
uint8_t LED_Cnt=0;

CanRxMsg RxMessage;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	static unsigned int lightCountFlag,sysTickCout;
    if(sysTickCout>19)
    {
          sysTickCout = 0;
          if(lightCountFlag%2==0)
          {
              GPIO_ResetBits(GPIOE ,  GPIO_Pin_7);
          }
          else
          {
              GPIO_SetBits(GPIOE ,  GPIO_Pin_7);
          }
		  if(lightCountFlag<60000)
           lightCountFlag++;
		  else
		   lightCountFlag=0;
     
    }
    else
    {
       sysTickCout++;       
    }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
void USART1_IRQHandler(void)
{  
	 if(USART_GetITStatus(USART1, USART_IT_RXNE)!= RESET)
  { 
      USART_ClearITPendingBit(USART1, USART_IT_RXNE);
      Usart1RxBuffer[Usart1Rxcount] = USART_ReceiveData(USART1); 
      if(Usart1OverTimeFlagCount<4) //???
      {
          Usart1OverTimeFlag=SET;
          Usart1OverTimeFlagCount=0;
          Usart1Rxcount++;
      }
	  if(Usart1Rxcount>=100)
	  {
			Usart1Rxcount=0;          
 		 Usart1RxOkFlag=SET;
	  }
  }
	
	
	
	

	
	
	
	
	
	
	
	
	
	
	
//	uint8_t tempda;
//	if (USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
//	//if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
//	{
//		tempda=(USART_ReceiveData(USART1)&0X7F);
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//		if (tempda==PROTO_HEAD)
//		{	
//			RxCounter=0;
//			beginRecv=SET;
//		}
//		if(tempda==PROTO_TAIL)
//		{
//			flag_Finish_RX=SET;
//	    beginRecv=RESET;
//		}
//		if(beginRecv==SET||tempda==PROTO_TAIL)
//		{				
//			RX_Data_temp[RxCounter++]=tempda;
//		}
//	}
	
}
void USART2_IRQHandler(void)
{
	 if(USART_GetITStatus(USART2, USART_IT_RXNE)!= RESET)
  { 
      USART_ClearITPendingBit(USART2, USART_IT_RXNE);
      Usart2RxBuffer[Usart2Rxcount] = USART_ReceiveData(USART2); 
      if(Usart2OverTimeFlagCount<4) //???
      {
          Usart2OverTimeFlag=SET;
          Usart2OverTimeFlagCount=0;
          Usart2Rxcount++;
      }
	  if(Usart2Rxcount>=100)
	  {
			Usart2Rxcount=0;          
 		 Usart2RxOkFlag=SET;
	  }
  }

	
//    uint8_t tempda;
//		if (USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
//		{
//			tempda=(USART_ReceiveData(USART2)&0X7F);
//			USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//			if (tempda==PROTO_HEAD)
//			{	
//				RxCounter=0;
//				beginRecv=SET;
//			}
//			if(tempda==PROTO_TAIL)
//			{
//				flag_Finish_RX=SET;
//	      beginRecv=RESET;
//			}
//			if(beginRecv==SET||tempda==PROTO_TAIL)
//			{				
//				RX_Data_temp[RxCounter++]=tempda;
//			}
//		}
}


void USART3_IRQHandler(void)
{
	 if(USART_GetITStatus(USART3, USART_IT_RXNE)!= RESET)
  { 
      USART_ClearITPendingBit(USART3, USART_IT_RXNE);
      Usart3RxBuffer[Usart3Rxcount] = USART_ReceiveData(USART3); 
      if(Usart3OverTimeFlagCount<4) //???
      {
          Usart3OverTimeFlag=SET;
          Usart3OverTimeFlagCount=0;
          Usart3Rxcount++;
      }
	  if(Usart3Rxcount>=100)
	  {
			Usart3Rxcount=0;          
 		 Usart3RxOkFlag=SET;
	  }
  }
}

void UART4_IRQHandler(void)
{
	 if(USART_GetITStatus(UART4, USART_IT_RXNE)!= RESET)
  { 
      USART_ClearITPendingBit(UART4, USART_IT_RXNE);
      Uart4RxBuffer[Uart4Rxcount] = USART_ReceiveData(UART4); 
      if(Uart4OverTimeFlagCount<4) //???
      {
          Uart4OverTimeFlag=SET;
          Uart4OverTimeFlagCount=0;
          Uart4Rxcount++;
      }
	  if(Uart4Rxcount>=100)
	  {
			Uart4Rxcount=0;          
 		 Uart4RxOkFlag=SET;
	  }
  }
}
void CAN1_RX0_IRQHandler(void)
{ 
		uint8_t i=0;
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		if ((RxMessage.ExtId == 0x10F8109A)&&(RxMessage.IDE == CAN_ID_EXT))
		{ 
			if(canupdateflag1==200)
			 {
				 canupdateflag1=0;
			 }
				canupdateflag1++;
			for ( i=0;i<8;i++)
			{
				canMsg1[i]=RxMessage.Data[i];
			}
			CanComOvertimeCnt=0;   //通信超时计数清零
		}  
	 if ((RxMessage.ExtId == 0x10F8108D)&&(RxMessage.IDE == CAN_ID_EXT))
		{
			if(canupdateflag2==200)
			{
			  canupdateflag2=0;
			}
				canupdateflag2++;
			for(i=0;i<8;i++)
			{
			canMsg2[i]=RxMessage.Data[i];
			}
			CanComOvertimeCnt=0;   //通信超时计数清零
		}  
		
}


void CAN2_RX0_IRQHandler(void)
{ 
		uint8_t i=0;
		CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
		if ((RxMessage.ExtId == 0x10F8109A)&&(RxMessage.IDE == CAN_ID_EXT))
		{ 
			if(canupdateflag1==200)
			 {
				 canupdateflag1=0;
			 }
				canupdateflag1++;
			for ( i=0;i<8;i++)
			{
				canMsg1[i]=RxMessage.Data[i];
			}
			CanComOvertimeCnt=0;   //通信超时计数清零
		}  
	 if ((RxMessage.ExtId == 0x10F8108D)&&(RxMessage.IDE == CAN_ID_EXT))
		{
			if(canupdateflag2==200)
			{
			  canupdateflag2=0;
			}
				canupdateflag2++;
			for(i=0;i<8;i++)
			{
			canMsg2[i]=RxMessage.Data[i];
			}
			CanComOvertimeCnt=0;   //通信超时计数清零
		}  
		
}

void TIM5_IRQHandler(void)	 //1s  定时中断
{					   

    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
//		// 状态信息定时发送	   测试
//		SendTimeCnt++;
//		if(SendTimeCnt>9)  //状态信息定时发送
//		{
//	        flag_TIMER_RX=SET; //10s一次			
//			SendTimeCnt=0;
//		}
		// 状态信息定时发送	   实际运行
		SendTimeCnt++;
		if(SendTimeCnt>59)  //状态信息定时发送
		{
	        flag_TIMER_RX=SET; //10s一次			
			SendTimeCnt=0;
		}					  //实际运行
	   // 报警连续发送
		if(warnCounter==1) //报警信息重复发送时间控制  1min
		{
			
			if(WarnTimeCon>59)
			{
				warnCounter=0;
				WarnTimeCon=0;	
			}
			else
				WarnTimeCon++;
		}
		// 运行时间
		RunTimeCnt++;
       // timeCounter++;
	   if(RunTimeCnt>59)   //运行时间定时跟新  1min
	   {	
	   		flag_Running_time=SET; //1min一次
		  
		    RunTimeCnt=0;
		}
		   if(LED_flag==0)
			{
				GPIO_ResetBits(GPIOD , GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
				LED_flag=1;
			}
			else
			{
			  GPIO_SetBits(GPIOD , GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
			  LED_flag=0;
			}
			LED_Cnt=0;
		
    }
}
void TIM4_IRQHandler(void)
{
    /* www.armjishu.com ARM技术论坛 */
    

    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//			  flag_Running_time=SET;

    }
}
void filter(void)
{
   int  sum = 0;
   u8 count,i;
   for(i=0;i<2;i++)
   {
			for ( count=0;count<30;count++)
			{
				sum += AD_Value[count][i];
			}
			anaMsg[i]=sum/30;
			sum=0;
  }
}
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
	{
  filter();
  DMA_ClearITPendingBit(DMA1_IT_TC1);
	}
}

void TIM3_IRQHandler(void)    // 定时器定时2ms,超过4ms认为数据接收完毕
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //
       

		 if(Uart4OverTimeFlag==SET)  
        {
            if(Uart4OverTimeFlagCount<255) 
							Uart4OverTimeFlagCount++;
        }
        if(Uart4OverTimeFlagCount>4) 
        {
            Uart4OverTimeFlag=RESET;
            Uart4OverTimeFlagCount=0;
            Uart4RxOkFlag=SET;  
        }
   
		if(Usart3OverTimeFlag==SET)  
        {
            if(Usart3OverTimeFlagCount<255) 
							Usart3OverTimeFlagCount++;
        }
        if(Usart3OverTimeFlagCount>4) 
        {
            Usart3OverTimeFlag=RESET;
            Usart3OverTimeFlagCount=0;
            Usart3RxOkFlag=SET;  
        }

        if(Usart2OverTimeFlag==SET)  
        {
            if(Usart2OverTimeFlagCount<255) 
							Usart2OverTimeFlagCount++;
        }
        if(Usart2OverTimeFlagCount>4) 
        {
            Usart2OverTimeFlag=RESET;
            Usart2OverTimeFlagCount=0;
            Usart2RxOkFlag=SET;  
        }
        
		if(Usart1OverTimeFlag==SET)  
        {
            if(Usart1OverTimeFlagCount<255) 
							Usart1OverTimeFlagCount++;
        }

        if(Usart1OverTimeFlagCount>4) 
        {
            Usart1OverTimeFlag=RESET;
            Usart1OverTimeFlagCount=0;
            Usart1RxOkFlag=SET;  
        }

        if(CanComOvertimeCnt<7000) //CAN Come Count	  通信超时计数
			CanComOvertimeCnt++;	
//		//报警连续发送   测试	 
//		if(warnCounter==1) //报警信息发送时间控制  10s
//		{
//			
//			if(WarnTimeCon>5000)
//			{
//				warnCounter=0;
//				WarnTimeCon=0;	
//			}
//			else
//				WarnTimeCon++;
//		}

//		if((ScanData.allinfo.colData.anaData[0]>8&&ScanData.allinfo.colData.anaData[0]<13)||(ScanData.allinfo.colData.anaData[0]>94&&ScanData.allinfo.colData.anaData[0]<99))	 //电流传感器断路故障  持续 10 11   120 121
//		{
//			if(CurrentErrorCnt<7000)
//				CurrentErrorCnt++;
//		}
//		else
//		  CurrentErrorCnt=0;

		if(ScanData.allinfo.colData.anaData[0]!=0&&abs(ScanData.allinfo.colData.anaData[0]-CurrentHistoryValue)<=2)	 //电流传感器断路故障  持续 10 11   120 121
		{
			if(CurrentErrorCnt<7000)
				CurrentErrorCnt++;
		}
		else
		{
			CurrentErrorCnt=0;
			CurrentHistoryValue=ScanData.allinfo.colData.anaData[0];
		}


		  

    }
		
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
