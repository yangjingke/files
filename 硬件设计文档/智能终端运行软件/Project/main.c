#include "SystemBody.h"
#include "string.h"
//#include "i2c_ee.h"
#include "WIN_24C02.h"
#include "SZ_SPI_FLASH.h"
#include "USART.h"
#include "Sampling.H"
#include "MessageSave.h"
#include "ds1302.h"
#include "Usart1Deal.H"

#define DEBUG

uint8_t canupdateflag1=0;
uint8_t canupdateflag2=0;

//uint8_t RX_Data_temp[RxC]={0};  //receive buffer 
//uint8_t RxCounter=0;  //receive buffer counter
//FlagStatus flag_Finish_RX=RESET;//uasrt finish flah
DATETIME running_time; //running time
FlagStatus flag_Running_time=RESET;  //running time +1 min flag
u16 AD_Value[30][2]={0};   //ad value before fitler
u16 anaMsg[2]={0}; // ad value after fitler
FlagStatus flag_Fault=RESET; //warnning flag
FlagStatus flag_TIMER_RX=RESET; // time for scan and send flag
FlagStatus fromWifi=RESET;  //wifi=SET or gprs=RESET
FlagStatus beginRecv=RESET; //begin recv buffer  because the gprs or wifi will send sth.to usart
FlagStatus flag_Sendontime=RESET; // time for record the running time flag
uint32_t DESTNATION=0;  
uint32_t DEVICEADD=0x01;
uint8_t canMsg1[8]={0}; //can msg 1
uint8_t canMsg2[8]={0}; //can msg 2
ALLKINDSTRUCT ScanData; // the object for saving update of can and anolog and digital data
uint8_t timeCounter = 0;  //开机发送计数	start flag ,if stm32 starts,send info one time; 
uint8_t warnCounter=0;  //故障报警次数  count the (warning sending) times
uint8_t warnResetCounter=3; //故障恢复发送计数 count the warnReset  sending times
DEVICEINFO deviceinfo; //device id

 CanTxMsg TxMessage;
uint8_t LED_flag=0;

float RT=0,Temper=0;


uint8_t RelayFlag=0;


