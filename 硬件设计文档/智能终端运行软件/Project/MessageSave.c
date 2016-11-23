#include "MessageSave.h"
//Flash���ݰ��洢�Ͷ�ȡ����
u32	  StatusMessageStartAddr=StatusMessageSaveStartAddr,StatusMessageEndAddr=StatusMessageSaveStartAddr,StatusMessageNumber=0;  // FLASH���ݱ����ñ���
u32	  StatusReadStartAddr=0,StatusReadEndAddr=0,StatusReadNumber=0;  //��FLASH�����ñ���
u8    StatusReadEnFlag=0,StatusReadBuff[DatagramLength];
// �����ȡ���ݱ���
u8    MessageReadFlag[2];//0 ���绺�����ݶ�ȡ��־   1 ������ʷ���ݶ�ȡ��־
u32   MessageReadNumberBuf[2];//0 ���绺�����ݶ�ȡ����   1 ������ʷ���ݶ�ȡ����
u8    MessageReadStatus=0; //���ݶ�ȡ״̬  0  û�ж�ȡ����   1 �����ȡ��������  2������ȡ��ʷ����


//���ݶ�ȡǰ��������
void StatusMessageReadInit(void)
{
	
	if(StatusReadNumber>=StatusMessageNumber)	//��ȡ����С�ڵ��ڴ洢����
	{
		StatusReadNumber=StatusMessageNumber;	//ȡ�洢�ĸ���
		StatusReadStartAddr=StatusMessageStartAddr;//��ȡҪ��ȡ���ݵ���ʼλ��
	}
	else
	{
	    u16  secpos=(StatusMessageEndAddr-StatusMessageSaveStartAddr)/4096,//�������� 0~511 for w25x16
		     secoff=(StatusMessageEndAddr-StatusMessageSaveStartAddr)%4096,//�������ڵ�ƫ��
			 FormerNum=0,NumRemain=0;//��ǰ��ַ֮ǰ���ݸ���
        u16  secoffnum=0,remainnun=0,secposnum=0;

	    if(secoff%DatagramLength==0) //���������ڸ���
		{
		  	FormerNum=secpos*StatusMessageSaveSingleNum+secoff/DatagramLength+1;
			if(StatusReadNumber==FormerNum)
			{
			 	StatusReadStartAddr=StatusMessageSaveStartAddr;
				StatusReadEnFlag=1;// ���ݶ�ȡʹ�ܱ�־
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
		else  // ���������ڸ���
		{
			u16  numoff=StatusMessageNumber%StatusMessageSaveSingleNum,numread=0;
							 
		    numread=StatusMessageNumber-numoff;
			if(StatusReadNumber>=numread)
			{
			  	StatusReadNumber=numread;
				StatusReadStartAddr=StatusMessageStartAddr;
				StatusReadEnFlag=1;// ���ݶ�ȡʹ�ܱ�־
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
				StatusReadEnFlag=1;// ���ݶ�ȡʹ�ܱ�־
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
		if(remainnun<=secoffnum)//�ڵ�ǰ������
		{
			StatusReadStartAddr=StatusReadEndAddr-(remainnun-1)*DatagramLength;
		}
		else //��֮ǰ������
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
	StatusReadEnFlag=1;// ���ݶ�ȡʹ�ܱ�־

}
//��ȡָ����Ŀ������
void StatusMessageReadFromFlash(void)
{

	//���ݶ�ȡ�ж�
	if(MessageReadStatus==0)//û�����ݶ�ȡ
	{
		if(MessageReadFlag[0]==1)	//���绺������
		{
		 	if(MessageReadNumberBuf[0])
			{
			 	StatusReadNumber=MessageReadNumberBuf[0];
				MessageReadStatus=1;
			}
			MessageReadFlag[0]=0;
			MessageReadNumberBuf[0]=0;	 		
		}
		else if(MessageReadFlag[1]==1)//������ʷ����
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
		
	//��ȡ����
	if(StatusReadEnFlag)
	{
		
		if(StatusReadNumber)
		{
			 SPI_FLASH_BufferRead(StatusReadBuff, StatusReadStartAddr, DatagramLength);	//��ȡ
			 StatusReadStartAddr=StatusReadStartAddr+DatagramLength;
			 if(StatusReadStartAddr%4096==StatusMessageSaveSingleNum*DatagramLength)
			 {
			 	StatusReadStartAddr=StatusReadStartAddr+(4096-StatusReadStartAddr%4096);
				if(StatusReadStartAddr>StatusMessageSaveEndAddr)
					StatusReadStartAddr=StatusMessageSaveStartAddr;
			 }
			 if(MessageReadStatus==2) //������ʷ����
			 	USART1_SendMData(StatusReadBuff,DatagramLength); //����
			 if(MessageReadStatus==1) //���绺������
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
		if(StatusMessageNumber%StatusMessageSaveSingleNum == 0)	//һ��������¼��ɻ�տ�ʼ��¼
		{
		   //�������� sector=StatusMessageNumber/StatusMessageSaveSingleNum
		   sector=StatusMessageNumber/StatusMessageSaveSingleNum;
		   SPI_FLASH_SectorErase(sector*4096);
		   StatusMessageEndAddr=sector*4096;  // ĩβλ�õ�ַ  ָ�������������ʼ��ַ
		}
		else  //��һ�������м�¼����
		{ 
	 		StatusMessageEndAddr=StatusMessageEndAddr+DatagramLength; //����һ�����ݰ��ĳ���
		}
		StatusMessageNumber++;//���ϱ��μ�¼�����ݸ���1
	}
	else
	{   
	   //��������  sector=StatusMessageStartAddr/4096
	   sector=StatusMessageStartAddr/4096;
	   SPI_FLASH_SectorErase(sector*4096);
	   StatusMessageEndAddr=StatusMessageStartAddr;//ĩβλ�õ�ַ  ָ����ʼλ�õ�ַ
	   StatusMessageStartAddr=StatusMessageStartAddr+4096;//��ʼλ�õ�ַ �����һ������
	   if(StatusMessageStartAddr>StatusMessageSaveEndAddr)//�����ʼλ�õ�ַ ���ڴ洢��Χ  ��ָ��洢��Χ����ʼ��ַ
		   StatusMessageStartAddr=StatusMessageSaveStartAddr;
	   StatusMessageNumber=StatusMessageNumber-StatusMessageSaveSingleNum; //��ȥһ������������
	   StatusMessageNumber++;//���ϱ��μ�¼�����ݸ���1	
	   	  		
	}

	SPI_FLASH_BufferWrite(Message, StatusMessageEndAddr,DatagramLength);
	SaveFlashpara();
}
//���ݴ洢
//130~141
void SaveFlashpara(void)
{
 	//StatusMessageStartAddr=StatusMessageSaveStartAddr,StatusMessageEndAddr=StatusMessageSaveStartAddr,StatusMessageNumber=0;  // FLASH���ݱ����ñ���
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

