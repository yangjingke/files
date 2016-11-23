#include "Sampling.H"
#include "WIN_24C02.h"
extern ALLKINDSTRUCT ScanData;

uint8_t IO_OUT=0;//制动器开关检测  不用了 
//通信变量
uint8_t linkstatu[3];//网络连接状态 
uint8_t LinkStatuOnlineFlag=0; //网络连接在线标志  0:离线  1：WIFI在线 2：GPRS在线 3：同时在线
union int32TOCHAR  	MessageCacheCnt={0};  //离线状态下  数据包计数
uint32_t   LinkTimeCnt=0,LinkTimeCntLimit=5000;


uint8_t Switch[4];
uint8_t DigDataBuffer[6]; \
uint8_t DigDataCurrent[6];
uint8_t ActionCnt[6];
uint8_t ActionStatus[6];
uint8_t RecordFlag=0;
//开关动作次数清零
void ActionNumberClear(void)
{
   uint8_t i=0;
   for(i=0;i<6;i++)
   {		   	  
	   ActionCnt[i]=0; 	 
   }
}
//记录开关动作次数
void RecordActionNumber(void)
{
	if(RecordFlag==0)
	{
	   GetDigDataBuffer();
	   RecordFlag=1;
	}
	else
	{
		GetDigDataCurrent();
		CompareDigData();
		GetDigDataBuffer();
	}
}
 //存为历史开关量
void GetDigDataBuffer(void)
{
   DigDataBuffer[0]=ScanData.allinfo.colData.digData[1];
   DigDataBuffer[1]=ScanData.allinfo.colData.digData[3];
   DigDataBuffer[2]=ScanData.allinfo.colData.digData[5];
   DigDataBuffer[3]=ScanData.allinfo.colData.digData[7];
   DigDataBuffer[4]=ScanData.allinfo.colData.digData[10];
   DigDataBuffer[5]=ScanData.allinfo.colData.digData[31];
}
//获取当前开关量
void GetDigDataCurrent(void)
{
   DigDataCurrent[0]=ScanData.allinfo.colData.digData[1];
   DigDataCurrent[1]=ScanData.allinfo.colData.digData[3];
   DigDataCurrent[2]=ScanData.allinfo.colData.digData[5];
   DigDataCurrent[3]=ScanData.allinfo.colData.digData[7];
   DigDataCurrent[4]=ScanData.allinfo.colData.digData[10];
   DigDataCurrent[5]=ScanData.allinfo.colData.digData[31];
}
//统计开关动作次数
void CompareDigData(void)
{
   uint8_t i=0;
   for(i=0;i<6;i++)
   {
   	   if(i>3)
	   {
	   		if(DigDataCurrent[i]==0&&DigDataBuffer[i]==1)
		   	 ActionCnt[i]++;
	   }
	   else
	   {
	   	   if(DigDataCurrent[i]==1&&DigDataBuffer[i]==0)
		   	 ActionCnt[i]++; 
	   }
	   if(DigDataCurrent[i])
	   {
	   	    ActionStatus[i]=ActionCnt[i]|0x80;
	   }
	   else
	   {
	   		ActionStatus[i]=ActionCnt[i]&0x7f;
	   }

   }
}


//读取拨码开关量
void ReadSwitch(void)
{
  uint8_t TempSW=0;
  TempSW=Read_SW();
  Switch[0]=TempSW&0x0004?1:0;
  Switch[1]=TempSW&0x0008?1:0;
  Switch[2]=TempSW&0x0010?1:0;
  Switch[3]=TempSW&0x0020?1:0;
}


