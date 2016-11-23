#include "protocal.h"
static const uint32_t count_base[5]={1,16,256,4096,65536};
static const uint8_t diglenth=17+6;
static const uint8_t analenth=2;
static const uint8_t canlenth=8;
void uint8Tochar(uint8_t ID,uint8_t * ch_single)
{
	if(ID>9)
		*ch_single='a'+ID-10;
	else
		*ch_single='0'+ID;
}

void uintxTochar (uint32_t Value,uint8_t *ch_double,uint8_t dataLenth)
{
	uint8_t shift_i;
	for (shift_i=0;shift_i<=dataLenth-1;shift_i++)
	{
			uint8_t tmp=(uint8_t)(Value%16);
			Value=Value>>4;
			uint8Tochar(tmp,&ch_double[dataLenth-1-shift_i]);
	}
}

uint8_t charToint8(uint8_t  ch_single)
{
		uint8_t ID=0;
		if(ch_single>='a'&&ch_single<='f')
			{
				ID=10+ch_single-'a';
			}
		else if(ch_single>='A'&&ch_single<='F')
			{
				ID=10+ch_single-'A';
			}
		else 
			{
				ID=ch_single-'0';
			}
				return	 ID;	 
}
uint32_t charTouintx (uint8_t * ch_double,uint8_t dataLenth)
{
	uint32_t Value=0;
	uint8_t shift_i=0;
	for(shift_i=0;shift_i<=dataLenth-1;shift_i++)
	 {
		 Value+=charToint8(ch_double[shift_i])*count_base[dataLenth-shift_i-1];
	 }
	return Value;

}

 
void timeinfoTochar(const DATETIME *timeinfo,uint8_t * Tx_timeinfo)
{ 
	uintxTochar(timeinfo->year,Tx_timeinfo+23,2);
	uintxTochar(timeinfo->month,Tx_timeinfo+25,2);
	uintxTochar(timeinfo->day,Tx_timeinfo+27,2);
	uintxTochar(timeinfo->hour,Tx_timeinfo+29,2);
	uintxTochar(timeinfo->minute,Tx_timeinfo+31,2);
}
void allinfoTochar(const ALLINFO *allinfo,uint8_t * Tx_allinfo)
{ 
	uint8_t shift_i=0;
	uint8_t addr=0;  
	for(shift_i=0;shift_i< 12 ;shift_i++)    //12个24V开关量数据   12
	{
		uint8Tochar(allinfo->colData.digData[shift_i],Tx_allinfo+33+addr);
		addr++;
		if(shift_i==1||shift_i==3||shift_i==5||shift_i==7||shift_i==10)
		{
		  	uint8Tochar(9,Tx_allinfo+33+addr);
			addr++;
		}
	}
	addr=0; 
	for (shift_i=0;shift_i<5;shift_i++)		 //5个接地线开关量数据	 5
	{
		uint8Tochar(allinfo->colData.digData[27+shift_i],Tx_allinfo+33+17+addr);
		addr++;
		if(shift_i==4)
		{
		  	uint8Tochar(9,Tx_allinfo+33+17+addr);
			addr++;
		}

	}
	for(shift_i=0;shift_i< analenth ;shift_i++)	 //	两个模拟量数据    4
	{
		uintxTochar(allinfo->colData.anaData[shift_i],Tx_allinfo+33+diglenth+shift_i*analenth,2);
	
	}
	for(shift_i=0;shift_i< canlenth ;shift_i++)	  //CAN数据1   	2*8   
	{
		uintxTochar(allinfo->canData.canData1[shift_i],Tx_allinfo+33+diglenth+2*analenth+shift_i*2,2);
	}
	uintxTochar(canupdateflag1,Tx_allinfo+33+diglenth+2*analenth+2*canlenth,2);	 //CAN数据1 更新标志
 
	for(shift_i=0;shift_i<canlenth ;shift_i++)	 //CAN数据2   	2*8 
	{
		uintxTochar(allinfo->canData.canData2[shift_i],Tx_allinfo+33+diglenth+2*analenth+2*canlenth+2+shift_i*2,2);
	}
	uintxTochar(canupdateflag2,Tx_allinfo+33+diglenth+2*analenth+2*canlenth+2+2*canlenth,2);   //CAN数据2 更新标志
}
void charTodevicinfo(DEVICEINFO *deif,uint8_t *Rx_devinfo)
{
  uint8_t i=0;
	for(i=0;i<10;i++)
	{
		deif->deviceData[i]=charTouintx(&Rx_devinfo[23+i],1);                	
	}
}
void charTotimeinfo(DATETIME *tmif,uint8_t *Rx_devinfo)
{
	tmif->year=charTouintx(&Rx_devinfo[23],2);
	tmif->month=charTouintx(&Rx_devinfo[25],2);
	tmif->day=charTouintx(&Rx_devinfo[27],2);
  tmif->hour=charTouintx(&Rx_devinfo[29],2);
  tmif->minute=charTouintx(&Rx_devinfo[31],2);
}


