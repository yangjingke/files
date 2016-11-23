#include "SystemBody.h"



//���ڹ����жϵı���
u16   CanComOvertimeCnt=0;	 //CANͨ�ų�ʱ����
u8    ErrorFlag[20];         //��������
u8    CanErrorFlag=0;	    //CANͨ�Ź��ϱ�־
u8    CanErrorPast[14];	   //CAN��������ʷ���ϴ���
u16   WarnTimeCon=0;		//���ϱ���ʱ����Ʊ���

u16   CurrentErrorCnt=0;   //�������������ϼ���
u16   CurrentHistoryValue=0;//������ʷֵ
u8    CurrentErrorFlag=0,TemperErrorFlag=0; //���������ϱ�־
u8    ArresterErrorFlag=0; //�ƶ������߻���Ȧ���ϱ�־
u16   SendTimeCnt=0,RunTimeCnt=0;//����ʱ���ʱ  ����ʱ���ʱ
u16   TemperErrorValue=0;
u8    LiftErrorCnt=0,CurrentTestCnt=0,TemperTestCnt=0; //��ʾ�Ӵ������ϼ�����
u8   BeefTestCnt=0;//���ȹ��ϼ����ʱ����
u8   ErroeTestCnt[8];//���ȹ��ϼ����ʱ����
u8	 ErroeTestCntlimit=5;
				
u8 CurrentBoundaryValue=50,TemperBoundaryValue=30;
//void CLK_Config(void)
//{   /* ʹ��STM32��USART1��ӦGPIO��Clockʱ�� ʹ��STM32��USART1��Clockʱ��*/
////    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
////    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
////	 /* ʹ��STM32��USART2 ��ӦGPIO��Clockʱ�� ʹ��STM32��USART2�Ĺܽ���ӳ��  ��ӳ��  ʹ��STM32��USART2��Clockʱ�� */
////    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
////    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
////   	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
////    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//  /*   ʹ�ܶ˿�2��ʱ�Ӻ�ADC1��ʱ��                                                       */
////	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_ADC1,ENABLE );
////   RCC_ADCCLKConfig(RCC_PCLK2_Div6); //����ADC��Ƶ����6 72M/6=12M,ADC���ʱ�䲻�ܳ���14M��Ҳ����ADC��ʱ��Ƶ��Ϊ12MHz
////	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMAʱ��
//	/* GPIOB, GPIOD and AFIO clocks enable */

////  /* CAN1 Periph clocks enable */
////  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 , ENABLE); 
//  /*        gpio�ɼ�ʹ��                */
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD |RCC_APB2Periph_GPIOE, ENABLE);
//	
//};







void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	//����1�жϳ�ʼ��usart1 nvic configuration
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
		//����2�жϳ�ʼ��usart2 nvic configuration
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   	//����2�жϳ�ʼ��usart2 nvic configuration
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   	//����4�жϳ�ʼ��usart2 nvic configuration
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
//   #if CANSelect
	//CAN1RX �жϳ�ʼ��CAN1RX nvic configuration
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =4;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
//  #else
   	//CAN1RX �жϳ�ʼ��CAN1RX nvic configuration
  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =4;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
//  #endif
	
	
    /* Enable the TIM5 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	    /* Enable the TIM4 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
		
		 /* Enable the DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);         
	
	 NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;  //�����ȼ�7��
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
   NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������
}

 void GPIO_Config(void)
 {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);
  //PE0-15 ��Ϊ�������ɼ���������                         
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������
//  GPIO_Init(GPIOE, &GPIO_InitStructure);
// 
// //PD15 ��Ϊ�������ɼ���������                         
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������
//  GPIO_Init(GPIOD, &GPIO_InitStructure);

	//PE2,3,4,5   SW1,2,3,4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
    GPIO_Init(GPIOE , &GPIO_InitStructure);
   //PE8-IO_OUT   PE9-Relay 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE , &GPIO_InitStructure);
    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
    GPIO_Init(GPIOE , &GPIO_InitStructure);

 //PA4-165_SH/LD   PC4-165_CLK  PC5-165_QH ��Ϊ����������оƬͨ������
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
    GPIO_Init(GPIOC , &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    
    GPIO_Init(GPIOC , &GPIO_InitStructure);

    //GPRSָʾ�������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          
    GPIO_Init(GPIOC , &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;          
    GPIO_Init(GPIOC , &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;          
    GPIO_Init(GPIOD , &GPIO_InitStructure);

	 //WIFIָʾ�������	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
    GPIO_Init(GPIOD , &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          
    GPIO_Init(GPIOE , &GPIO_InitStructure);

	 //DS1302
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);
    

	//BUFF
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD , GPIO_Pin_3);
	delay_ms(200);
	GPIO_ResetBits(GPIOD , GPIO_Pin_3);
 }

 void ADC_Config(void)
 {
	ADC_InitTypeDef ADC_InitStructure; 
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_ADC1,ENABLE );
  RCC_ADCCLKConfig(RCC_PCLK2_Div6); //����ADC��Ƶ����6 72M/6=12M,ADC���ʱ�䲻�ܳ���14M��Ҳ����ADC��ʱ��Ƶ��Ϊ12MHz
  //PBx ��Ϊģ��ͨ����������                         
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
	  ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ʵ��ʹ�õ���ADC1����ADC1�����ڶ���ģʽADC_CR1��λ19:16,���⼸λΪ0000
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//ADC_ScanConvMode ���������Ƿ���ɨ��ģʽ����ʵ�鿪��ɨ��ģʽ.ADC_CR1��λ8
  ADC_InitStructure.ADC_ContinuousConvMode =ENABLE;	//ADC_ContinuousConvMode ���������Ƿ�������ת��ģʽ ģ��ת������������ת��ģʽ��ADC_CR2��λ1
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ�������� ADC_CR2��λ19:17
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���ADC_CR2��λ11
  ADC_InitStructure.ADC_NbrOfChannel = 2;	//˳����й���ת����ADCͨ������ĿADC_SQR1λ23:20
  ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5 );//ADC1��ADC1ͨ��0����1ת��������ʱ��Ϊ239.5����
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5 );//ADC1��ADC1ͨ��1����2ת��������ʱ��Ϊ239.5����
  
  ADC_DMACmd(ADC1, ENABLE); //ʹ��ADC1��DMA���䣬ADC_CR2λ8
    
  ADC_Cmd(ADC1, ENABLE);	//ʹ�ܵ�ADC1,ADC_CR2λ0
	
  ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼��ADC_CR2λ3  
  while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
  ADC_StartCalibration(ADC1);	 //����ADУ׼��ADC_CR2λ2
  while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����

  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	 
 };
void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;   
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMAʱ��
	DMA_DeInit(DMA1_Channel1);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AD_Value;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴����跢�͵��ڴ�  DMA_CCRXλ4
	DMA_InitStructure.DMA_BufferSize = 60;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //�������ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //�ڴ����ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	DMA_Cmd(DMA1_Channel1, ENABLE);//����DMAͨ�� 	
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //enable dma it
};

 void CAN_Config(void)
 {
  GPIO_InitTypeDef  GPIO_InitStructure;
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
  CAN_InitStructure.CAN_Prescaler =24;

  /* CAN filter init   ����*/
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