//u32 watch[8];
int main(void)
{ 
  //CLK_Config();//时钟配置





	SPI_FLASH_Init();//SPI flash config
//	//I2C_EE_Init();  
	AT24C02_Init();  //eeprom config
	delay_ms(1000);  
	TIM4_Init();  //tim4 init  1min  for record the running time
	FirstSaveSystemInfo();

//	WIN24C02_write(20,ComModeSelect);
//	delay_ms(100);
//	WIN24C02_write(21,ArresterControlEn);		  def
//	delay_ms(100);
//	ComModeSelect=WIN24C02_read(20);	
//	ArresterControlEn=WIN24C02_read(21);

	getTimeandDeviceInfo();  //read runningtime and device id from eeprom;
	NVIC_Configuration();//中断配置 nvic config
	GPIO_Config();//GPIO配置 digital config
    USART_Config();//串口配置 usart config
	ADC_Config();//ADC配置 adc config
	DMA_Config();//DMA配置 dma config
	CAN_Config();//CAN配置 can config
    SysTickConfig();
	Delay((uint32_t)0xFFFF);
	TIM5_Init();  //初始化定时器  tim5 init 1s   for record the time for send data 
	TIM3_Int_Init(19,7199);  //10kHZ的计数频率，计数到20用时2ms。
//	SaveFlashpara();
	ReadFlashpara();
//	SaveResistorCoefficientPara();
	ReadResistorCoefficientPara();
//	changeMessageCacheCnt(0);//缓存数据计数清零并保持  用于第一次调试
	ReadMessageCacheCnt();




//    Set_DS1302(&settime);
	RELAY_OFF;	delay_ms(1);
	#ifdef DEBUG 
	LED_Init();  //led init 
//Send_Instruction("fdsf$");
//  GPIOD->BRR= GPIO_Pin_2;
     
	Delay(5000000);
	#endif
//	watch[0]=FlashSaveStartAddr;
//	watch[1]=FlashSaveEndAddr;
//	watch[2]=DatagramLength;
//	watch[3]=StatusMessageSaveStartAddr;
//	watch[4]=StatusMessageSaveEndAddr;
//	watch[5]=StatusMessageSaveSectorNum;
//	watch[6]=StatusMessageSaveSingleNum;
//	watch[7]=StatusMessageSaveTotalNum;

	while (1)
	{ 
		
		Get_DS1302(&realtime);
		if(flag_Running_time==SET)   //是否1分钟 IS 1 MIN
		{	 
				recordTime();      //  update the running time in eeprom and spiflash
				flag_Running_time=RESET;
				
		}	
		#ifdef DEBUG 
	//	USART2_SendData(ScanData.allinfo.colData.anaData[1]*100/1000);
	//USART2_SendData(ScanData.allinfo.colData.anaData[1]*100/100%10);
	//USART2_SendData(ScanData.allinfo.colData.anaData[1]*100/10%10);
	//	USART2_SendData(ScanData.allinfo.colData.anaData[1]*100%10);
//Send_Instruction(anaMsg[0]*100/0x1000*33/1000,anaMsg[0]*100/0x1000*33%1000/100,anaMsg[0]*100/0x1000*33%100/10);
  
	#endif
//		Scan_Current_Info();     //扫描 scan_info
//  	GPIOD->ODR^= GPIO_Pin_7;
	
//		Delay(5000000);

		ScanData.allinfo.colData.anaData[0]=abs(250-2-anaMsg[1]*330/4096);//anaMsg[0]*3.3/0x1000;//    电流  //待实际处理
		if(ScanData.allinfo.colData.anaData[0]<4) ScanData.allinfo.colData.anaData[0]=0;
//	   ScanData.allinfo.colData.anaData[1]=(anaMsg[0]*3300/4095+40-400)/16;//anaMsg[1]*(3.3/4095);	  温度
//		 RT=98.182*anaMsg[0]*3300/4095/1000;   //公式计算值
//		 RT=95.263*anaMsg[0]*3300/4095/1000;   //生产版1.0值
//		RT=93.043*anaMsg[0]*3300/4095/1000;		//生产版2.0修正值 0
//		RT=94.936*anaMsg[0]*3300/4095/1000;		//生产版2.0修正值 1 
//RT=98.478*anaMsg[0]*3300/4095/1000;		//生产版2.0修正值 2
RT=ResistorCoefficient.fdat*anaMsg[0]*3300/4095/1000;		//生产版2.0修正值 5
	TemperatureMeasureCalibration(RT);
		 Temper=CalculateTemperature(RT);
		 TemperErrorValue=(u16)Temper;
		 ScanData.allinfo.colData.anaData[1]=(u8)Temper;
		ReadSwitch();//读拨码开关
		ReadHC165Data();//获取32个开关量信号
		RecordActionNumber();//获取动作次数
		IO_OUT=Read_IO();
//		linkstatu[0]=GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
//		linkstatu[1]=GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
//		linkstatu[2]=GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10);
        if(ArresterControlEn==1)
		{
			if(ScanData.allinfo.colData.digData[31]==1)
			{	
				RELAY_OFF;	delay_ms(1);  
				if(RelayFlag==0)
				{
					if(ScanData.allinfo.colData.digData[15]==0)
					{
						ArresterErrorFlag=1;
					}
					else
						ArresterErrorFlag=0;
				}
				else			
					RelayFlag--;
			 }
			else
			{	RELAY_ON; delay_ms(1);  RelayFlag=3; }
		}

		ComunitationLinkTest();//网络状态检测

		Scan_Current_Info();     //扫描 scan_info 并发送状态信息

		if(Usart2RxOkFlag==SET)  // GPS
		{  
				Usart2RxOkFlag=RESET;  //复位 reset
				Usart2Rxcount=0;  // char number reset
//			  if (Usart2RxBuffer[0]=='@')
//				{
//				ProtocolReply(Usart2RxBuffer); 
//				}//yingda  answer the order from server;
		}
		
		if(Usart1RxOkFlag==SET)  // 调试及配置串口
		{  
//				USART1_SendMData(Usart1RxBuffer,Usart1Rxcount);
//				UART4_SendMData(Usart1RxBuffer,Usart1Rxcount);
//				USART3_SendMData(Usart1RxBuffer,Usart1Rxcount);
//				if(Usart1RxBuffer[0]=='R'&&Usart1Rxcount==4)
//				{
//			      StatusReadNumber=0;
//				  StatusReadNumber+=(Usart1RxBuffer[1]-'0')*100;
//				  StatusReadNumber+=(Usart1RxBuffer[2]-'0')*10;
//				  StatusReadNumber+=(Usart1RxBuffer[3]-'0')*1;
//				  if(StatusReadNumber)
//				  	StatusMessageReadInit();
//				
//				}
//				if(Usart1RxBuffer[0]=='T'&&Usart1Rxcount==7)
//				{
//					
//					settime.second=Usart1RxBuffer[1];
//				    settime.minute=Usart1RxBuffer[2];	
//				    settime.hour=Usart1RxBuffer[3];		
//				    settime.day=Usart1RxBuffer[4];		
//				    settime.month=Usart1RxBuffer[5];		
//				    settime.year=Usart1RxBuffer[6];	
//					Set_DS1302(&settime);
//				}
				Usart1RxPackgeDeal(Usart1RxBuffer,Usart1Rxcount);
				if(Usart1RxBuffer[0]=='@')
				{
					if(ProtocolReply(Usart1RxBuffer))
						DebugSendFlag=1;
				}
				Usart1RxOkFlag=RESET;  //复位 reset
				Usart1Rxcount=0;  // char number reset
//			if (Usart1RxBuffer[0]=='@')
//				{
//					ProtocolReply(Usart1RxBuffer);  //yingda  answer the order from server;
//				}
				
		}

		
		if(Usart3RxOkFlag==SET)  // WIFI 
		{  
//				USART1_SendMData(Usart3RxBuffer,Usart3Rxcount);
//				UART4_SendMData(Uart4RxBuffer,Uart4Rxcount);
				
				if(DebugModFlag==3)
					USART1_SendMData(Usart3RxBuffer,Usart3Rxcount);
				else
				{
					
					if(Usart3RxBuffer[0]=='@')
					{
						timefromflag=1;
						if(ProtocolReply(Usart3RxBuffer))
							WIFISendFlag=1;
					}
				}
				
				Usart3RxOkFlag=RESET;  //复位 reset
				Usart3Rxcount=0;  // char number reset	 
				
		}
		
		if(Uart4RxOkFlag==SET)  // GPRS
		{  
//				USART1_SendMData(Uart4RxBuffer,Uart4Rxcount);
//				UART4_SendMData(Uart4RxBuffer,Uart4Rxcount);
			
				if(DebugModFlag==3)
					USART1_SendMData(Uart4RxBuffer,Uart4Rxcount);
				else
				{
					if(Uart4RxBuffer[0]=='@')
					{
						timefromflag=2;
						if(ProtocolReply(Uart4RxBuffer))
						GPRSSendFlag=1;
					}
				}
				Uart4RxOkFlag=RESET;  //复位 reset
				Uart4Rxcount=0;  // char number reset	 
				
		}

		StatusMessageReadFromFlash();//读取Flash中的数据
		SendOnceMessage(); //响应实时刷新数据一次
		
	 }
		 
	
	
	
	
	
}