uint8_t* creatPtrTotrans(uint32_t dest,uint32_t source,DEVICEINFO *dev,uint16_t FC,DATETIME *runtime,ALLKINDSTRUCT *allkindstruct,uint8_t ErrorData)
{  
	 uint8_t i=0; 
	 uint16_t checksum=0;
	 uint16_t dataLenth=0;
	 uint8_t shift_i=0;
	 uint8_t *ptrTotrans=NULL;
	 switch(FC)
	 {
		 case FC_DAT:dataLenth=67+18;
						while(ptrTotrans==NULL)
						{
							ptrTotrans=(uint8_t*)malloc((dataLenth+PROTO_SUM_TAILandHEAD)*sizeof(uint8_t));
						}
						timeinfoTochar(runtime,ptrTotrans);
						allinfoTochar(&allkindstruct->allinfo,ptrTotrans);
						break;
		 case FC_WNG:dataLenth=67+18;
						while(ptrTotrans==NULL)
						{
							ptrTotrans=(uint8_t*)malloc((dataLenth+PROTO_SUM_TAILandHEAD)*sizeof(uint8_t));
						}
						timeinfoTochar(runtime,ptrTotrans);
						allinfoTochar(&allkindstruct->allinfo,ptrTotrans);
						break;
			case FC_RWF:dataLenth=67+18;
						while(ptrTotrans==NULL)
						{
							ptrTotrans=(uint8_t*)malloc((dataLenth+PROTO_SUM_TAILandHEAD)*sizeof(uint8_t));
						}
						timeinfoTochar(runtime,ptrTotrans);
						allinfoTochar(&allkindstruct->allinfo,ptrTotrans);
						break;
//		 case FC_GTM:dataLenth=10;
//						while(ptrTotrans==NULL)
//						{
//							ptrTotrans=(uint8_t*)malloc((dataLenth+PROTO_SUM_TAILandHEAD)*sizeof(uint8_t));
//						}
//						timeinfoTochar(&allkindstruct->timeinfo,ptrTotrans);
//						break;
		 default:dataLenth=0;
						while(ptrTotrans==NULL)
						{
							ptrTotrans=(uint8_t*)malloc((dataLenth+PROTO_SUM_TAILandHEAD)*sizeof(uint8_t));
						}
						break;
	 }
	    uintxTochar(ErrorData,&ptrTotrans[33+diglenth+2*analenth+3*2],2);  //故障代码
	
		ptrTotrans[0]=PROTO_HEAD;  //加头
		uintxTochar(dest,&ptrTotrans[1],4); //地址
		uintxTochar(source,&ptrTotrans[5],4); //地址

		for (i=0;i<10;i++)	//叉车ID
		{
//			uintxTochar(dev->deviceData[i],&ptrTotrans[9+i],1);
			ptrTotrans[9+i]=dev->deviceData[i];
		}
		uintxTochar(FC,&ptrTotrans[19],2);//功能码
		
		
		uintxTochar(dataLenth,&ptrTotrans[21],2); //数据长度
		
		uintxTochar(realtime.year,&ptrTotrans[dataLenth+23-2*6],2);
		uintxTochar(realtime.month,&ptrTotrans[dataLenth+23-2*5],2);
		uintxTochar(realtime.day,&ptrTotrans[dataLenth+23-2*4],2);
		uintxTochar(realtime.hour,&ptrTotrans[dataLenth+23-2*3],2);
		uintxTochar(realtime.minute,&ptrTotrans[dataLenth+23-2*2],2);
		uintxTochar(realtime.second,&ptrTotrans[dataLenth+23-2*1],2); 

		//开关次数
		uintxTochar(ActionStatus[0],&ptrTotrans[23+10+2-1],2);
		uintxTochar(ActionStatus[1],&ptrTotrans[23+10+5-1],2);
		uintxTochar(ActionStatus[2],&ptrTotrans[23+10+8-1],2);
		uintxTochar(ActionStatus[3],&ptrTotrans[23+10+11-1],2);
		uintxTochar(ActionStatus[4],&ptrTotrans[23+10+15-1],2);
		uintxTochar(ActionStatus[5],&ptrTotrans[23+10+22-1],2);

		for(shift_i=23;shift_i<dataLenth+23;shift_i++)//计算校验和
		{
			checksum^=ptrTotrans[shift_i];
		}

		uintxTochar(checksum,&ptrTotrans[dataLenth+23],2); //校验和
		ptrTotrans[dataLenth+25]=PROTO_TAIL;  //加尾
		return ptrTotrans;
};
void extractHEADandTAIL(HEADandTAIL *hdtl,uint8_t *char_rx)
{
	uint8_t i=0;
	hdtl->CheckSum=0;
	hdtl->DataLenth=charTouintx(char_rx+21,2);
	for (i=23;i<23+hdtl->DataLenth;i++)
	{
		hdtl->CheckSum^=char_rx[i];
	}
	hdtl->CheckSumRx=charTouintx(char_rx+23+hdtl->DataLenth,2);
	
}
void extractFCandAdrandId(HEADandTAIL *HdTl,DEVICEINFO * DeIfo ,uint8_t *char_rx)
{ 
	uint8_t i=0;
	HdTl->FC=charTouintx(char_rx+19,2);
	HdTl->Dest=charTouintx(char_rx+1,4);
	HdTl->Source=charTouintx(char_rx+5,4);
	for(i=0;i<10;i++)
	{
//	 DeIfo->deviceData[i]=charTouintx(char_rx+9+i,1);
     DeIfo->deviceData[i]=char_rx[9+i];
	}
	
}
FlagStatus isdevinforight(DEVICEINFO * DeIfo1,DEVICEINFO * DeIfo2)
{ 
	uint8_t i=0;
	for(i=0;i<10;i++)
	{
			if (DeIfo1->deviceData[i]!=(DeIfo2->deviceData[i]))
			return RESET;
	}
	return SET;
	
}