//  #if CANSelect
	 /* CAN1 Periph clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 , ENABLE); 
  /* Configure CAN1 RX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
 /* Configure CAN1 TX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   /* Remap CAN1 GPIOs */
//  GPIO_PinRemapConfig(GPIO_Remap2_CAN1 , ENABLE);
 
  CAN_DeInit(CAN1);
  /* CAN1 cell init */	 
  CAN_Init(CAN1, &CAN_InitStructure);     //36/(1+2+3)/24=250kbps
	 /* CAN1 filter init   ����*/
  CAN_FilterInitStructure.CAN_FilterNumber = 0;    
  CAN_FilterInit(&CAN_FilterInitStructure);	 
	 /* CAN1�ж����� */  
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
//  #else
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2 , ENABLE); 
  /* Configure CAN2 RX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
 /* Configure CAN2 TX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   /* Remap CAN2 GPIOs */
  GPIO_PinRemapConfig(GPIO_Remap_CAN2 , ENABLE);
 
  CAN_DeInit(CAN2);
  /* CAN1 cell init */	 
  CAN_Init(CAN2, &CAN_InitStructure);     //36/(1+2+3)/24=250kbps
	 /* CAN1 filter init   ����*/
  CAN_FilterInitStructure.CAN_FilterNumber = 14;    
  CAN_FilterInit(&CAN_FilterInitStructure);	 
	 /* CAN1�ж����� */  
  CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
//  #endif
	 
	 
 };
 void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����							 
};
 void TIM5_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */ 
    TIM_TimeBaseStructure.TIM_Period = (10000 - 1);

	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    /* �������Ԥ��Ƶϵ��*/
    TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);

    /* ������Ƶ */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;

    /* ���ϼ���ģʽ */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    /* ��ʼ����ʱ��5 */
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    /* �������жϱ�־ */
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

    /* ����ж�ʹ�� */
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    /* ������ʹ�� */
    TIM_Cmd(TIM5, ENABLE); 
}
 void TIM4_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */ 
    TIM_TimeBaseStructure.TIM_Period = (65536 - 1);

	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    /* �������Ԥ��Ƶϵ��*/
    TIM_TimeBaseStructure.TIM_Prescaler = (65536 - 1);

    /* ������Ƶ */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;

    /* ���ϼ���ģʽ */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    /* ��ʼ����ʱ��5 */
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* �������жϱ�־ */
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

    /* ����ж�ʹ�� */
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    /* ������ʹ�� */
    TIM_Cmd(TIM4, ENABLE); 
}


