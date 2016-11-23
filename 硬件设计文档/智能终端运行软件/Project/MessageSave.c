#include "MessageSave.h"
//Flash数据包存储和读取变量
u32	  StatusMessageStartAddr=StatusMessageSaveStartAddr,StatusMessageEndAddr=StatusMessageSaveStartAddr,StatusMessageNumber=0;  // FLASH数据保存用变量
u32	  StatusReadStartAddr=0,StatusReadEndAddr=0,StatusReadNumber=0;  //读FLASH数据用变量
u8    StatusReadEnFlag=0,StatusReadBuff[DatagramLength];
// 缓存读取数据变量
u8    MessageReadFlag[2];//0 网络缓存数据读取标志   1 单机历史数据读取标志
u32   MessageReadNumberBuf[2];//0 网络缓存数据读取数量   1 单机历史数据读取数量
u8    MessageReadStatus=0; //数据读取状态  0  没有读取数据   1 网络读取缓存数据  2单机读取历史数据


//数据读取前参数设置
void StatusMessageReadInit(void)
{
	
	if(StatusReadNumber>=StatusMessageNumber)	//读取个数小于等于存储个数
	{
		StatusReadNumber=StatusMessageNumber;	//取存储的个数
		StatusReadStartAddr=StatusMessageStartAddr;//获取要读取数据的起始位置
	}
	else
	{
	    u16  secpos=(StatusMessageEndAddr-StatusMessageSaveStartAddr)/4096,//扇区个数 0~511 for w25x16
		     secoff=(StatusMessageEndAddr-StatusMessageSaveStartAddr)%4096,//在扇区内的偏移
			 FormerNum=0,NumRemain=0;//当前地址之前数据个数
        u16  secoffnum=0,remainnun=0,secposnum=0;

	    if(secoff%DatagramLength==0) //算上扇区内个数
		{
		  	FormerNum=secpos*StatusMessageSaveSingleNum+secoff/DatagramLength+1;
			if(StatusReadNumber==FormerNum)
			{
			 	StatusReadStartAddr=StatusMessageSaveStartAddr;
				StatusReadEnFlag=1;// 数据读取使能标志
				return;
			}
			else if(StatusReadNumber>FormerNum)
			{
			    NumRemain=StatusReadNumber-FormerNum;
				StatusReadEndAddr=StatusMessageSaveEndAddr-(4096-DatagramLength*(StatusMessageSaveSingleNum-1)-1);
			}
			else
				StatusReadEndAddr=StatusMessageEndAddr; 						
		}
		else  // 不算扇区内个数
		{
			u16  numoff=StatusMessageNumber%StatusMessageSaveSingleNum,numread=0;
							 
		    numread=StatusMessageNumber-numoff;
			if(StatusReadNumber>=numread)
			{
			  	StatusReadNumber=numread;
				StatusReadStartAddr=StatusMessageStartAddr;
				StatusReadEnFlag=1;// 数据读取使能标志
				return;
			}
			FormerNum=secpos*StatusMessageSaveSingleNum;
			if(FormerNum==0)
			{
			 	FormerNum=numread;				
				StatusReadEndAddr=StatusMessageSaveEndAddr-(4096-DatagramLength*(StatusMessageSaveSingleNum-1)-1);
			} 
			else if(StatusReadNumber==FormerNum)	
			{
			    StatusReadStartAddr=StatusMessageSaveStartAddr;
				StatusReadEnFlag=1;// 数据读取使能标志
				return;
			}
			else if(StatusReadNumber>FormerNum)
			{
				 NumRemain=StatusReadNumber-FormerNum;
				 StatusReadEndAddr=StatusMessageSaveEndAddr-(4096-DatagramLength*(StatusMessageSaveSingleNum-1)-1);
			}
			else
			   	 StatusReadEndAddr=secpos*4096-(4096-DatagramLength*(StatusMessageSaveSingleNum-1));	
		}

		secoffnum= (StatusReadEndAddr-StatusMessageSaveStartAddr)%4096/DatagramLength+1;
		if(NumRemain) //
		{
		    remainnun=NumRemain; 
		}
		else
		{
			remainnun=StatusReadNumber;
		}
		if(remainnun<=secoffnum)//在当前扇区内
		{
			StatusReadStartAddr=StatusReadEndAddr-(remainnun-1)*DatagramLength;
		}
		else //在之前的扇区
		{
		    StatusReadEndAddr=StatusReadEndAddr-(secoffnum-1)*DatagramLength;
			remainnun=remainnun-secoffnum;
			secposnum=remainnun/StatusMessageSaveSingleNum;
			secoffnum=remainnun%StatusMessageSaveSingleNum;
			if(secoffnum)
				StatusReadStartAddr=StatusReadEndAddr-(secposnum+1)*4096+(StatusMessageSaveSingleNum-secoffnum)*DatagramLength;
			else
			    StatusReadStartAddr=StatusReadEndAddr-(secposnum+1)*4096;
		}
	}	
	StatusReadEnFlag=1;// 数据读取使能标志

}
//读取指定数目的数据
void StatusMessageReadFromFlash(void)
{

	//数据读取判断
	if(MessageReadStatus==0)//没有数据读取
	{
		if(MessageReadFlag[0]==1)	//网络缓存数据
		{
		 	if(MessageReadNumberBuf[0])
			{
			 	StatusReadNumber=MessageReadNumberBuf[0];
				MessageReadStatus=1;
			}
			MessageReadFlag[0]=0;
			MessageReadNumberBuf[0]=0;	 		
		}
		else if(MessageReadFlag[1]==1)//单机历史数据
		{
			if(MessageReadNumberBuf[1])
			{
			 	StatusReadNumber=MessageReadNumberBuf[1];
				MessageReadStatus=2;
			}
			MessageReadFlag[1]=0;
			MessageReadNumberBuf[1]=0;
		}

		if(StatusReadNumber) 			
			StatusMessageReadInit();
	}
		
	//读取数据
	if(StatusReadEnFlag)
	{
		
		if(StatusReadNumber)
		{
			 SPI_FLASH_BufferRead(StatusReadBuff, StatusReadStartAddr, DatagramLength);	//读取
			 StatusReadStartAddr=StatusReadStartAddr+DatagramLength;
			 if(StatusReadStartAddr%4096==StatusMessageSaveSingleNum*DatagramLength)
			 {
			 	StatusReadStartAddr=StatusReadStartAddr+(4096-StatusReadStartAddr%4096);
				if(StatusReadStartAddr>StatusMessageSaveEndAddr)
					StatusReadStartAddr=StatusMessageSaveStartAddr;
			 }
			 if(MessageReadStatus==2) //单机历史数据
			 	USART1_SendMData(StatusReadBuff,DatagramLength); //发送
			 if(MessageReadStatus==1) //网络缓存数据
			 	Send_Instruction(StatusReadBuff);
			 StatusReadNumber--;
		}
		else
		{
			StatusReadEnFlag=0;
			MessageReadStatus=0;
		}
	}
}

