#ifndef _SAMPLING_H
#define _SAMPLING_H
#include "stm32f10x.h"
#include "protocal.h"
//#include "WIN_24C02.h"

#define RELAY_GPIO GPIOE
#define RELAY_PIN  GPIO_Pin_9
#define RELAY_ON   GPIO_SetBits(RELAY_GPIO, RELAY_PIN)
#define RELAY_OFF   GPIO_ResetBits(RELAY_GPIO, RELAY_PIN)

#define Read_IO()   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8)  
#define Read_SW()   GPIO_ReadInputData(GPIOE)&0x003C

union FTOCHAR
{
  float fdat;
  unsigned char chdat[4];
  uint16_t intdat[2];
  
};

union int32TOCHAR
{
  uint32_t intdat;
  unsigned char chdat[4];   
};

extern uint8_t linkstatu[];
extern uint8_t LinkStatuOnlineFlag; //网络连接在线标志  0:离线  1：WIFI在线 2：GPRS在线 3：同时在线
extern union int32TOCHAR  	MessageCacheCnt;  //离线状态下  数据包计数


extern uint8_t IO_OUT; 
extern uint8_t Switch[];
extern uint8_t DigDataBuffer[];
extern uint8_t DigDataCurrent[];
extern uint8_t ActionCnt[];
extern uint8_t ActionStatus[];

extern union FTOCHAR ResistorCoefficient;
extern float ResistorTemp[];
extern float ResistorTarget;
extern u8  CalibrationFlag,CalibrationCnt;

void ActionNumberClear(void);
void RecordActionNumber(void);
void GetDigDataBuffer(void);
void GetDigDataCurrent(void);
void CompareDigData(void);
void ReadSwitch(void);
float CalculateTemperature(float fR);

void TemperatureMeasureCalibration(float fR);
void SaveResistorCoefficientPara(void);
void ReadResistorCoefficientPara(void);

void ComunitationLinkTest(void);
void changeMessageCacheCnt(u8 type);
void ReadMessageCacheCnt(void);
#endif