void setDeviceInfo(ALLKINDSTRUCT *allkind)
{  
		uint8_t i=0;  
		uint8_t check[3];
    deviceinfo=allkind->devinfo;
	  check[0]=0x24;
	  check[1]=0x42;
	  check[2]=0x33;
	 //I2C_EE_BufferWrite(deviceinfo.deviceData,EEP_Firstpage,10);	 
	 for   (i=0;i<10;i++)
	{
		WIN24C02_write(0x00+i,deviceinfo.deviceData[i]);
		delay_ms(100);
	}
  //	I2C_EE_BufferWrite(check,EEP_Firstpage+10,3);	 
	for (i=0;i<3;i++)
	{
		WIN24C02_write(0x0a+i,check[i]);
		delay_ms(100);
	}
		SPI_FLASH_BufferWrite(deviceinfo.deviceData, FLASH_WriteAddress,10);
		
}
void setInitTimeInfo(ALLKINDSTRUCT *allkind)
{		
		uint8_t i=0;
		uint8_t check[3];
		running_time=allkind->timeinfo;
		check[0]=0x24;
	  check[1]=0x42;
	  check[2]=0x33;
	  WIN24C02_write(0x64,running_time.year);
		delay_ms(100);
		WIN24C02_write(0x65,running_time.month);
		delay_ms(100);
		WIN24C02_write(0x66,running_time.day);
		delay_ms(100);
		WIN24C02_write(0x67,running_time.hour);
		delay_ms(100);
		WIN24C02_write(0x68,running_time.minute);
		delay_ms(100);
	for (i=0;i<3;i++)
	{
			WIN24C02_write(0x69+i,check[i]);
			delay_ms(100);
	}
//		I2C_EE_BufferWrite(&running_time.year,EEP_Firstpage+100,1);	 
//	  I2C_EE_BufferWrite(&running_time.month,EEP_Firstpage+101,1);	 
//	   I2C_EE_BufferWrite(&running_time.day,EEP_Firstpage+102,1);	 
//	  I2C_EE_BufferWrite(&running_time.hour,EEP_Firstpage+103,1);	 
//	  I2C_EE_BufferWrite(&running_time.minute,EEP_Firstpage+104,1);	 
//	  I2C_EE_BufferWrite(check,EEP_Firstpage+105,3);	 
		SPI_FLASH_BufferWrite(&running_time.year, FLASH_WriteAddress+100,1);
		SPI_FLASH_BufferWrite(&running_time.month, FLASH_WriteAddress+100,1);
		SPI_FLASH_BufferWrite(&running_time.day, FLASH_WriteAddress+100,1);
		SPI_FLASH_BufferWrite(&running_time.hour, FLASH_WriteAddress+100,1);
		SPI_FLASH_BufferWrite(&running_time.minute, FLASH_WriteAddress+100,1);
}		
void setConnectInfo (ALLKINDSTRUCT *allkind)
{
	
	
}
void sendDeviceInfo(ALLKINDSTRUCT *allkind)
{
	
	
	
}



 void save(ALLKINDSTRUCT *allinfo)
 {
		 
	 
	 
 }

 void ReadHC165Data(void)
 { 
	uint8_t i=0;
//	uint8_t temp=0;
	HC165_CLK_L;//ʼ������
//	delay_ms(1);
	HC165_SHLD_L;
//	delay_ms(1);
 	HC165_SHLD_H;
//	delay_ms(1);
	for(i=0;i<32;i++)
 	{
	  
	  
//	  ScanData.allinfo.colData.digData[7+i/8*8-i%8]=HC165_QH_DATA;
	  ScanData.allinfo.colData.digData[i]=HC165_QH_DATA;
	  HC165_CLK_L;
//	  delay_ms(1);
	   HC165_CLK_H;
//	  delay_ms(1);	     

	}	
//	temp=ScanData.allinfo.colData.digData[20];
//	ScanData.allinfo.colData.digData[20]=ScanData.allinfo.colData.digData[19];
//	ScanData.allinfo.colData.digData[19]=ScanData.allinfo.colData.digData[18];
//	ScanData.allinfo.colData.digData[18]=temp;
 }


void SendOnceMessage(void)
{
	if(WIFISendFlag==1||GPRSSendFlag==1||DebugSendFlag==1)
	{	
		uint8_t *Tx_currentInfo=NULL;
		if(CanErrorFlag == 1) //can error
		{
			canupdateflag1=0xff;canupdateflag2=0xff; 
		}
		Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x01,&running_time,&ScanData,0x00); 
//		Send_Instruction(Tx_currentInfo);
		if(WIFISendFlag==1)
		{
		 	USART3_SendMData(Tx_currentInfo,DatagramLength);
			WIFISendFlag=0;
		}
		if(GPRSSendFlag==1)
		{
		    UART4_SendMData(Tx_currentInfo,DatagramLength);
			GPRSSendFlag=0;
		}
		if(DebugSendFlag==1)
		{
			USART1_SendMData(Tx_currentInfo,DatagramLength); 
			DebugSendFlag=0;
			if(DebugModFlag==1)	//ʵʱ״̬��  ʵʱ����״̬����
				DebugSendFlag=1;		
		}
	//	StatusMessageSaveToFlash(Tx_currentInfo);
		releaseChar_ptr(Tx_currentInfo);
	}
	
}

 void Scan_Current_Info(void)
 {  
 	  uint8_t i=0;
//	  uint8_t errordata=0;
	  uint8_t *Tx_currentInfo=NULL;
	 
	 /*��������ȡ���ṹ���� get dig data */
//	   ReadHC165Data();

	  /*ģ������ȡ���ṹ���� get ana	 data */
  // ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
//	   ScanData.allinfo.colData.anaData[0]=abs(250-4-anaMsg[0]*330/4096);//anaMsg[0]*3.3/0x1000;//    ����  //��ʵ�ʴ���
//	   ScanData.allinfo.colData.anaData[1]=(anaMsg[1]*3300/4095+40-400)/16;//anaMsg[1]*(3.3/4095);	  �¶�
		
		 /*CAN���ݶ�ȡ���ṹ���� get CAN	 data */
		 for(i=0;i<8;i++)
		 {
				ScanData.allinfo.canData.canData1[i]=canMsg1[i];
				ScanData.allinfo.canData.canData2[i]=canMsg2[i];
		 }
//	  if(  ScanData.allinfo.colData.anaData[0]>30|| ScanData.allinfo.colData.anaData[1]>18/*|| ScanData.allinfo.canData.canData2[3]!=0x000*/)
//			{
//				flag_Fault=SET;
//			}

			ErrorTest( );//���ϼ��

//	
//		 	 GPIOD->ODR^= GPIO_Pin_3;
//				Delay(5000000);
			
			///��ʱ���˷��Ͳɼ���Ϣ
	  if(flag_TIMER_RX==SET||timeCounter==0)
		{ 
		    if(CanErrorFlag == 1) //can error
			{
				canupdateflag1=0xff;canupdateflag2=0xff; 
			}
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x01,&running_time,&ScanData,0x00); 
			if(LinkStatuOnlineFlag==0&&MessageCacheCnt.intdat<StatusMessageNumber)//������� ��ʼ��¼��������ݸ���    20160622	 ��
				changeMessageCacheCnt(1);
			Send_Instruction(Tx_currentInfo);
			StatusMessageSaveToFlash(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			ActionNumberClear();
			timeCounter=1;
			flag_Sendontime=SET;
		}

		

		//�б����˷���һ��  if warning  send 1 times
//		 if(flag_Fault==SET && warnCounter<1)
//		{ 
//			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x01,&running_time,&ScanData,errordata); 
//			Send_Instruction(Tx_currentInfo);
//			releaseChar_ptr(Tx_currentInfo);
//			warnCounter++;
//			delay_ms(1000); 
//			warnResetCounter=0;
//
//		}
//	 if(flag_Fault==SET)
//	 {
//		 flag_Fault=RESET;
//	 }
		/* if no more warning,reset the flag of warning  */
//		if(  ScanData.allinfo.colData.anaData[0]>0&&ScanData.allinfo.colData.anaData[0]<50&&ScanData.allinfo.colData.anaData[1]>0&&ScanData.allinfo.colData.anaData[1]<100/*|| ScanData.allinfo.canData.canData2[3]!=0x000*/)
//		{
//			if(warnResetCounter<3)
//			{
//			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x01,&running_time,&ScanData,0x00); 
//			Send_Instruction(Tx_currentInfo);
//			releaseChar_ptr(Tx_currentInfo);
//				delay_ms(1000); 
//				flag_Fault=RESET;
//				warnCounter=0;
//				warnResetCounter++;
//			}
//		}
		/* if  send on time;reset the timer flag*/
		 if(flag_Sendontime==SET)
			{
			 flag_TIMER_RX=RESET;
			 flag_Sendontime=RESET;
			 save(&ScanData);
			}
//			GPIOD->ODR^= GPIO_Pin_4;
//			Delay(5000000);
 }

