#include "Usart1Deal.H"

u8 DebugModFlag=0;//1实时状态   2历史状态    3配置模式
u8 WIFISendFlag=0,GPRSSendFlag=0;  //响应实时刷新标志
u8 DebugSendFlag=0;	//响应实时刷新标志

u8 ComModeSelect=3;	//通信模式 1：WIFI  2：GPRS   3:WIFI+GPRSZ自动  4:WIFI+GPRS同时
u8 ArresterControlEn=1;	//制动器控制使能    0：不控制和检测制动器   1：使能制动器控制和检测

void Usart1SendError(void)
{
  USART1_SendMData("Error",5);
}

void Usart1RxPackgeDeal(unsigned char * UsartBuff,unsigned char dataNum)
{
	if(UsartBuff[0]=='M')//串口模式选择
	{
	 	if(UsartBuff[1]=='?'&&dataNum==2)
		{
		 	USART1_SendMData("M+ok=",5);
			USART1_SendData(DebugModFlag+'0');
		}
		else
		{
			if(UsartBuff[1]=='='&&dataNum==3)
			{
				if((UsartBuff[2]-'0')<4)
				{
					DebugModFlag=UsartBuff[2]-'0';
					USART1_SendMData("M=",2);
			        USART1_SendData(DebugModFlag+'0');
					USART1_SendMData("+ok",3);
				}
			}	 
		}
	}
	else
	if(UsartBuff[0]=='R') //历史状态下  查询历史数据
	{
		if(DebugModFlag==2&&UsartBuff[1]=='='&&dataNum==5)
		{
		 	MessageReadNumberBuf[1]=0;
			MessageReadNumberBuf[1]+=(UsartBuff[2]-'0')*100;
			MessageReadNumberBuf[1]+=(UsartBuff[3]-'0')*10;
			MessageReadNumberBuf[1]+=(UsartBuff[4]-'0')*1;
			if(MessageReadNumberBuf[1])
				MessageReadFlag[1]=1; 			
		}
	}
	else
	if(DebugModFlag==3)	//进入配置模式
	{
	 	if(UsartBuff[0]=='S')
		{
			switch(UsartBuff[1])
			{
				case 'I': if(UsartBuff[2]=='?'&&dataNum==3)
						  {
						      uint8_t i=0;
						  	  USART1_SendMData("SI+ok=",6);
							  for(i=0;i<10;i++)
							  {
							  	  USART1_SendData(deviceinfo.deviceData[i]);
							  }	 	
						  }
						  else
						  if(UsartBuff[2]=='='&&dataNum==13)
						  {
						   	  uint8_t i=0;
							  for(i=0;i<10;i++)
							  {
							  	  
								  deviceinfo.deviceData[i]=UsartBuff[3+i];
							  }
						  	  USART1_SendMData("SI=",3);
							  for(i=0;i<10;i++)
							  {
							  	  USART1_SendData(deviceinfo.deviceData[i]);
							  }	
							  USART1_SendMData("+ok",3);
							   for   (i=0;i<10;i++)
								{
								WIN24C02_write(0x00+i,deviceinfo.deviceData[i]);
									delay_ms(50);
								}
						  }
						   break;

				 case 'C': if(UsartBuff[2]=='?'&&dataNum==3)
						  {
						      
								uint8_t tempchar[2];
								USART1_SendMData("SC+ok=",6);
								uintxTochar(realtime.year,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(realtime.month,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(realtime.day,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(realtime.hour,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(realtime.minute,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(realtime.second,tempchar,2);
								USART1_SendMData(tempchar,2);	 	
						  }
						  else
						  if(UsartBuff[2]=='='&&dataNum==15)
						  {
						   	  uint8_t tempchar[2];							
								settime.second=charTouintx(&UsartBuff[13],2);
							    settime.minute=charTouintx(&UsartBuff[11],2);	
							    settime.hour=charTouintx(&UsartBuff[9],2);		
							    settime.day=charTouintx(&UsartBuff[7],2);		
							    settime.month=charTouintx(&UsartBuff[5],2);		
							    settime.year=charTouintx(&UsartBuff[3],2);	
								Set_DS1302(&settime);
						  	  USART1_SendMData("SC=",3);
							   uintxTochar(settime.year,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(settime.month,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(settime.day,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(settime.hour,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(settime.minute,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(settime.second,tempchar,2);
								USART1_SendMData(tempchar,2);
							  USART1_SendMData("+ok",3);
							   
						  }
						   break;
				 case 'R': if(UsartBuff[2]=='?'&&dataNum==3)
						  {
						      uint8_t tempchar[2];
						  	  USART1_SendMData("SR+ok=",6);
							  uintxTochar(running_time.year,tempchar,2);
							  USART1_SendMData(tempchar,2);
								uintxTochar(running_time.month,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(running_time.day,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(running_time.hour,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(running_time.minute,tempchar,2);
								USART1_SendMData(tempchar,2);	 	
						  }
						  else
						  if(UsartBuff[2]=='='&&dataNum==13)
						  {
						   	  uint8_t tempchar[2];
								running_time.year=charTouintx(&UsartBuff[3],2);
								running_time.month=charTouintx(&UsartBuff[5],2);
								running_time.day=charTouintx(&UsartBuff[7],2);
								running_time.hour=charTouintx(&UsartBuff[9],2);
								running_time.minute=charTouintx(&UsartBuff[11],2);

						  	  USART1_SendMData("SR=",3);
							  uintxTochar(running_time.year,tempchar,2);
							  USART1_SendMData(tempchar,2);
								uintxTochar(running_time.month,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(running_time.day,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(running_time.hour,tempchar,2);
								USART1_SendMData(tempchar,2);
								uintxTochar(running_time.minute,tempchar,2);
								USART1_SendMData(tempchar,2);
							  USART1_SendMData("+ok",3);
							   WIN24C02_write(0x64,running_time.year);
								delay_ms(50);
								WIN24C02_write(0x65,running_time.month);
								delay_ms(50);
								WIN24C02_write(0x66,running_time.day);
								delay_ms(50);
								WIN24C02_write(0x67,running_time.hour);
								delay_ms(50);
								WIN24C02_write(0x68,running_time.minute);
								delay_ms(50);
						  }
						   break;
				 case 'E': if(UsartBuff[2]=='?'&&dataNum==3)
						  {
						      uint8_t tempchar[2];
						  	  USART1_SendMData("SE+ok=",6);
							  uintxTochar(CurrentBoundaryValue,tempchar,2);
							  USART1_SendMData(tempchar,2);
								 	
						  }
						  else
						  if(UsartBuff[2]=='='&&dataNum==5)
						  {
						   	  uint8_t tempchar[2];
								CurrentBoundaryValue=charTouintx(&UsartBuff[3],2);								

						  	  USART1_SendMData("SE=",3);
							  uintxTochar(CurrentBoundaryValue,tempchar,2);
							  USART1_SendMData(tempchar,2);
							
							  USART1_SendMData("+ok",3);
							   WIN24C02_write(50,CurrentBoundaryValue);
								delay_ms(100);
								
						  }
						   break;
				 case 'T': if(UsartBuff[2]=='?'&&dataNum==3)
						  {
						      uint8_t tempchar[2];
						  	  USART1_SendMData("ST+ok=",6);
							  uintxTochar(TemperBoundaryValue,tempchar,2);
							  USART1_SendMData(tempchar,2);
								 	
						  }
						  else
						  if(UsartBuff[2]=='='&&dataNum==5)
						  {
						   	  uint8_t tempchar[2];
								TemperBoundaryValue=charTouintx(&UsartBuff[3],2);								

						  	  USART1_SendMData("ST=",3);
							  uintxTochar(TemperBoundaryValue,tempchar,2);
							  USART1_SendMData(tempchar,2);
							
							  USART1_SendMData("+ok",3);
							   WIN24C02_write(51,TemperBoundaryValue);
								delay_ms(100);
								
						  }
						   break;
				 case 'S':if(UsartBuff[2]=='=')
						  {
						   	  uint32_t tempint=0;
							  uint8_t i=0;
							  if(dataNum>3)
							  {
							  	for(i=3;i<dataNum;i++)
								{
								    tempint*=10;
								 	tempint+=(UsartBuff[i]-'0');
								} 
								ResistorTarget=(float)tempint/10;
								CalibrationCnt=0;
								CalibrationFlag=1;

								USART1_SendMData("SS=",3);								  
							  	USART1_SendMData(&UsartBuff[3],dataNum-3);							 	
							  	USART1_SendMData("+ok",3);							   	
							  }									
						  }
						   break;
				case 'P': if(UsartBuff[2]=='?'&&dataNum==3)
						  {
//						      uint8_t tempchar[2];
						  	  USART1_SendMData("SP+ok=",6);
//							  uintxTochar(TemperBoundaryValue,tempchar,2);
//							  USART1_SendMData(tempchar,2);
							  USART1_SendData(ComModeSelect+'0');
							  USART1_SendData(ArresterControlEn+'0');
								 	
						  }
						  else
						  if(UsartBuff[2]=='='&&dataNum==5)
						  {
//						   	  uint8_t tempchar[2];
//								TemperBoundaryValue=charTouintx(&UsartBuff[3],2);								
							  ComModeSelect=UsartBuff[3]-'0';
							  ArresterControlEn=UsartBuff[4]-'0';

						  	  USART1_SendMData("ST=",3);
//							  uintxTochar(TemperBoundaryValue,tempchar,2);
//							  USART1_SendMData(tempchar,2);
							  USART1_SendData(ComModeSelect+'0');
							  USART1_SendData(ArresterControlEn+'0');
							  USART1_SendMData("+ok",3);
							  WIN24C02_write(20,ComModeSelect);
							  delay_ms(100);
							  WIN24C02_write(21,ArresterControlEn);
							  delay_ms(100);
						  }
						   break;
			}
		}
		else
		if(UsartBuff[0]=='W')
			USART3_SendMData(Usart1RxBuffer+1,Usart1Rxcount-1);
		else
		if(UsartBuff[0]=='G')
			UART4_SendMData(Usart1RxBuffer+1,Usart1Rxcount-1);
	}	

}

//	CurrentBoundaryValue,TemperBoundaryValue;

