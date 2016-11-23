#ifndef _PROTOCAL_H
#define _PROTOCAL_H
	#include "stm32f10x.h"
	#include "stdlib.h"
    #include "ds1302.h"
	#include "Sampling.H"
	

	#define PROTO_HEAD ('@')
	#define PROTO_TAIL 0x24
	#define BYTE u8
	#define PROTO_SUM_TAILandHEAD  (uint8_t)26

	typedef struct DataInfo
	{
		uint8_t digData[32];   //17 digital data
		uint16_t  anaData[2]; // 2 anaData
	} COLLECTINFO;

	typedef struct ControlInfo
	{
		uint8_t  canData1[8];   //CAN data 1;
		uint8_t  canData2[8];   //CAN  data 2;
	}CANINFO;

	typedef struct BagInfo
	{
		CANINFO canData;   //can data ;
		COLLECTINFO colData;//collect data;
	}ALLINFO;


	typedef struct DevInfo
	{
		uint8_t deviceData[10]; 
	}DEVICEINFO;
	
	typedef struct ComInfo
	{
		uint16_t ipAdd[4];//IP add;
		int32_t port;  // port;
	}COMINFO;


	typedef struct TimeInfo
	{
		uint8_t year;
		uint8_t month;
		uint8_t day;
		uint8_t hour;//IP add;
		uint8_t minute;	
	}DATETIME;

	typedef union 
	{
		COMINFO cominfo;     //通讯设置结构体
		DEVICEINFO devinfo;        //ID设置结构体;
		ALLINFO  allinfo;       //数据采集结构体;
		DATETIME timeinfo;	
	}ALLKINDSTRUCT;

	typedef  struct tagCMDHEADandTAIL
	{
		uint32_t  Dest;    //destination;
		uint32_t   Source;    // source;
		uint16_t   FC;            
		uint16_t DataLenth;
		uint16_t CheckSum;     
		uint16_t CheckSumRx;	
	}HEADandTAIL;
	extern uint8_t canupdateflag1;
    extern uint8_t canupdateflag2;
    extern ALLKINDSTRUCT ScanData;
	extern DEVICEINFO deviceinfo;
    extern u8 timefromflag;


	#define FC_DAT 0x01    //data info
	#define FC_WNG  0x02   //warning info
	#define FC_GSI  0x03   //get system info
	#define FC_SCT  0x04   // set Clock Time

	#define FC_SDI  0x09   // set device id
	#define FC_SCI  0x03   // set connect info
	
	#define FC_RWF  0x07
	
	#define FC_GTM  0x05   // get the running time
	#define FC_STM  0x06   //set the running time 
//	uint8_t* creatPtrTotrans(uint32_t dest,uint32_t source,DEVICEINFO *dev,uint16_t FC,DATETIME *runtime,ALLKINDSTRUCT *allkindstruct);
	uint8_t* creatPtrTotrans(uint32_t dest,uint32_t source,DEVICEINFO *dev,uint16_t FC,DATETIME *runtime,ALLKINDSTRUCT *allkindstruct,uint8_t ErrorData);
	void releaseChar_ptr (uint8_t *char_ptr);
	uint16_t parseChar_ptr(uint8_t *char_rx,ALLKINDSTRUCT *allkindstruct);

   void uintxTochar (uint32_t Value,uint8_t *ch_double,uint8_t dataLenth);
   uint32_t charTouintx (uint8_t * ch_double,uint8_t dataLenth);

	uint8_t ProtocolReply(uint8_t *Rx_info);




















#endif