//���ϼ��
void  ErrorTest(void)
{
	ErrorRealtimeTest();//����ʵʱ���߼��
	ErrorMessageContinueSend();//������Ϣ��������

}

 //������������
void  ErrorMessageContinueSend(void)
{
	if(warnCounter<1)
	{
		uint8_t *Tx_currentInfo=NULL;
	    uint8_t temp_error=0;
		uint8_t FlagCnt=0;
//		uint8_t BoundaryValue=0;
		//	  if(  ScanData.allinfo.colData.anaData[0]>30|| ScanData.allinfo.colData.anaData[1]>18/*|| ScanData.allinfo.canData.canData2[3]!=0x000*/)
//			{
//				flag_Fault=SET;
//			}

		//1-13
		if(ErrorFlag[FlagCnt]==1) //1-13   CAN  Control error check
		{				  
		 	temp_error=ScanData.allinfo.canData.canData2[3];
			CanErrorPast[temp_error]=1;
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80);
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			//  delay_ms(500);
		}
	
		//38	  CurrentErrorFlag=0,TemperErrorFlag=0;
		FlagCnt++;
		temp_error=38;
		if(ErrorFlag[FlagCnt]==1) //38  circuit sensor error check
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			CurrentErrorFlag=1;
			// delay_ms(500);
		}
	
		//39
		FlagCnt++;
		temp_error=39;
		if(ErrorFlag[FlagCnt]==1) //39  temperation sensor error check
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			TemperErrorFlag=1;
			// delay_ms(500);
		}
	
		//40
		FlagCnt++;
		temp_error=40;
//		BoundaryValue=35;
		if(ErrorFlag[FlagCnt]==1) //40  Current OverValue error 
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			// delay_ms(500);
		}
	
		//41
		FlagCnt++;
		temp_error=41;
//		BoundaryValue=18;
		if(ErrorFlag[FlagCnt]==1) //41  temperation OverValue error 
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			// delay_ms(500);
		}
		
	    //42
		FlagCnt++;
		temp_error=42;
		if(ErrorFlag[FlagCnt]==1) //42   CAN  Come error check
		{				  
		 	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			CanErrorFlag=1;
			// delay_ms(500);
		}
		
		//43
		FlagCnt++;
		temp_error=43;
		if(ErrorFlag[FlagCnt]==1) //43  SwitchValue Error
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;  
			// delay_ms(500);
		}
		
		//44
		FlagCnt++;
		temp_error=44;
		if(ErrorFlag[FlagCnt]==1) //44  SwitchValue Error
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			// delay_ms(500);
		}
	
		//45
		FlagCnt++;
		temp_error=45;
		if(ErrorFlag[FlagCnt]==1) //45  SwitchValue Error
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			// delay_ms(500);
		}
		
		//46
		FlagCnt++;
		temp_error=46;
		if(ErrorFlag[FlagCnt]==1) //46  SwitchValue Error
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			// delay_ms(500);
		}
	
		//47
		FlagCnt++;
		temp_error=47;
		if(ErrorFlag[FlagCnt]==1) //47  SwitchValue Error
		{	
			
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
		
			// delay_ms(500);
			
		}
		
		//48
		FlagCnt++;
		temp_error=48;
		if(ErrorFlag[FlagCnt]==1) //48  SwitchValue Error
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			// delay_ms(500);
		}
		
		//49
		FlagCnt++;
		temp_error=49;
		if(ErrorFlag[FlagCnt]==1) //49  SwitchValue Error
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			// delay_ms(500);
		}
		
		//50
		FlagCnt++;
		temp_error=50;
		if(ErrorFlag[FlagCnt]==1) //50  SwitchValue Error
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			// delay_ms(500);
		}
		
		//51
		FlagCnt++;
		temp_error=51;
		if(ErrorFlag[FlagCnt]==1) //51  SwitchValue Error
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;  
			// delay_ms(500);
		}
		
		//52
		FlagCnt++;
		temp_error=52;
		if(ErrorFlag[FlagCnt]==1 ) //52  SwitchValue Error
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;
			// delay_ms(500);
		}
		
		//53
		FlagCnt++;
		temp_error=53;
		if(ErrorFlag[FlagCnt]==1) //53  SwitchValue Error
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;  
			// delay_ms(500);
		}
	
		//54
		FlagCnt++;
		temp_error=54;
		if(ErrorFlag[FlagCnt]==1 ) //54  Arrester Error
		{	
			Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
			Send_Instruction(Tx_currentInfo);
			releaseChar_ptr(Tx_currentInfo);
			warnCounter=1;
			ErrorFlag[FlagCnt]=1;  
			// delay_ms(500);
		}
		

	}


} 