void StatusMessageSaveToFlash(uint8_t *Message)
{  
    uint8_t sector=0; 
 	if(StatusMessageNumber < StatusMessageSaveTotalNum)
	{
		if(StatusMessageNumber%StatusMessageSaveSingleNum == 0)	//一个扇区记录完成或刚开始记录
		{
		   //擦除扇区 sector=StatusMessageNumber/StatusMessageSaveSingleNum
		   sector=StatusMessageNumber/StatusMessageSaveSingleNum;
		   SPI_FLASH_SectorErase(sector*4096);
		   StatusMessageEndAddr=sector*4096;  // 末尾位置地址  指向擦除扇区的起始地址
		}
		else  //在一个扇区中记录数据
		{ 
	 		StatusMessageEndAddr=StatusMessageEndAddr+DatagramLength; //加上一个数据包的长度
		}
		StatusMessageNumber++;//加上本次记录的数据个数1
	}
	else
	{   
	   //擦除扇区  sector=StatusMessageStartAddr/4096
	   sector=StatusMessageStartAddr/4096;
	   SPI_FLASH_SectorErase(sector*4096);
	   StatusMessageEndAddr=StatusMessageStartAddr;//末尾位置地址  指向起始位置地址
	   StatusMessageStartAddr=StatusMessageStartAddr+4096;//起始位置地址 向后移一个扇区
	   if(StatusMessageStartAddr>StatusMessageSaveEndAddr)//如果起始位置地址 大于存储范围  则指向存储范围的起始地址
		   StatusMessageStartAddr=StatusMessageSaveStartAddr;
	   StatusMessageNumber=StatusMessageNumber-StatusMessageSaveSingleNum; //减去一个扇区的数据
	   StatusMessageNumber++;//加上本次记录的数据个数1	
	   	  		
	}

	SPI_FLASH_BufferWrite(Message, StatusMessageEndAddr,DatagramLength);
	SaveFlashpara();
}
//数据存储
//130~141
void SaveFlashpara(void)
{
 	//StatusMessageStartAddr=StatusMessageSaveStartAddr,StatusMessageEndAddr=StatusMessageSaveStartAddr,StatusMessageNumber=0;  // FLASH数据保存用变量
	u32 tempdata[3];
	u8 i=0,j=0,SaveAddr=130;
	u8 cnt=0,temp[12];	  
	tempdata[0]=StatusMessageStartAddr;
	tempdata[1]=StatusMessageEndAddr;
	tempdata[2]=StatusMessageNumber;  
	for(i=0;i<3;i++)
	{
		for(j=0;j<4;j++)
		{
			temp[cnt] = tempdata[i]&0xFF;		 		
			cnt++;
			tempdata[i]=tempdata[i]>>8;
		}
	} 
	WIN24C02_BufferWrite(temp, SaveAddr,12);
//	SaveAddr=130;
//	WIN24C02_BufferRead(temp, SaveAddr,12);
}

void ReadFlashpara(void)
{
   u8 temp[12];
   u8 SaveAddr=130,i=0,j=0;
   u32 tempdata[3];
   WIN24C02_BufferRead(temp, SaveAddr,12);
   for(i=0;i<3;i++)
   {
   	 tempdata[i]=temp[j++];
	 tempdata[i]|=(temp[j++]<<8);
	 tempdata[i]|=(temp[j++]<<16);
	 tempdata[i]|=(temp[j++]<<24);
   }
	StatusMessageStartAddr=tempdata[0];
	StatusMessageEndAddr=tempdata[1];
	StatusMessageNumber=tempdata[2];
}