float const RTD_TAB_PT100[211] =   // 表格是以5度为一步，即-200, -195, - 190.....
{
18.52,20.68,22.83,24.97,27.10,29.22,31.34,33.44,35.54,37.64,        // -200 ~ -155   10
39.72,41.80,43.88,45.94,48.00,50.06,52.11,54.15,56.19,58.23,           // -150 ~ -105
60.26,62.28,64.30,66.31,68.33,70.33,72.33,74.33,76.33,78.32,        // -100 ~ -55
80.31,82.29,84.27,86.25,88.22,90.19,92.16,94.12,96.09,98.04,        //        -50 ~ -5

100.00,101.95,103.90,105.85,107.79,109.73,111.67,113.61,115.54,117.47,         // 0   ~ 45
119.40,121.32,123.24,125.16,127.08,128.99,130.90,132.80,134.71,136.61,        // 50  ~ 95
138.51,140.40,142.29,144.18,146.07,147.95,149.83,151.71,153.58,155.46,        // 100 ~ 145
157.33,159.19,161.05,162.91,164.77,166.63,168.48,170.33,172.17,174.02,        // 150 ~ 195
175.86,177.69,179.53,181.36,183.19,185.01,186.84,188.66,190.47,192.29,        // 200 ~ 245
194.10,195.91,197.71,199.51,201.31,203.11,204.90,206.70,208.48,210.27,        // 250 ~ 295
212.05,213.83,215.61,217.38,219.15,220.92,222.68,224.45,226.21,227.96,        // 300 ~ 345
229.72,231.47,233.21,234.96,236.70,238.44,240.18,241.91,243.64,245.37,        // 350 ~ 395
247.09,248.81,250.53,252.25,253.96,255.67,257.38,259.08,260.78,262.48,        // 400 ~ 445
264.18,265.87,267.56,269.25,270.93,272.61,274.29,275.97,277.64,279.31,        // 450 ~ 495
280.98,282.64,284.30,285.96,287.62,289.27,290.92,292.56,294.21,295.85,        // 500 ~ 545
297.49,299.12,300.75,302.38,304.01,305.63,307.25,308.87,310.49,312.10,        // 550 ~ 595
313.71,315.31,316.92,318.52,320.12,321.71,323.30,324.89,326.48,328.06,        // 600 ~ 645
329.64,331.22,332.79,334.36,335.93,337.50,339.06,340.62,342.18,343.73,        // 650 ~ 695
345.28,346.83,348.38,349.92,351.46,353.00,354.53,356.06,357.59,359.12,        // 700 ~ 745
360.64,362.16,363.67,365.19,366.70,368.21,369.71,371.21,372.71,374.21,        // 750 ~ 795
375.70,377.19,378.68,380.17,381.65,383.13,384.60,386.08,387.55,389.02,        // 800 ~ 845
390.48        // 850
};

/***********************************************************************
*FunName:        float CalculateTemperature(float fR)
*
*In:                fR -> PT100的电阻值。
*                        
*Out:                fTem -> 测得的温度值。               
*
*Discription: 将电阻值查表算出温度值。
*
*Notes:         采用2分查找法。          
*
************************************************************************/
float CalculateTemperature(float fR)
{
        float fTem;
        float fLowRValue;
        float fHighRValue;        
        int   iTem;
        u8 i;

//        u8 cLimite = 0xFF;

        u8 cBottom, cTop;

        if (fR < RTD_TAB_PT100[0])                // 电阻值小于表格最小值，低于量程下限。
        {
//                return BELOW_LOW_LIMITE;
        }

        if (fR > RTD_TAB_PT100[210])        // 电阻值大于表格最大值，超出量程上限。
        {
//                return EXCESS_UP_LIMITE;
        }

        cBottom = 0; 
        cTop    = 210;

        for (i=105; (cTop-cBottom)!=1; )        // 2分法查表。
        {
                if (fR < RTD_TAB_PT100[i])
                {
                        cTop = i;
                        i = (cTop + cBottom) / 2;
                }
                else if (fR > RTD_TAB_PT100[i])
                {
                        cBottom = i;
                        i = (cTop + cBottom) / 2;
                }
                else
                {
                        iTem = (u8)i * 5 - 200;
                        fTem = (float)iTem;

                        return fTem;
                }
        }

        iTem = (u8)i * 5 - 200;

        fLowRValue  = RTD_TAB_PT100[cBottom];
        fHighRValue = RTD_TAB_PT100[cTop];

        fTem = ( ((fR - fLowRValue)*5) / (fHighRValue - fLowRValue) ) + iTem;        // 表格是以5度为一步的。
                                                                                                                                                        // 两点内插进行运算。

        return fTem;
}


/***********************************************************************
*FunName:        TemperatureMeasureCalibration(float fR)
*
*In:                fR -> PT100的电阻值。     
*
*Discription: 电阻值测量标定
*
*Notes:       根据标准电阻，取平均值求比例          
*
************************************************************************/
union FTOCHAR ResistorCoefficient={93.043};
float ResistorTemp[10];
float ResistorTarget=99.7;
u8  CalibrationFlag=0,CalibrationCnt=0;
void TemperatureMeasureCalibration(float fR)
{
	float temp=0,Coefficient=0;
	u8 i=0;
	if(CalibrationFlag==1)
	{
		if(ResistorCoefficient.fdat==0)
			ResistorCoefficient.fdat=93.043;
		else
		{
			ResistorTemp[CalibrationCnt]=fR;
			CalibrationCnt++;
			if(CalibrationCnt==10)
			{
				temp=0;
				for(i=0;i<10;i++)
				{
				 	temp=temp+ResistorTemp[i];
				}
				temp=temp/10;
				Coefficient=ResistorTarget/temp;
				ResistorCoefficient.fdat=ResistorCoefficient.fdat*Coefficient;
				if(ResistorCoefficient.fdat==0)
					ResistorCoefficient.fdat=93.043;	
				SaveResistorCoefficientPara();	//保存数据
				CalibrationCnt=0;
				CalibrationFlag=0;
			    
				
			}
		}
	}
}