//����ʵʱ���
void  ErrorRealtimeTest(void)
{

		uint8_t *Tx_currentInfo=NULL;
	    uint8_t temp_error=0;
		uint8_t FlagCnt=0;
		uint8_t BoundaryValue=0;
		//	  if(  ScanData.allinfo.colData.anaData[0]>30|| ScanData.allinfo.colData.anaData[1]>18/*|| ScanData.allinfo.canData.canData2[3]!=0x000*/)
//			{
//				flag_Fault=SET;
//			}

		//1-13
		if(ScanData.allinfo.canData.canData1[3]!=0x00 && CanErrorFlag==0) //1-13   CAN  Control error check
		{	
			temp_error=ScanData.allinfo.canData.canData1[3];
		    if(CanErrorPast[temp_error]==0)
			{			  				 	
				CanErrorPast[temp_error]=1;
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80);
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);
				warnCounter=1;
				ErrorFlag[FlagCnt]=1;
				//  delay_ms(500);
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {
				uint8_t i=1;
				for(i=1;i<14;i++)
				{
					if(CanErrorPast[i]==1)
					{
					  	temp_error=i; 
						Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
						Send_Instruction(Tx_currentInfo);
						releaseChar_ptr(Tx_currentInfo); 
						CanErrorPast[i]=0;
					}
				} 				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
		}
		//38	  CurrentErrorFlag=0,TemperErrorFlag=0;
		FlagCnt++;
		temp_error=38;
		if(CurrentErrorCnt>1500) //38  circuit sensor error check	  2s
		{	
		    if(ErrorFlag[FlagCnt]==0)
			{
				
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);
				warnCounter=1;
				ErrorFlag[FlagCnt]=1;
				CurrentErrorFlag=1;
				// delay_ms(500);
			
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				CurrentErrorFlag=0;
				// delay_ms(500);
			 }
			 
		}
		//39
		FlagCnt++;
		temp_error=39;
		if(TemperErrorValue>550) //39  temperation sensor error check
		{	
		    if(ErrorFlag[FlagCnt]==0)
			{
				
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);
				warnCounter=1;
				ErrorFlag[FlagCnt]=1;
				TemperErrorFlag=1;
				// delay_ms(500);
				
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				TemperErrorFlag=0;
				// delay_ms(500);
			 }
			 
		}
		//40
		FlagCnt++;
		temp_error=40;
		BoundaryValue=CurrentBoundaryValue;
		
		if(ScanData.allinfo.colData.anaData[0]>BoundaryValue && CurrentErrorFlag==0) //40  Current OverValue error 
		{	
		    if(ErrorFlag[FlagCnt]==0)
			{
				if(CurrentTestCnt>200)
				{
					Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
					Send_Instruction(Tx_currentInfo);
					releaseChar_ptr(Tx_currentInfo);
					warnCounter=1;
					ErrorFlag[FlagCnt]=1;
					// delay_ms(500);
				}
				else
				   CurrentTestCnt++;
			}
		}
		else
		{
			if(CurrentErrorFlag==1 && ErrorFlag[FlagCnt]==1)
			{
			    Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			}
			 if(ErrorFlag[FlagCnt]==1 && ScanData.allinfo.colData.anaData[0]<BoundaryValue-5)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
			 CurrentTestCnt=0;
		}
		//41
		FlagCnt++;
		temp_error=41;
		BoundaryValue=TemperBoundaryValue;
		if(ScanData.allinfo.colData.anaData[1]>BoundaryValue && TemperErrorFlag==0) //41  temperation OverValue error 
		{	
		    if(ErrorFlag[FlagCnt]==0)
			{
				if(TemperTestCnt>30)
				{
					Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
					Send_Instruction(Tx_currentInfo);
					releaseChar_ptr(Tx_currentInfo);
					warnCounter=1;
					ErrorFlag[FlagCnt]=1; 			
					// delay_ms(500);
				}
				else
				   TemperTestCnt++;
			 }
		}
		else
		{
		     if(TemperErrorFlag==1 && ErrorFlag[FlagCnt]==1)
			 {
			 	Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
			 if(ErrorFlag[FlagCnt]==1 && ScanData.allinfo.colData.anaData[1]<BoundaryValue-3)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
			 TemperTestCnt=0;
		}
	    //42
		FlagCnt++;
		temp_error=42;
		if(CanComOvertimeCnt>2000) //42   CAN  Come error check	  5000
		{				  
		 	if(ErrorFlag[FlagCnt]==0)
			{
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);
				warnCounter=1;
				ErrorFlag[FlagCnt]=1;
				CanErrorFlag=1;
				// delay_ms(500);
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {
			  	 
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				CanErrorFlag=0;
				// delay_ms(500);
			 }
		}

		//43
		FlagCnt++;
		temp_error=43;
		if(ScanData.allinfo.colData.digData[0]==0) //43  SwitchValue Error
		{	
			if(ErrorFlag[FlagCnt]==0)
			{
				if(BeefTestCnt>ErroeTestCntlimit)
				{
					Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
					Send_Instruction(Tx_currentInfo);
					releaseChar_ptr(Tx_currentInfo);
					warnCounter=1;
					ErrorFlag[FlagCnt]=1;  
					// delay_ms(500);
				}
				else
					BeefTestCnt++;
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
			 BeefTestCnt=0;
		}
		//44
		FlagCnt++;
		temp_error=44;
		if(ScanData.allinfo.colData.digData[27]==1) //44  SwitchValue Error
		{	
		    if(ErrorFlag[FlagCnt]==0)
			{
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);
				warnCounter=1;
				ErrorFlag[FlagCnt]=1;
				// delay_ms(500);
			} 
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
		}
		//45
		FlagCnt++;
		temp_error=45;
		if(ScanData.allinfo.colData.digData[2]==0) //45  SwitchValue Error
		{	
			if(ErrorFlag[FlagCnt]==0)
			{
			    if(ErroeTestCnt[0]>ErroeTestCntlimit)
				{
					Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
					Send_Instruction(Tx_currentInfo);
					releaseChar_ptr(Tx_currentInfo);
					warnCounter=1;
					ErrorFlag[FlagCnt]=1;
					// delay_ms(500);
				}
				else
					ErroeTestCnt[0]++;
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
			 ErroeTestCnt[0]=0;
		}
		//46
		FlagCnt++;
		temp_error=46;
		if(ScanData.allinfo.colData.digData[28]==1) //46  SwitchValue Error
		{	
			if(ErrorFlag[FlagCnt]==0)
			{
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);
				warnCounter=1;
				ErrorFlag[FlagCnt]=1;
				// delay_ms(500);
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
		}
		//47
		FlagCnt++;
		temp_error=47;
		if(ScanData.allinfo.colData.digData[4]==0 || (ScanData.allinfo.colData.digData[3]==1&&ScanData.allinfo.colData.digData[5]==0)) //47  SwitchValue Error
		{	
		    if(ErrorFlag[FlagCnt]==0)
			{
				if(LiftErrorCnt>50)
				{
					Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
					Send_Instruction(Tx_currentInfo);
					releaseChar_ptr(Tx_currentInfo);
					warnCounter=1;
					ErrorFlag[FlagCnt]=1;
				}
				else
					 LiftErrorCnt++;
				// delay_ms(500);
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
			 LiftErrorCnt=0;
		}

		//48
		FlagCnt++;
		temp_error=48;
		if(ScanData.allinfo.colData.digData[6]==0 ) //48  SwitchValue Error
		{	
			if(ErrorFlag[FlagCnt]==0)
			{
				if(ErroeTestCnt[1]>ErroeTestCntlimit)
				{
					Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
					Send_Instruction(Tx_currentInfo);
					releaseChar_ptr(Tx_currentInfo);
					warnCounter=1;
					ErrorFlag[FlagCnt]=1;
					// delay_ms(500);
				}
				else
					ErroeTestCnt[1]++;
				
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
			 	ErroeTestCnt[1]=0;
		}
		//49
		FlagCnt++;
		temp_error=49;
		if(ScanData.allinfo.colData.digData[29]==1 ) //49  SwitchValue Error
		{	
			if(ErrorFlag[FlagCnt]==0)
			{
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);
				warnCounter=1;
				ErrorFlag[FlagCnt]=1;
				// delay_ms(500);
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
		}
		//50
		FlagCnt++;
		temp_error=50;
		if(ScanData.allinfo.colData.digData[8]==0 ) //50  SwitchValue Error
		{	
			if(ErrorFlag[FlagCnt]==0)
			{
				if(ErroeTestCnt[2]>ErroeTestCntlimit)
				{
					Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
					Send_Instruction(Tx_currentInfo);
					releaseChar_ptr(Tx_currentInfo);
					warnCounter=1;
					ErrorFlag[FlagCnt]=1;
					// delay_ms(500);
				}
				else
					ErroeTestCnt[2]++;
			
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
			 ErroeTestCnt[2]=0;
		}
		//51
		FlagCnt++;
		temp_error=51;
		if(ScanData.allinfo.colData.digData[9]==0 ) //51  SwitchValue Error
		{	
			if(ErrorFlag[FlagCnt]==0)
			{
				if(ErroeTestCnt[3]>ErroeTestCntlimit)
				{
					Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
					Send_Instruction(Tx_currentInfo);
					releaseChar_ptr(Tx_currentInfo);
					warnCounter=1;
					ErrorFlag[FlagCnt]=1;  
					// delay_ms(500);
				}
				else
					ErroeTestCnt[3]++;
			
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
			 ErroeTestCnt[3]=0;
		}
		//52
		FlagCnt++;
		temp_error=52;
		if(ScanData.allinfo.colData.digData[10]==0 && ScanData.allinfo.colData.digData[8]==1 ) //52  SwitchValue Error
		{	
			if(ErrorFlag[FlagCnt]==0)
			{
				if(ErroeTestCnt[4]>ErroeTestCntlimit)
				{
					Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
					Send_Instruction(Tx_currentInfo);
					releaseChar_ptr(Tx_currentInfo);
					warnCounter=1;
					ErrorFlag[FlagCnt]=1;
					// delay_ms(500);
				}
				else
					ErroeTestCnt[4]++;
				
			}
		}
		else
		{
			 if(ErrorFlag[FlagCnt]==1)
			 {			  	
				Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
				Send_Instruction(Tx_currentInfo);
				releaseChar_ptr(Tx_currentInfo);  				
				ErrorFlag[FlagCnt]=0;
				// delay_ms(500);
			 }
			 ErroeTestCnt[4]=0;
		}


		if(ArresterControlEn==1)
		{
			//53
			FlagCnt++;
			temp_error=53;
			if(ScanData.allinfo.colData.digData[11]==0 ) //53  SwitchValue Error
			{
				if(ErrorFlag[FlagCnt]==0)
				{
					if(ErroeTestCnt[5]>ErroeTestCntlimit)
					{
						Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
						Send_Instruction(Tx_currentInfo);
						releaseChar_ptr(Tx_currentInfo);
						warnCounter=1;
						ErrorFlag[FlagCnt]=1;  
						// delay_ms(500);
					}
					else
						 ErroeTestCnt[5]++;
				}
			}
			else
			{
				 if(ErrorFlag[FlagCnt]==1)
				 {			  	
					Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
					Send_Instruction(Tx_currentInfo);
					releaseChar_ptr(Tx_currentInfo);  				
					ErrorFlag[FlagCnt]=0;
					// delay_ms(500);
				 }
				 ErroeTestCnt[5]=0;
			}
			//54
			FlagCnt++;
			temp_error=54;
			if(ArresterErrorFlag==1 ) //54  Arrester Error
			{	
				if(ErrorFlag[FlagCnt]==0)
				{					
					Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error|0x80); 
					Send_Instruction(Tx_currentInfo);
					releaseChar_ptr(Tx_currentInfo);
					warnCounter=1;
					ErrorFlag[FlagCnt]=1;  
					// delay_ms(500);					
				}
			}
			else
			{
				 if(ErrorFlag[FlagCnt]==1)
				 {			  	
					Tx_currentInfo=creatPtrTotrans(DESTNATION,DEVICEADD,&deviceinfo,0x02,&running_time,&ScanData,temp_error); 
					Send_Instruction(Tx_currentInfo);
					releaseChar_ptr(Tx_currentInfo);  				
					ErrorFlag[FlagCnt]=0;
					// delay_ms(500);
				 }
				 
			}
		}

	


} 




 void delay_ms(u16 time) 
	 {     
		 u16 i=0;   
		 while(time--)  
			 {      
				 i=12000;   
				 while(i--) ; 
	     }
	 }
 void Delay(uint32_t nCount)
 {
	 while(nCount!=0)
	 {
		 nCount--;
	 }
 }

void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* ʹ��LED��ӦGPIO��Clockʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);

    /* ��ʼ��ϵͳָʾLED��GPIO�ܽţ�����Ϊ������� */				  //���ö˿�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | 
		GPIO_Pin_6 | GPIO_Pin_7;   //������4��led�ƹܽ�
														 	  //���öԶ˿ڵ�ģʽ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //����Ϊ ����(Push-Pull)���
														 	  //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�������ٶ�Ϊ50MHz

    GPIO_Init(GPIOD, &GPIO_InitStructure);	                  //����GPIO��ʼ������
	 //Ĭ��Ϊ��
	GPIO_SetBits(GPIOD , GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

	 /* ��ʼ��ϵͳָʾLED��GPIO�ܽţ�����Ϊ������� */				  //���ö˿�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;   //������4��led�ƹܽ�
														 	  //���öԶ˿ڵ�ģʽ
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //����Ϊ ����(Push-Pull)���
//														 	  //
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�������ٶ�Ϊ50MHz

    GPIO_Init(GPIOE, &GPIO_InitStructure);	                  //����GPIO��ʼ������
	 //Ĭ��Ϊ��
	GPIO_SetBits(GPIOE ,  GPIO_Pin_7);
}

// 0-9 	deviceinfo.deviceData
// 10-12 check
// 20 21  commode  arrester
// 30-33  MessageCacheCnt
//50\51	   current temperature
//	100-104   running_time
//  104-106   timecheck
//  130-141	  FlashPara
//  150-153	  Resister
//  255       saveflag
void  FirstSaveSystemInfo(void)
{
	uint8_t check[3]={0};
    uint8_t timecheck[3]={0};
	uint8_t i=0,temp=0;
	temp=WIN24C02_read(0xFF);
	if(temp==0xAA)//�Ѵ� ����
		return;
	 //��һ�����ݴ洢
	deviceinfo.deviceData[0]='1';
	deviceinfo.deviceData[1]='6';
	deviceinfo.deviceData[2]='0';
	deviceinfo.deviceData[3]='2';
	deviceinfo.deviceData[4]='2';
	deviceinfo.deviceData[5]='4';
	deviceinfo.deviceData[6]='0';
	deviceinfo.deviceData[7]='0';
	deviceinfo.deviceData[8]='0';
	deviceinfo.deviceData[9]='1';
	 for   (i=0;i<10;i++)
	{
	WIN24C02_write(0x00+i,deviceinfo.deviceData[i]);
		delay_ms(100);
	}
  
  	check[0]=0x24,check[1]=0x42,check[2]=0x33;
	for (i=0;i<3;i++)
	{
		WIN24C02_write(0x0a+i,check[i]);
		delay_ms(100);
	}

    running_time.year=0;
    running_time.month=0;
	running_time.day=0;
	running_time.hour=0;
	running_time.minute=0;
	WIN24C02_write(0x64,running_time.year);
	delay_ms(100);
	WIN24C02_write(0x65,running_time.month);
	delay_ms(100);
	WIN24C02_write(0x66,running_time.day);
	delay_ms(100);
	WIN24C02_write(0x67,running_time.hour);
	delay_ms(100);
	WIN24C02_write(0x68,running_time.minute);
	delay_ms(100);

	timecheck[0]=0x24,timecheck[1]=0x42,timecheck[2]=0x33;
	for (i=0;i<3;i++)
	{
		WIN24C02_write(0x69+i,timecheck[i]);
		delay_ms(100);
	}
    
	WIN24C02_write(50,CurrentBoundaryValue);
	delay_ms(100);
	WIN24C02_write(51,TemperBoundaryValue);
	delay_ms(100);


	WIN24C02_write(20,ComModeSelect);
	delay_ms(100);
	WIN24C02_write(21,ArresterControlEn);
	delay_ms(100);
	//	SaveFlashpara();
	//  Set_DS1302(&settime);
	WIN24C02_write(0xFF,0xAA);
	delay_ms(100);

	SaveFlashpara();
	SaveResistorCoefficientPara();
	changeMessageCacheCnt(0);
}


// 0-9 	deviceinfo.deviceData
// 10-12 check
//	100-104   running_time
//  104-106   timecheck
void  getTimeandDeviceInfo(void)
{   
	uint8_t check[3]={0};
    uint8_t timecheck[3]={0};
	uint8_t i=0;
	for(i=0;i<10;i++)
	{
		deviceinfo.deviceData[i]=WIN24C02_read(0x00+i);
	}
	for(i=0;i<3;i++)
	{
		check[i]=WIN24C02_read(0x0a+i);
	}
	running_time.year=WIN24C02_read(0x64);
	running_time.month=WIN24C02_read(0x65);
	running_time.day=WIN24C02_read(0x66);
	running_time.hour=WIN24C02_read(0x67);
	running_time.minute=WIN24C02_read(0x68);
	for(i=0;i<3;i++)
	{
		timecheck[i]=WIN24C02_read(0x69+i);
	}

	if(check[0]!=0x24||check[1]!=0x42||check[2]!=0x33)
	{
	//		SPI_FLASH_BufferRead(deviceinfo.deviceData, FLASH_ReadAddress, 10);
		deviceinfo.deviceData[0]='0';
		deviceinfo.deviceData[1]='0';
		deviceinfo.deviceData[2]='0';
		deviceinfo.deviceData[3]='0';
		deviceinfo.deviceData[4]='0';
		deviceinfo.deviceData[5]='0';
		deviceinfo.deviceData[6]='0';
		deviceinfo.deviceData[7]='0';
		deviceinfo.deviceData[8]='0';
		deviceinfo.deviceData[9]='0';
//		 for   (i=0;i<10;i++)
//		{
//		WIN24C02_write(0x00+i,deviceinfo.deviceData[i]);
//		delay_ms(100);
//		}
//	
//		check[0]=0x24,check[1]=0x42,check[2]=0x33;
//		for (i=0;i<3;i++)
//		{
//		WIN24C02_write(0x0a+i,check[i]);
//		delay_ms(100);
//		}
	
	}

	if(timecheck[0]!=0x24||timecheck[1]!=0x42||timecheck[2]!=0x33)
	{
	
		running_time.year=0;
		running_time.month=0;
		running_time.day=0;
		running_time.hour=0;
		running_time.minute=0;
//		WIN24C02_write(0x64,running_time.year);
//		delay_ms(100);
//		WIN24C02_write(0x65,running_time.month);
//		delay_ms(100);
//		WIN24C02_write(0x66,running_time.day);
//		delay_ms(100);
//		WIN24C02_write(0x67,running_time.hour);
//		delay_ms(100);
//		WIN24C02_write(0x68,running_time.minute);
//		delay_ms(100);
//
//		timecheck[0]=0x24,timecheck[1]=0x42,timecheck[2]=0x33;
//		for (i=0;i<3;i++)
//		{
//			WIN24C02_write(0x69+i,timecheck[i]);
//			delay_ms(100);
//		}
	
	
	}
	else
	{
		CurrentBoundaryValue=WIN24C02_read(50);	
		TemperBoundaryValue=WIN24C02_read(51);

		ComModeSelect=WIN24C02_read(20);	
		ArresterControlEn=WIN24C02_read(21);
	
	}
		
}

void recordTime(void)
{	
	uint8_t i=0;	
		 uint8_t timecheck[3];
       running_time.minute++;
			 if(running_time.minute==60)
			 { running_time.minute=0;
				 running_time.hour++;
				 
			 }
			  if(running_time.hour==24)
				 { 
					 running_time.hour=0;
					 running_time.day++;
				  
				 }				
				  if(running_time.day==30)
					 {
						 running_time.day=0;
						 running_time.month++;
						
					 }
					   if(running_time.month==12)
						 {
							 
							 running_time.month=0;
							 running_time.year++;
						 }
	timecheck[0]=0x24;
	  timecheck[1]=0x42;
	  timecheck[2]=0x33;
	 WIN24C02_write(0x64,running_time.year);
		delay_ms(100);
	 WIN24C02_write(0x65,running_time.month);
		delay_ms(100);
	 WIN24C02_write(0x66,running_time.day);
		delay_ms(100);
	 WIN24C02_write(0x67,running_time.hour);
		delay_ms(100);
	 WIN24C02_write(0x68,running_time.minute);
		delay_ms(100);
	for (i=0;i<3;i++)
	{
		WIN24C02_write(0x69+i,timecheck[i]);
		delay_ms(100);
	}
//		I2C_EE_BufferWrite(&running_time.year,EEP_Firstpage+100,1);	 
//	  I2C_EE_BufferWrite(&running_time.month,EEP_Firstpage+101,1);	 
//	   I2C_EE_BufferWrite(&running_time.day,EEP_Firstpage+102,1);	 
//	  I2C_EE_BufferWrite(&running_time.hour,EEP_Firstpage+103,1);	 
//	  I2C_EE_BufferWrite(&running_time.minute,EEP_Firstpage+104,1);	 
//	  I2C_EE_BufferWrite(check,EEP_Firstpage+105,3);	 
//		SPI_FLASH_BufferWrite(&running_time.year, FLASH_WriteAddress+100,1);
//		SPI_FLASH_BufferWrite(&running_time.month, FLASH_WriteAddress+101,1);
//		SPI_FLASH_BufferWrite(&running_time.day, FLASH_WriteAddress+102,1);
//		SPI_FLASH_BufferWrite(&running_time.hour, FLASH_WriteAddress+103,1);
//		SPI_FLASH_BufferWrite(&running_time.minute, FLASH_WriteAddress+104,1);
     


		 
		 
	 }
	