DEVICEINFO Alldevinfo={'0','0','0','0','0','0','0','0','0','0'};
uint16_t parseChar_ptr(uint8_t *char_rx,ALLKINDSTRUCT *allkindstruct)
{ 
	DEVICEINFO tmpdevinfo;
	
	HEADandTAIL tmpheadtail;
	extractFCandAdrandId(&tmpheadtail,&tmpdevinfo,char_rx);
	extractHEADandTAIL(&tmpheadtail,char_rx);
	if(tmpheadtail.CheckSumRx==tmpheadtail.CheckSum)
	{ 
		if(isdevinforight(&tmpdevinfo,&deviceinfo)||isdevinforight(&tmpdevinfo,&Alldevinfo))
		{
				switch(tmpheadtail.FC)
				{
		     case FC_SDI:
//							charTodevicinfo(&allkindstruct->devinfo,char_rx);
							return FC_SDI;
//							break;
	       case FC_STM:
//							charTotimeinfo(&allkindstruct->timeinfo,char_rx);
							return FC_STM;
//							break;		
					case FC_GSI:
							return FC_GSI;
//							break;
					case FC_SCT:
					        return FC_SCT; 
					default:
							return 0xac;  //功能码错误
//							break;
	 
			}
		}
		else	
		{
		return 0xbb;//ID 错误
		}
	}
	else
	{
	 return 	0xca; //校验和错误
	}
}


void releaseChar_ptr (uint8_t *char_ptr)
{
	if(char_ptr)
	{
		free(char_ptr);
		char_ptr=NULL;
	}
	
};

u8 timesynflag=0;
u8 timefromflag=0;//1 WIFI   2 GPRS
/*   应答函数*/
uint8_t ProtocolReply(uint8_t *Rx_info)
{ 
	uint16_t recieveFC=0; //接收到的功能码
	uint8_t* Tx_info=NULL;
	uint8_t data=0;
	ALLKINDSTRUCT unionstruct;	 //所有参数
	recieveFC=parseChar_ptr(Rx_info,&unionstruct);
	switch(recieveFC)
	{
	
		case FC_GSI:
			//sendDeviceInfo(&unionstruct);
			data=1;
			break;
		case FC_SCT:
					if(timesynflag==0||(timesynflag==1&&timefromflag==1))
					{
						settime.second=charTouintx(&Rx_info[33],2);
					    settime.minute=charTouintx(&Rx_info[31],2);	
					    settime.hour=charTouintx(&Rx_info[29],2);		
					    settime.day=charTouintx(&Rx_info[27],2);		
					    settime.month=charTouintx(&Rx_info[25],2);		
					    settime.year=charTouintx(&Rx_info[23],2);	
						Set_DS1302(&settime);
						if(timefromflag==1)
						    timesynflag=2;
						else
							timesynflag=1;
						timefromflag=0;
					}
					break;
	
	}
	releaseChar_ptr(Tx_info);
	return data;
}