//数据存储
//150~153
void SaveResistorCoefficientPara(void)
{
 	
	u8 i=0,SaveAddr=150;
	u8 temp[4];	  
	 
	for(i=0;i<4;i++)
	{	  		
			temp[i] = ResistorCoefficient.chdat[i];	
	} 
	WIN24C02_BufferWrite(temp, SaveAddr,4);
//	SaveAddr=130;
//	WIN24C02_BufferRead(temp, SaveAddr,12);
}
//数据存储
//150~153
void ReadResistorCoefficientPara(void)
{
 	//StatusMessageStartAddr=StatusMessageSaveStartAddr,StatusMessageEndAddr=StatusMessageSaveStartAddr,StatusMessageNumber=0;  // FLASH数据保存用变量
	
	u8 i=0,SaveAddr=150;
	u8 temp[4];	  
	WIN24C02_BufferRead(temp, SaveAddr,4); 
	for(i=0;i<4;i++)
	{	  		
		ResistorCoefficient.chdat[i] = temp[i];	
	} 
//	WIN24C02_BufferWrite(temp, SaveAddr,4);
//	SaveAddr=130;
//	WIN24C02_BufferRead(temp, SaveAddr,12);
}
/***********************************************************************
*FunName:        void ComunitationLinkTest(void)
*	  
*Discription: 通信连接检测
*
*Notes:       检测连接服务器的情况，用于自动选择通信方式          
*
************************************************************************/
void ComunitationLinkTest(void)
{
	linkstatu[0]=GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6); //高电平有效
	linkstatu[1]=GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);//高电平有效
	linkstatu[2]=GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10);	//低电平有效
	switch(ComModeSelect)
	{
		case 1:if(linkstatu[2]==0)
			   {
			   		if(LinkTimeCnt==LinkTimeCntLimit)
						LinkStatuOnlineFlag=1;	 //WIFI在线
					else
						LinkTimeCnt++;
			   }				 
			   else
			   {
				   	LinkStatuOnlineFlag=0;
					LinkTimeCnt=0;
			   }
			   	 
			   break;
		case 2:if(linkstatu[0]==1)
			   {
					if(LinkTimeCnt==LinkTimeCntLimit)
						LinkStatuOnlineFlag=2;	 //GPRS在线
					else
						LinkTimeCnt++;
			   } 				 
			   else
			   {
			   		LinkStatuOnlineFlag=0;
					LinkTimeCnt=0;
			   }
			   break;
		case 3:
		case 4: if(linkstatu[0]==1&&linkstatu[2]==0)
				{
				 	if(LinkTimeCnt==LinkTimeCntLimit)
						LinkStatuOnlineFlag=3;	 //同时在线
					else
						LinkTimeCnt++;
				} 					
				else if(linkstatu[0]==1&&linkstatu[2]==1)
					 {
					 	if(LinkTimeCnt==LinkTimeCntLimit)
							LinkStatuOnlineFlag=2;	  //GPRS在线
						else
							LinkTimeCnt++;	
					 }
					else if(linkstatu[0]==0&&linkstatu[2]==0)
						 {
						 	if(LinkTimeCnt==LinkTimeCntLimit)
								LinkStatuOnlineFlag=1;	  //WIFI在线
							else
							   LinkTimeCnt++;
						 }	
						 else
						 {
							LinkStatuOnlineFlag=0;	//离线
							LinkTimeCnt=0;
					 	 }
			   break;	   		

	}
	//如果有缓存需要发送
	if(LinkStatuOnlineFlag&&MessageCacheCnt.intdat)
	{
//	 	StatusReadNumber=MessageCacheCnt.intdat;
//		StatusMessageReadInit();
		MessageReadFlag[0]=1;
		MessageReadNumberBuf[0]=MessageCacheCnt.intdat;
		changeMessageCacheCnt(0);
	}
	
}
 
/***********************************************************************
*FunName:        void changeMessageCacheCnt(u8 type)
*
*In:                type -> 改变形式   0 清零  1 加一  
*	  
*Discription: 改变数据缓存计数
*
*Notes:       网络离线时记录缓存数据的个数          
*
************************************************************************/

void changeMessageCacheCnt(u8 type)
{
	u8 i=0,SaveAddr=30;
	u8 temp[4];	  
	switch(type)
	{
		case 0:
				 MessageCacheCnt.intdat=0;
				 break;
		case 1:
				 MessageCacheCnt.intdat++;
				 break;
	}
	 
	for(i=0;i<4;i++)
	{	  		
			temp[i] = MessageCacheCnt.chdat[i];	
	} 
	WIN24C02_BufferWrite(temp, SaveAddr,4);	
}
/***********************************************************************
*FunName:        void ReadMessageCacheCnt(void)
*
*Discription:    读取数据缓存计数
*
*Notes:       开机读取 数据缓存计数         
*
************************************************************************/
void ReadMessageCacheCnt(void)
{
	u8 i=0,SaveAddr=30;
	u8 temp[4];	  
	WIN24C02_BufferRead(temp, SaveAddr,4); 
	for(i=0;i<4;i++)
	{	  		
		MessageCacheCnt.chdat[i] = temp[i];
	}	
}
