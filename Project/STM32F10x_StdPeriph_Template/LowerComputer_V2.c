/****************修改时间2015-1-18***************************/
#include "LowerComputer_V2.h"
#include "UserCustom.h"

typedef union Float2Bytes
{
	float Float;
	unsigned char Bytes[sizeof(float)];
}Float2Bytes;

#define LC_TRUE  1
#define LC_FALSE 0
////常数宏定义////(不需要修改)
#define NUM_R_CMD_BYTE  2

////**校验宏定义表**/////
//校验结果宏定义
#define LC_CHECK_RESULT_NONE      0
#define LC_CHECK_RESULT_R_GROUP1  3
#define LC_CHECK_RESULT_R_GROUP2  4
#define LC_CHECK_RESULT_R_GROUP3  5
#define LC_CHECK_RESULT_R_GROUP4  6
#define LC_CHECK_RESULY_CMD       1

//校验字节(接收)
#define LC_R_NO1    0xaa
#define LC_R_NO2    0xbb
#define LC_R_GROUP1 0xc1
#define LC_R_GROUP2 0xc2
#define LC_R_GROUP3 0xc3
#define LC_R_GROUP4 0xc4

#define LC_R_CMD    0xfb

//校验字节(发送)
#define LC_S_NO1         0xff
#define LC_S_NO2         0xee
#define LC_S_DATA1       0xdd
#define LC_S_DATA2       0xde

#define LC_S_CMD         0xc0
#define LC_S_REC_GROUP1  0xc1
#define LC_S_REC_GROUP2  0xc2
#define LC_S_REC_GROUP3  0xc3
#define LC_S_REC_GROUP4  0xc4
////**命令宏定义表**////(需要与上位机对应)
#define CMD_STOP_SEND      0x01
#define CMD_START_SEND     0x02
#define LC_CMD01           0x03
#define LC_CMD02           0x04
#define LC_CMD01_DISLC     0x05
#define LC_CMD02_ENLC      0x06
#define LC_CMD_CHANGE_SEND 0x07
#define LC_CMD_03          0x08
#define LC_CMD_04          0x09

#define LC_CMD_ENDIAN      0x10

#define CMD_SEND_REC       0xfc

#define CMD_S_LC_ENDIAN    0x01
/****************** 函数声明 *****************************/
void HIT_LC_Init(void);
LC_Uint8 LC_Is_LittleEndian(void);
void LC_SendUint16(LC_Uint16 data);
void LC_SendFloat(float data);
void LC_SendDataReceived1(void);
void LC_SendData(void);
void LC_PutByteInFIFObuffer(LC_Uint8 LC_byte);
void LC_ReceivedData(void);
void LC_SetCarData(void);
void LC_SetCarData2(void);
void LC_SetCarData3(void);
void LC_SetCarData4(void);

void StopCarAndLCSned(void);
void StartLCSendAndRunCar(void);


///////////////// Queue_byte相关 ///////////////////////
void  Queue_byteInit(QUEUE_byte *);        //消息初始化
void  UpHead(QUEUE_byte *);                //前移队首指针
void  UpTail(QUEUE_byte *);                //前移队尾指针
LC_Uint8 GetByteFromQueue(QUEUE_byte *);
LC_Uint16 GetUint16FromQueue(QUEUE_byte *This);
float GetFloatFromQueue(QUEUE_byte *This);
void  PutByteInQueue(QUEUE_byte *,LC_Uint8);
LC_Uint8 Queue_byteCount(QUEUE_byte *);


/****************** 变量声明 *****************************/
LC_Uint8  LCMachineStatuse = 0;

Float2Bytes FloatBytesConverter;

LC_Uint8 LC_SendEnable = LC_TRUE;
LC_Uint8 LC_Num_SendEnable = 0;
/****************** 变量声明 *****************************/
QUEUE_byte  LC_FIFO_ByteBuffer;
QUEUE_byte  *pLC_FIFO_ByteBuffer=&LC_FIFO_ByteBuffer;

//发送变量声明
#ifdef S1_CUSTOM1
	#ifdef TYPE_S1_C1
	extern TYPE_S1_C1 S1_CUSTOM1;
	#else
	#ifdef TYPE_S1_C1_FLOAT_DEC
	extern float S1_CUSTOM1;
	#endif
	#endif
#endif
#ifdef S1_CUSTOM2
	#ifdef TYPE_S1_C2
	extern TYPE_S1_C2 S1_CUSTOM2;
	#else
	#ifdef TYPE_S1_C2_FLOAT_DEC
	extern float S1_CUSTOM2;
	#endif
	#endif
#endif
#ifdef S1_CUSTOM3
	#ifdef TYPE_S1_C3
	extern TYPE_S1_C3 S1_CUSTOM3;
	#else
	#ifdef TYPE_S1_C3_FLOAT_DEC
	extern float S1_CUSTOM3;
	#endif
	#endif
#endif
#ifdef S1_CUSTOM4
	#ifdef TYPE_S1_C4
	extern TYPE_S1_C4 S1_CUSTOM4;
	#else
	#ifdef TYPE_S1_C4_FLOAT_DEC
	extern float S1_CUSTOM4;
	#endif
	#endif
#endif
#ifdef S1_CUSTOM5
	#ifdef TYPE_S1_C5
	extern TYPE_S1_C5 S1_CUSTOM5;
	#else
	#ifdef TYPE_S1_C5_FLOAT_DEC
	extern float S1_CUSTOM5;
	#endif
	#endif
#endif
#ifdef S1_CUSTOM6
	#ifdef TYPE_S1_C6
	extern TYPE_S1_C6 S1_CUSTOM6;
	#else
	#ifdef TYPE_S1_C6_FLOAT_DEC
	extern float S1_CUSTOM6;
	#endif
	#endif
#endif
#ifdef S1_CUSTOM7
	#ifdef TYPE_S1_C7
	extern TYPE_S1_C7 S1_CUSTOM7;
	#else
	#ifdef TYPE_S1_C7_FLOAT_DEC
	extern float S1_CUSTOM7;
	#endif
	#endif
#endif
#ifdef S1_CUSTOM8
	#ifdef TYPE_S1_C8
	extern TYPE_S1_C8 S1_CUSTOM8;
	#else
	#ifdef TYPE_S1_C8_FLOAT_DEC
	extern float S1_CUSTOM8;
	#endif
	#endif
#endif
#ifdef S1_CUSTOM9
	#ifdef TYPE_S1_C9
	extern TYPE_S1_C9 S1_CUSTOM9;
	#else
	#ifdef TYPE_S1_C9_FLOAT_DEC
	extern float S1_CUSTOM9;
	#endif
	#endif
#endif
#ifdef S1_CUSTOM10
	#ifdef TYPE_S1_C10
	extern TYPE_S1_C10 S1_CUSTOM10;
	#else
	#ifdef TYPE_S1_C10_FLOAT_DEC
	extern float S1_CUSTOM10;
	#endif
	#endif
#endif
#ifdef S1_CUSTOM11
	#ifdef TYPE_S1_C11
	extern TYPE_S1_C11 S1_CUSTOM11;
	#else
	#ifdef TYPE_S1_C11_FLOAT_DEC
	extern float S1_CUSTOM11;
	#endif
	#endif
#endif
#ifdef S1_CUSTOM12
	#ifdef TYPE_S1_C12
	extern TYPE_S1_C12 S1_CUSTOM12;
	#else
	#ifdef TYPE_S1_C12_FLOAT_DEC
	extern float S1_CUSTOM12;
	#endif
	#endif
#endif
#ifdef S1_CUSTOM13
	#ifdef TYPE_S1_C13
	extern TYPE_S1_C13 S1_CUSTOM13;
	#else
	#ifdef TYPE_S1_C13_FLOAT_DEC
	extern float S1_CUSTOM13;
	#endif
	#endif
#endif
#ifdef S1_BIT76
	#ifdef TYPE_S1_BIT76
	extern TYPE_S1_BIT76 S1_BIT76;
	#else
	#ifdef TYPE_S1_BIT76_FLOAT_DEC
	extern float S1_BIT76;
	#endif
	#endif
#endif
#ifdef S1_BIT5
	#ifdef TYPE_S1_BIT5
	extern TYPE_S1_BIT5 S1_BIT5;
	#else
	#ifdef TYPE_S1_BIT5_FLOAT_DEC
	extern float S1_BIT5;
	#endif
	#endif
#endif
#ifdef S1_BIT4
	#ifdef TYPE_S1_BIT4
	extern TYPE_S1_BIT4 S1_BIT4;
	#else
	#ifdef TYPE_S1_BIT4_FLOAT_DEC
	extern float S1_BIT4;
	#endif
	#endif
#endif
#ifdef S1_BIT3
	#ifdef TYPE_S1_BIT3
	extern TYPE_S1_BIT3 S1_BIT3;
	#else
	#ifdef TYPE_S1_BIT3_FLOAT_DEC
	extern float S1_BIT3;
	#endif
	#endif
#endif
#ifdef S1_BIT2
	#ifdef TYPE_S1_BIT2
	extern TYPE_S1_BIT2 S1_BIT2;
	#else
	#ifdef TYPE_S1_BIT2_FLOAT_DEC
	extern float S1_BIT2;
	#endif
	#endif
#endif
#ifdef S1_BIT1
	#ifdef TYPE_S1_BIT1
	extern TYPE_S1_BIT1 S1_BIT1;
	#else
	#ifdef TYPE_S1_BIT1_FLOAT_DEC
	extern float S1_BIT1;
	#endif
	#endif
#endif
#ifdef S1_BIT0
	#ifdef TYPE_S1_BIT0
	extern TYPE_S1_BIT0 S1_BIT0;
	#else
	#ifdef TYPE_S1_BIT0_FLOAT_DEC
	extern float S1_BIT0;
	#endif
	#endif
#endif

#ifdef S2_CUSTOM1
	#ifdef TYPE_S2_C1
	extern TYPE_S2_C1 S2_CUSTOM1;
	#else
	#ifdef TYPE_S2_C1_FLOAT_DEC
	extern float S2_CUSTOM1;
	#endif
	#endif
#endif
#ifdef S2_CUSTOM2
	#ifdef TYPE_S2_C2
	extern TYPE_S2_C2 S2_CUSTOM2;
	#else
	#ifdef TYPE_S2_C2_FLOAT_DEC
	extern float S2_CUSTOM2;
	#endif
	#endif
#endif
#ifdef S2_CUSTOM3
	#ifdef TYPE_S2_C3
	extern TYPE_S2_C3 S2_CUSTOM3;
	#else
	#ifdef TYPE_S2_C3_FLOAT_DEC
	extern float S2_CUSTOM3;
	#endif
	#endif
#endif
#ifdef S2_CUSTOM4
	#ifdef TYPE_S2_C4
	extern TYPE_S2_C4 S2_CUSTOM4;
	#else
	#ifdef TYPE_S2_C4_FLOAT_DEC
	extern float S2_CUSTOM4;
	#endif
	#endif
#endif
#ifdef S2_CUSTOM5
	#ifdef TYPE_S2_C5
	extern TYPE_S2_C5 S2_CUSTOM5;
	#else
	#ifdef TYPE_S2_C5_FLOAT_DEC
	extern float S2_CUSTOM5;
	#endif
	#endif
#endif
#ifdef S2_CUSTOM6
	#ifdef TYPE_S2_C6
	extern TYPE_S2_C6 S2_CUSTOM6;
	#else
	#ifdef TYPE_S2_C6_FLOAT_DEC
	extern float S2_CUSTOM6;
	#endif
	#endif
#endif
#ifdef S2_CUSTOM7
	#ifdef TYPE_S2_C7
	extern TYPE_S2_C7 S2_CUSTOM7;
	#else
	#ifdef TYPE_S2_C7_FLOAT_DEC
	extern float S2_CUSTOM7;
	#endif
	#endif
#endif
#ifdef S2_CUSTOM8
	#ifdef TYPE_S2_C8
	extern TYPE_S2_C8 S2_CUSTOM8;
	#else
	#ifdef TYPE_S2_C8_FLOAT_DEC
	extern float S2_CUSTOM8;
	#endif
	#endif
#endif
#ifdef S2_CUSTOM9
	#ifdef TYPE_S2_C9
	extern TYPE_S2_C9 S2_CUSTOM9;
	#else
	#ifdef TYPE_S2_C9_FLOAT_DEC
	extern float S2_CUSTOM9;
	#endif
	#endif
#endif
#ifdef S2_CUSTOM10
	#ifdef TYPE_S2_C10
	extern TYPE_S2_C10 S2_CUSTOM10;
	#else
	#ifdef TYPE_S2_C10_FLOAT_DEC
	extern float S2_CUSTOM10;
	#endif
	#endif
#endif
#ifdef S2_CUSTOM11
	#ifdef TYPE_S2_C11
	extern TYPE_S2_C11 S2_CUSTOM11;
	#else
	#ifdef TYPE_S2_C11_FLOAT_DEC
	extern float S2_CUSTOM11;
	#endif
	#endif
#endif
#ifdef S2_CUSTOM12
	#ifdef TYPE_S2_C12
	extern TYPE_S2_C12 S2_CUSTOM12;
	#else
	#ifdef TYPE_S2_C12_FLOAT_DEC
	extern float S2_CUSTOM12;
	#endif
	#endif
#endif
#ifdef S2_CUSTOM13
	#ifdef TYPE_S2_C13
	extern TYPE_S2_C13 S2_CUSTOM13;
	#else
	#ifdef TYPE_S2_C13_FLOAT_DEC
	extern float S2_CUSTOM13;
	#endif
	#endif
#endif
#ifdef S2_BIT76
	#ifdef TYPE_S2_BIT76
	extern TYPE_S2_BIT76 S2_BIT76;
	#else
	#ifdef TYPE_S2_BIT76_FLOAT_DEC
	extern float S2_BIT76;
	#endif
	#endif
#endif
#ifdef S2_BIT5
	#ifdef TYPE_S2_BIT5
	extern TYPE_S2_BIT5 S2_BIT5;
	#else
	#ifdef TYPE_S2_BIT5_FLOAT_DEC
	extern float S2_BIT5;
	#endif
	#endif
#endif
#ifdef S2_BIT4
	#ifdef TYPE_S2_BIT4
	extern TYPE_S2_BIT4 S2_BIT4;
	#else
	#ifdef TYPE_S2_BIT4_FLOAT_DEC
	extern float S2_BIT4;
	#endif
	#endif
#endif
#ifdef S2_BIT3
	#ifdef TYPE_S2_BIT3
	extern TYPE_S2_BIT3 S2_BIT3;
	#else
	#ifdef TYPE_S2_BIT3_FLOAT_DEC
	extern float S2_BIT3;
	#endif
	#endif
#endif
#ifdef S2_BIT2
	#ifdef TYPE_S2_BIT2
	extern TYPE_S2_BIT2 S2_BIT2;
	#else
	#ifdef TYPE_S2_BIT2_FLOAT_DEC
	extern float S2_BIT2;
	#endif
	#endif
#endif
#ifdef S2_BIT1
	#ifdef TYPE_S2_BIT1
	extern TYPE_S2_BIT1 S2_BIT1;
	#else
	#ifdef TYPE_S2_BIT1_FLOAT_DEC
	extern float S2_BIT1;
	#endif
	#endif
#endif
#ifdef S2_BIT0
	#ifdef TYPE_S2_BIT0
	extern TYPE_S2_BIT0 S2_BIT0;
	#else
	#ifdef TYPE_S2_BIT0_FLOAT_DEC
	extern float S2_BIT0;
	#endif
	#endif
#endif

//接受函数声明
#ifdef R1_CUSTOM1
	#ifdef TYPE1_R1
	extern TYPE1_R1 R1_CUSTOM1;
	#else
	#ifdef TYPE1_R1_FLOAT_DEC
	extern float R1_CUSTOM1;
	#endif
	#endif
#endif
#ifdef R1_CUSTOM2
	#ifdef TYPE1_R2
	extern TYPE1_R2 R1_CUSTOM2;
	#else
	#ifdef TYPE1_R2_FLOAT_DEC
	extern float R1_CUSTOM2;
	#endif
	#endif
#endif
#ifdef R1_CUSTOM3
	#ifdef TYPE1_R3
	extern TYPE1_R3 R1_CUSTOM3;
	#else
	#ifdef TYPE1_R3_FLOAT_DEC
	extern float R1_CUSTOM3;
	#endif
	#endif
#endif
#ifdef R1_CUSTOM4
	#ifdef TYPE1_R4
	extern TYPE1_R4 R1_CUSTOM4;
	#else
	#ifdef TYPE1_R4_FLOAT_DEC
	extern float R1_CUSTOM4;
	#endif
	#endif
#endif
#ifdef R1_CUSTOM5
	#ifdef TYPE1_R5
	extern TYPE1_R5 R1_CUSTOM5;
	#else
	#ifdef TYPE1_R5_FLOAT_DEC
	extern float R1_CUSTOM5;
	#endif
	#endif
#endif
#ifdef R1_CUSTOM6
	#ifdef TYPE1_R6
	extern TYPE1_R6 R1_CUSTOM6;
	#else
	#ifdef TYPE1_R6_FLOAT_DEC
	extern float R1_CUSTOM6;
	#endif
	#endif
#endif
#ifdef R1_CUSTOM7
	#ifdef TYPE1_R7
	extern TYPE1_R7 R1_CUSTOM7;
	#else
	#ifdef TYPE1_R7_FLOAT_DEC
	extern float R1_CUSTOM7;
	#endif
	#endif
#endif

#ifdef R2_CUSTOM1
	#ifdef TYPE2_R1
	extern TYPE2_R1 R2_CUSTOM1;
	#else
	#ifdef TYPE2_R1_FLOAT_DEC
	extern float R2_CUSTOM1;
	#endif
	#endif
#endif
#ifdef R2_CUSTOM2
	#ifdef TYPE2_R2
	extern TYPE2_R2 R2_CUSTOM2;
	#else
	#ifdef TYPE2_R2_FLOAT_DEC
	extern float R2_CUSTOM2;
	#endif
	#endif
#endif
#ifdef R2_CUSTOM3
	#ifdef TYPE2_R3
	extern TYPE2_R3 R2_CUSTOM3;
	#else
	#ifdef TYPE2_R3_FLOAT_DEC
	extern float R2_CUSTOM3;
	#endif
	#endif
#endif
#ifdef R2_CUSTOM4
	#ifdef TYPE2_R4
	extern TYPE2_R4 R2_CUSTOM4;
	#else
	#ifdef TYPE2_R4_FLOAT_DEC
	extern float R2_CUSTOM4;
	#endif
	#endif
#endif
#ifdef R2_CUSTOM5
	#ifdef TYPE2_R5
	extern TYPE2_R5 R2_CUSTOM5;
	#else
	#ifdef TYPE2_R5_FLOAT_DEC
	extern float R2_CUSTOM5;
	#endif
	#endif
#endif
#ifdef R2_CUSTOM6
	#ifdef TYPE2_R6
	extern TYPE2_R6 R2_CUSTOM6;
	#else
	#ifdef TYPE2_R6_FLOAT_DEC
	extern float R2_CUSTOM6;
	#endif
	#endif
#endif
#ifdef R2_CUSTOM7
	#ifdef TYPE2_R7
	extern TYPE2_R7 R2_CUSTOM7;
	#else
	#ifdef TYPE2_R7_FLOAT_DEC
	extern float R2_CUSTOM7;
	#endif
	#endif
#endif

#ifdef R3_CUSTOM1
	#ifdef TYPE3_R1
	extern TYPE3_R1 R3_CUSTOM1;
	#else
	#ifdef TYPE3_R1_FLOAT_DEC
	extern float R3_CUSTOM1;
	#endif
	#endif
#endif
#ifdef R3_CUSTOM2
	#ifdef TYPE3_R2
	extern TYPE3_R2 R3_CUSTOM2;
	#else
	#ifdef TYPE3_R2_FLOAT_DEC
	extern float R3_CUSTOM2;
	#endif
	#endif
#endif
#ifdef R3_CUSTOM3
	#ifdef TYPE3_R3
	extern TYPE3_R3 R3_CUSTOM3;
	#else
	#ifdef TYPE3_R3_FLOAT_DEC
	extern float R3_CUSTOM3;
	#endif
	#endif
#endif
#ifdef R3_CUSTOM4
	#ifdef TYPE3_R4
	extern TYPE3_R4 R3_CUSTOM4;
	#else
	#ifdef TYPE3_R4_FLOAT_DEC
	extern float R3_CUSTOM4;
	#endif
	#endif
#endif
#ifdef R3_CUSTOM5
	#ifdef TYPE3_R5
	extern TYPE3_R5 R3_CUSTOM5;
	#else
	#ifdef TYPE3_R5_FLOAT_DEC
	extern float R3_CUSTOM5;
	#endif
	#endif
#endif
#ifdef R3_CUSTOM6
	#ifdef TYPE3_R6
	extern TYPE3_R6 R3_CUSTOM6;
	#else
	#ifdef TYPE3_R6_FLOAT_DEC
	extern float R3_CUSTOM6;
	#endif
	#endif
#endif
#ifdef R3_CUSTOM7
	#ifdef TYPE3_R7
	extern TYPE3_R7 R3_CUSTOM7;
	#else
	#ifdef TYPE3_R7_FLOAT_DEC
	extern float R3_CUSTOM7;
	#endif
	#endif
#endif

#ifdef R4_CUSTOM1
	#ifdef TYPE4_R1
	extern TYPE4_R1 R4_CUSTOM1;
	#else
	#ifdef TYPE4_R1_FLOAT_DEC
	extern float R4_CUSTOM1;
	#endif
	#endif
#endif
#ifdef R4_CUSTOM2
	#ifdef TYPE4_R2
	extern TYPE4_R2 R4_CUSTOM2;
	#else
	#ifdef TYPE4_R2_FLOAT_DEC
	extern float R4_CUSTOM2;
	#endif
	#endif
#endif
#ifdef R4_CUSTOM3
	#ifdef TYPE4_R3
	extern TYPE4_R3 R4_CUSTOM3;
	#else
	#ifdef TYPE4_R3_FLOAT_DEC
	extern float R4_CUSTOM3;
	#endif
	#endif
#endif
#ifdef R4_CUSTOM4
	#ifdef TYPE4_R4
	extern TYPE4_R4 R4_CUSTOM4;
	#else
	#ifdef TYPE4_R4_FLOAT_DEC
	extern float R4_CUSTOM4;
	#endif
	#endif
#endif
#ifdef R4_CUSTOM5
	#ifdef TYPE4_R5
	extern TYPE4_R5 R4_CUSTOM5;
	#else
	#ifdef TYPE4_R5_FLOAT_DEC
	extern float R4_CUSTOM5;
	#endif
	#endif
#endif
#ifdef R4_CUSTOM6
	#ifdef TYPE4_R6
	extern TYPE4_R6 R4_CUSTOM6;
	#else
	#ifdef TYPE4_R6_FLOAT_DEC
	extern float R4_CUSTOM6;
	#endif
	#endif
#endif
#ifdef R4_CUSTOM7
	#ifdef TYPE4_R7
	extern TYPE4_R7 R4_CUSTOM7;
	#else
	#ifdef TYPE4_R7_FLOAT_DEC
	extern float R4_CUSTOM7;
	#endif
	#endif
#endif

/****************** 外部函数声明 *****************************/
extern void f_COM_INIT;
extern void f_SEND_BYTE(LC_Uint8);
extern void COM_delay_ms(LC_Uint16);

#ifdef CMD01_f
void CMD01_f;
#endif

#ifdef CMD02_f
void CMD02_f;
#endif

#ifdef CMD03_f
void CMD03_f;
#endif

#ifdef CMD04_f
void CMD04_f;
#endif

/****************** 函数体部分 *****************************/
////////////////////// Message 相关 /////////////////////////
void Queue_byteInit(QUEUE_byte * This)  //消息初始化
{
  This->Front     = 0;
  This->Rear     = 0;
  This->FullFlag = 0;
}
void UpHead(QUEUE_byte *This)  //前移队首指针
{
  This->Front = (This->Front + 1) % NUM_MES_DATA;
}
void UpTail(QUEUE_byte *This)  //前移队尾指针
{
  This->Rear = (This->Rear + 1) % NUM_MES_DATA;
}

LC_Uint8 GetByteFromQueue(QUEUE_byte *This)
{
  LC_Uint8 GetData;
  if((This->Front==This->Rear)&&(!This->FullFlag))
  {
    return 0xff;   //如果队列为空，返回错误 ！*有待完善
  }
  else
  { 
    GetData = This->date[This->Rear];
    UpTail(This);
    This->FullFlag = 0;  //队列已满标识清零
    return GetData;
  }
}
LC_Uint16 GetUint16FromQueue(QUEUE_byte *This)
{  //上位机总是先发送高位
	LC_Uint16 result = GetByteFromQueue(This);
	result += GetByteFromQueue(This) * 256;
	return result;
}
float GetFloatFromQueue(QUEUE_byte *This)
{  //上位机总是先发送高位
#ifdef LC_IS_LITTLE_ENDIAN
	FloatBytesConverter.Bytes[0] = GetByteFromQueue(This);
	FloatBytesConverter.Bytes[1] = GetByteFromQueue(This);
	FloatBytesConverter.Bytes[2] = GetByteFromQueue(This);
	FloatBytesConverter.Bytes[3] = GetByteFromQueue(This);
	return FloatBytesConverter.Float;
#else
	FloatBytesConverter.Bytes[3] = GetByteFromQueue(This);
	FloatBytesConverter.Bytes[2] = GetByteFromQueue(This);
	FloatBytesConverter.Bytes[1] = GetByteFromQueue(This);
	FloatBytesConverter.Bytes[0] = GetByteFromQueue(This);
return FloatBytesConverter.Float;

#endif
}
void PutByteInQueue(QUEUE_byte *This,LC_Uint8 PutData)
{
  if(!This->FullFlag)
  { 
    This->date[This->Front] = PutData;    
    UpHead(This);
    if(This->Front==This->Rear)  //检查队列是否已满
    {
      This->FullFlag = 1;
    }
  }
}
LC_Uint8 Queue_byteCount(QUEUE_byte * This)
{
  if(This->Front<This->Rear)
  {
    return(This->Front + NUM_MES_DATA - This->Rear);
  }
  else
  {
    return(This->Front - This->Rear);
  }
}

/*****************下位机部分********************************/
void HIT_LC_Init(void)
{
  Queue_byteInit(pLC_FIFO_ByteBuffer);
  f_COM_INIT;
  EnableLCSend();
}
LC_Uint8 LC_Is_LittleEndian()
{
	LC_Uint16 usData = 0x1122;
	LC_Uint8* pucData = (LC_Uint8*)&usData;
	if(*pucData == 0x22)
		return LC_TRUE;
	else
		return LC_FALSE;
}
//////////////////串口发送程序/////////////////////////
void LC_SendUint16(LC_Uint16 data)
{
	f_SEND_BYTE((LC_Uint8)data);
	f_SEND_BYTE((LC_Uint8)(data>>8));
}
void LC_SendFloat(float data)
{
	FloatBytesConverter.Float = data;
#ifdef LC_IS_LITTLE_ENDIAN
	f_SEND_BYTE(FloatBytesConverter.Bytes[0]);
	f_SEND_BYTE(FloatBytesConverter.Bytes[1]);
	f_SEND_BYTE(FloatBytesConverter.Bytes[2]);
	f_SEND_BYTE(FloatBytesConverter.Bytes[3]);
#else
	f_SEND_BYTE(FloatBytesConverter.Bytes[3]);
	f_SEND_BYTE(FloatBytesConverter.Bytes[2]);
	f_SEND_BYTE(FloatBytesConverter.Bytes[1]);
	f_SEND_BYTE(FloatBytesConverter.Bytes[0]);
#endif
}
void LC_SendData1()
{
#ifdef S1_CUSTOM1
	#ifdef TYPE_S1_C1_FLOAT
		LC_SendFloat(S1_CUSTOM1);
	#else
		LC_SendUint16(S1_CUSTOM1);
	#endif
#endif
#ifdef S1_CUSTOM2
	#ifdef TYPE_S1_C2_FLOAT
		LC_SendFloat(S1_CUSTOM2);
	#else
		LC_SendUint16(S1_CUSTOM2);
	#endif
#endif
#ifdef S1_CUSTOM3
	#ifdef TYPE_S1_C3_FLOAT
		LC_SendFloat(S1_CUSTOM3);
	#else
		LC_SendUint16(S1_CUSTOM3);
	#endif
#endif
#ifdef S1_CUSTOM4
	#ifdef TYPE_S1_C4_FLOAT
		LC_SendFloat(S1_CUSTOM4);
	#else
		LC_SendUint16(S1_CUSTOM4);
	#endif
#endif
#ifdef S1_CUSTOM5
	#ifdef TYPE_S1_C5_FLOAT
		LC_SendFloat(S1_CUSTOM5);
	#else
		LC_SendUint16(S1_CUSTOM5);
	#endif
#endif
#ifdef S1_CUSTOM6
	#ifdef TYPE_S1_C6_FLOAT
		LC_SendFloat(S1_CUSTOM6);
	#else
		LC_SendUint16(S1_CUSTOM6);
	#endif
#endif
#ifdef S1_CUSTOM7
	#ifdef TYPE_S1_C7_FLOAT
		LC_SendFloat(S1_CUSTOM7);
	#else
		LC_SendUint16(S1_CUSTOM7);
	#endif
#endif
#ifdef S1_CUSTOM8
	#ifdef TYPE_S1_C8_FLOAT
		LC_SendFloat(S1_CUSTOM8);
	#else
		LC_SendUint16(S1_CUSTOM8);
	#endif
#endif
#ifdef S1_CUSTOM9
	#ifdef TYPE_S1_C9_FLOAT
		LC_SendFloat(S1_CUSTOM9);
	#else
		LC_SendUint16(S1_CUSTOM9);
	#endif
#endif
#ifdef S1_CUSTOM10
	#ifdef TYPE_S1_C10_FLOAT
		LC_SendFloat(S1_CUSTOM10);
	#else
		LC_SendUint16(S1_CUSTOM10);
	#endif
#endif
#ifdef S1_CUSTOM11
	#ifdef TYPE_S1_C11_FLOAT
		LC_SendFloat(S1_CUSTOM11);
	#else
		LC_SendUint16(S1_CUSTOM11);
	#endif
#endif
#ifdef S1_CUSTOM12
	#ifdef TYPE_S1_C12_FLOAT
		LC_SendFloat(S1_CUSTOM12);
	#else
		LC_SendUint16(S1_CUSTOM12);
	#endif
#endif
#ifdef S1_CUSTOM13
	#ifdef TYPE_S1_C13_FLOAT
		LC_SendFloat(S1_CUSTOM13);
	#else
		LC_SendUint16(S1_CUSTOM13);
	#endif
#endif

    LCMachineStatuse = 0;
#ifdef S1_BIT76
	LCMachineStatuse |= ((LC_Uint16)S1_BIT76&0x03)<<6;
#endif
#ifdef S1_BIT5
	LCMachineStatuse |= ((LC_Uint16)S1_BIT5&0x01)<<5;
#endif
#ifdef S1_BIT4
	LCMachineStatuse |= ((LC_Uint16)S1_BIT4&0x01)<<4;
#endif
#ifdef S1_BIT3
	LCMachineStatuse |= ((LC_Uint16)S1_BIT3&0x01)<<3;
#endif
#ifdef S1_BIT2
	LCMachineStatuse |= ((LC_Uint16)S1_BIT2&0x01)<<2;
#endif
#ifdef S1_BIT1
	LCMachineStatuse |= ((LC_Uint16)S1_BIT1&0x01)<<1;
#endif
#ifdef S1_BIT0
	LCMachineStatuse |= ((LC_Uint16)S1_BIT0&0x01)<<0;
#endif
	f_SEND_BYTE(LCMachineStatuse);
}
void LC_SendData2()
{
#ifdef S2_CUSTOM1
	#ifdef TYPE_S2_C1_FLOAT
		LC_SendFloat(S2_CUSTOM1);
	#else
		LC_SendUint16(S2_CUSTOM1);
	#endif
#endif
#ifdef S2_CUSTOM2
	#ifdef TYPE_S2_C2_FLOAT
		LC_SendFloat(S2_CUSTOM2);
	#else
		LC_SendUint16(S2_CUSTOM2);
	#endif
#endif
#ifdef S2_CUSTOM3
	#ifdef TYPE_S2_C3_FLOAT
		LC_SendFloat(S2_CUSTOM3);
	#else
		LC_SendUint16(S2_CUSTOM3);
	#endif
#endif
#ifdef S2_CUSTOM4
	#ifdef TYPE_S2_C4_FLOAT
		LC_SendFloat(S2_CUSTOM4);
	#else
		LC_SendUint16(S2_CUSTOM4);
	#endif
#endif
#ifdef S2_CUSTOM5
	#ifdef TYPE_S2_C5_FLOAT
		LC_SendFloat(S2_CUSTOM5);
	#else
		LC_SendUint16(S2_CUSTOM5);
	#endif
#endif
#ifdef S2_CUSTOM6
	#ifdef TYPE_S2_C6_FLOAT
		LC_SendFloat(S2_CUSTOM6);
	#else
		LC_SendUint16(S2_CUSTOM6);
	#endif
#endif
#ifdef S2_CUSTOM7
	#ifdef TYPE_S2_C7_FLOAT
		LC_SendFloat(S2_CUSTOM7);
	#else
		LC_SendUint16(S2_CUSTOM7);
	#endif
#endif
#ifdef S2_CUSTOM8
	#ifdef TYPE_S2_C8_FLOAT
		LC_SendFloat(S2_CUSTOM8);
	#else
		LC_SendUint16(S2_CUSTOM8);
	#endif
#endif
#ifdef S2_CUSTOM9
	#ifdef TYPE_S2_C9_FLOAT
		LC_SendFloat(S2_CUSTOM9);
	#else
		LC_SendUint16(S2_CUSTOM9);
	#endif
#endif
#ifdef S2_CUSTOM10
	#ifdef TYPE_S2_C10_FLOAT
		LC_SendFloat(S2_CUSTOM10);
	#else
		LC_SendUint16(S2_CUSTOM10);
	#endif
#endif
#ifdef S2_CUSTOM11
	#ifdef TYPE_S2_C11_FLOAT
		LC_SendFloat(S2_CUSTOM11);
	#else
		LC_SendUint16(S2_CUSTOM11);
	#endif
#endif
#ifdef S2_CUSTOM12
	#ifdef TYPE_S2_C12_FLOAT
		LC_SendFloat(S2_CUSTOM12);
	#else
		LC_SendUint16(S2_CUSTOM12);
	#endif
#endif
#ifdef S2_CUSTOM13
	#ifdef TYPE_S2_C13_FLOAT
		LC_SendFloat(S2_CUSTOM13);
	#else
		LC_SendUint16(S2_CUSTOM13);
	#endif
#endif

    LCMachineStatuse = 0;
#ifdef S2_BIT76
	LCMachineStatuse |= ((LC_Uint16)S2_BIT76&0x03)<<6;
#endif
#ifdef S2_BIT5
	LCMachineStatuse |= ((LC_Uint16)S2_BIT5&0x01)<<5;
#endif
#ifdef S2_BIT4
	LCMachineStatuse |= ((LC_Uint16)S2_BIT4&0x01)<<4;
#endif
#ifdef S2_BIT3
	LCMachineStatuse |= ((LC_Uint16)S2_BIT3&0x01)<<3;
#endif
#ifdef S2_BIT2
	LCMachineStatuse |= ((LC_Uint16)S2_BIT2&0x01)<<2;
#endif
#ifdef S2_BIT1
	LCMachineStatuse |= ((LC_Uint16)S2_BIT1&0x01)<<1;
#endif
#ifdef S2_BIT0
	LCMachineStatuse |= ((LC_Uint16)S2_BIT0&0x01)<<0;
#endif
	f_SEND_BYTE(LCMachineStatuse);
}
void LC_SendData(void)
{
  if(LC_TRUE==LC_SendEnable)
  {
	switch(LC_Num_SendEnable)
	{
		case 0:
			f_SEND_BYTE(LC_S_NO1);
			f_SEND_BYTE(LC_S_NO2);
			f_SEND_BYTE(LC_S_DATA1);
			LC_SendData1();
			break;
		case 1:
			f_SEND_BYTE(LC_S_NO1);
			f_SEND_BYTE(LC_S_NO2);
			f_SEND_BYTE(LC_S_DATA2);
			LC_SendData2();
			break;			
	}
  }
}

void LC_SendDataReceived1(void)
{
#ifdef R1_CUSTOM1
	#ifdef TYPE1_R1_FLOAT
	LC_SendFloat(R1_CUSTOM1);
	#else
	LC_SendUint16(R1_CUSTOM1);
	#endif
#endif
#ifdef R1_CUSTOM2
	#ifdef TYPE1_R2_FLOAT
	LC_SendFloat(R1_CUSTOM2);
	#else
	LC_SendUint16(R1_CUSTOM2);
	#endif
#endif
#ifdef R1_CUSTOM3
	#ifdef TYPE1_R3_FLOAT
	LC_SendFloat(R1_CUSTOM3);
	#else
	LC_SendUint16(R1_CUSTOM3);
	#endif
#endif
#ifdef R1_CUSTOM4
	#ifdef TYPE1_R4_FLOAT
	LC_SendFloat(R1_CUSTOM4);
	#else
	LC_SendUint16(R1_CUSTOM4);
	#endif
#endif
#ifdef R1_CUSTOM5
	#ifdef TYPE1_R5_FLOAT
	LC_SendFloat(R1_CUSTOM5);
	#else
	LC_SendUint16(R1_CUSTOM5);
	#endif
#endif
#ifdef R1_CUSTOM6
	#ifdef TYPE1_R6_FLOAT
	LC_SendFloat(R1_CUSTOM6);
	#else
	LC_SendUint16(R1_CUSTOM6);
	#endif
#endif
#ifdef R1_CUSTOM7
	#ifdef TYPE1_R7_FLOAT
	LC_SendFloat(R1_CUSTOM7);
	#else
	LC_SendUint16(R1_CUSTOM7);
	#endif
#endif
}
void LC_SendDataReceived2(void)
{
#ifdef R2_CUSTOM1
	#ifdef TYPE2_R1_FLOAT
	LC_SendFloat(R2_CUSTOM1);
	#else
	LC_SendUint16(R2_CUSTOM1);
	#endif
#endif
#ifdef R2_CUSTOM2
	#ifdef TYPE2_R2_FLOAT
	LC_SendFloat(R2_CUSTOM2);
	#else
	LC_SendUint16(R2_CUSTOM2);
	#endif
#endif
#ifdef R2_CUSTOM3
	#ifdef TYPE2_R3_FLOAT
	LC_SendFloat(R2_CUSTOM3);
	#else
	LC_SendUint16(R2_CUSTOM3);
	#endif
#endif
#ifdef R2_CUSTOM4
	#ifdef TYPE2_R4_FLOAT
	LC_SendFloat(R2_CUSTOM4);
	#else
	LC_SendUint16(R2_CUSTOM4);
	#endif
#endif
#ifdef R2_CUSTOM5
	#ifdef TYPE2_R5_FLOAT
	LC_SendFloat(R2_CUSTOM5);
	#else
	LC_SendUint16(R2_CUSTOM5);
	#endif
#endif
#ifdef R2_CUSTOM6
	#ifdef TYPE2_R6_FLOAT
	LC_SendFloat(R2_CUSTOM6);
	#else
	LC_SendUint16(R2_CUSTOM6);
	#endif
#endif
#ifdef R2_CUSTOM7
	#ifdef TYPE2_R7_FLOAT
	LC_SendFloat(R2_CUSTOM7);
	#else
	LC_SendUint16(R2_CUSTOM7);
	#endif
#endif
}
void LC_SendDataReceived3(void)
{
#ifdef R3_CUSTOM1
	#ifdef TYPE3_R1_FLOAT
	LC_SendFloat(R3_CUSTOM1);
	#else
	LC_SendUint16(R3_CUSTOM1);
	#endif
#endif
#ifdef R3_CUSTOM2
	#ifdef TYPE3_R2_FLOAT
	LC_SendFloat(R3_CUSTOM2);
	#else
	LC_SendUint16(R3_CUSTOM2);
	#endif
#endif
#ifdef R3_CUSTOM3
	#ifdef TYPE3_R3_FLOAT
	LC_SendFloat(R3_CUSTOM3);
	#else
	LC_SendUint16(R3_CUSTOM3);
	#endif
#endif
#ifdef R3_CUSTOM4
	#ifdef TYPE3_R4_FLOAT
	LC_SendFloat(R3_CUSTOM4);
	#else
	LC_SendUint16(R3_CUSTOM4);
	#endif
#endif
#ifdef R3_CUSTOM5
	#ifdef TYPE3_R5_FLOAT
	LC_SendFloat(R3_CUSTOM5);
	#else
	LC_SendUint16(R3_CUSTOM5);
	#endif
#endif
#ifdef R3_CUSTOM6
	#ifdef TYPE3_R6_FLOAT
	LC_SendFloat(R3_CUSTOM6);
	#else
	LC_SendUint16(R3_CUSTOM6);
	#endif
#endif
#ifdef R3_CUSTOM7
	#ifdef TYPE3_R7_FLOAT
	LC_SendFloat(R3_CUSTOM7);
	#else
	LC_SendUint16(R3_CUSTOM7);
	#endif
#endif
}
void LC_SendDataReceived4(void)
{
#ifdef R4_CUSTOM1
	#ifdef TYPE4_R1_FLOAT
	LC_SendFloat(R4_CUSTOM1);
	#else
	LC_SendUint16(R4_CUSTOM1);
	#endif
#endif
#ifdef R4_CUSTOM2
	#ifdef TYPE4_R2_FLOAT
	LC_SendFloat(R4_CUSTOM2);
	#else
	LC_SendUint16(R4_CUSTOM2);
	#endif
#endif
#ifdef R4_CUSTOM3
	#ifdef TYPE4_R3_FLOAT
	LC_SendFloat(R4_CUSTOM3);
	#else
	LC_SendUint16(R4_CUSTOM3);
	#endif
#endif
#ifdef R4_CUSTOM4
	#ifdef TYPE4_R4_FLOAT
	LC_SendFloat(R4_CUSTOM4);
	#else
	LC_SendUint16(R4_CUSTOM4);
	#endif
#endif
#ifdef R4_CUSTOM5
	#ifdef TYPE4_R5_FLOAT
	LC_SendFloat(R4_CUSTOM5);
	#else
	LC_SendUint16(R4_CUSTOM5);
	#endif
#endif
#ifdef R4_CUSTOM6
	#ifdef TYPE4_R6_FLOAT
	LC_SendFloat(R4_CUSTOM6);
	#else
	LC_SendUint16(R4_CUSTOM6);
	#endif
#endif
#ifdef R4_CUSTOM7
	#ifdef TYPE4_R7_FLOAT
	LC_SendFloat(R4_CUSTOM7);
	#else
	LC_SendUint16(R4_CUSTOM7);
	#endif
#endif
}
void LC_SendCMD(LC_Uint8 const cmd,LC_Uint8 const var)
{
	f_SEND_BYTE(LC_S_NO1);
	f_SEND_BYTE(LC_S_NO2);
	f_SEND_BYTE(LC_S_CMD);
	f_SEND_BYTE(cmd);
	f_SEND_BYTE(var);
}
/////////////////////串口接收函数////////////////////////
void LC_PutByteInFIFObuffer(LC_Uint8 LC_byte)
{
	PutByteInQueue(pLC_FIFO_ByteBuffer,LC_byte);
}
void LC_SetCarData(void)
{
#ifdef R1_CUSTOM1
	#ifdef TYPE1_R1_FLOAT
		R1_CUSTOM1 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R1_CUSTOM1 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R1_CUSTOM2
	#ifdef TYPE1_R2_FLOAT
		R1_CUSTOM2 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R1_CUSTOM2 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R1_CUSTOM3
	#ifdef TYPE1_R3_FLOAT
		R1_CUSTOM3 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R1_CUSTOM3 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R1_CUSTOM4
	#ifdef TYPE1_R4_FLOAT
		R1_CUSTOM4 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R1_CUSTOM4 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R1_CUSTOM5
	#ifdef TYPE1_R5_FLOAT
		R1_CUSTOM5 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R1_CUSTOM5 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R1_CUSTOM6
	#ifdef TYPE1_R6_FLOAT
		R1_CUSTOM6 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R1_CUSTOM6 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R1_CUSTOM7
	#ifdef TYPE1_R7_FLOAT
		R1_CUSTOM7 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R1_CUSTOM7 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
}
void LC_SetCarData2(void)
{
#ifdef R2_CUSTOM1
	#ifdef TYPE2_R1_FLOAT
		R2_CUSTOM1 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R2_CUSTOM1 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R2_CUSTOM2
	#ifdef TYPE2_R2_FLOAT
		R2_CUSTOM2 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R2_CUSTOM2 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R2_CUSTOM3
	#ifdef TYPE2_R3_FLOAT
		R2_CUSTOM3 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R2_CUSTOM3 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R2_CUSTOM4
	#ifdef TYPE2_R4_FLOAT
		R2_CUSTOM4 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R2_CUSTOM4 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R2_CUSTOM5
	#ifdef TYPE2_R5_FLOAT
		R2_CUSTOM5 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R2_CUSTOM5 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R2_CUSTOM6
	#ifdef TYPE2_R6_FLOAT
		R2_CUSTOM6 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R2_CUSTOM6 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R2_CUSTOM7
	#ifdef TYPE2_R7_FLOAT
		R2_CUSTOM7 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R2_CUSTOM7 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
}
void LC_SetCarData3(void)
{
#ifdef R3_CUSTOM1
	#ifdef TYPE3_R1_FLOAT
		R3_CUSTOM1 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R3_CUSTOM1 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R3_CUSTOM2
	#ifdef TYPE3_R2_FLOAT
		R3_CUSTOM2 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R3_CUSTOM2 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R3_CUSTOM3
	#ifdef TYPE3_R3_FLOAT
		R3_CUSTOM3 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R3_CUSTOM3 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R3_CUSTOM4
	#ifdef TYPE3_R4_FLOAT
		R3_CUSTOM4 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R3_CUSTOM4 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R3_CUSTOM5
	#ifdef TYPE3_R5_FLOAT
		R3_CUSTOM5 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R3_CUSTOM5 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R3_CUSTOM6
	#ifdef TYPE3_R6_FLOAT
		R3_CUSTOM6 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R3_CUSTOM6 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R3_CUSTOM7
	#ifdef TYPE3_R7_FLOAT
		R3_CUSTOM7 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R3_CUSTOM7 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
}
void LC_SetCarData4(void)
{
#ifdef R4_CUSTOM1
	#ifdef TYPE4_R1_FLOAT
		R4_CUSTOM1 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R4_CUSTOM1 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R4_CUSTOM2
	#ifdef TYPE4_R2_FLOAT
		R4_CUSTOM2 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R4_CUSTOM2 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R4_CUSTOM3
	#ifdef TYPE4_R3_FLOAT
		R4_CUSTOM3 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R4_CUSTOM3 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R4_CUSTOM4
	#ifdef TYPE4_R4_FLOAT
		R4_CUSTOM4 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R4_CUSTOM4 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R4_CUSTOM5
	#ifdef TYPE4_R5_FLOAT
		R4_CUSTOM5 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R4_CUSTOM5 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R4_CUSTOM6
	#ifdef TYPE4_R6_FLOAT
		R4_CUSTOM6 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R4_CUSTOM6 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
#ifdef R4_CUSTOM7
	#ifdef TYPE4_R7_FLOAT
		R4_CUSTOM7 = GetFloatFromQueue(pLC_FIFO_ByteBuffer);
	#else
		R4_CUSTOM7 = GetUint16FromQueue(pLC_FIFO_ByteBuffer);
	#endif
#endif
}
void LC_ReceivedData(void)
{
	LC_Uint8 tempu8;
	static LC_Uint8 CheckResult = LC_CHECK_RESULT_NONE;
	if(LC_CHECK_RESULT_NONE == CheckResult
		&& Queue_byteCount(pLC_FIFO_ByteBuffer)>=3)
	{
		if(GetByteFromQueue(pLC_FIFO_ByteBuffer) != LC_R_NO1)
			return;
		if(GetByteFromQueue(pLC_FIFO_ByteBuffer) != LC_R_NO2)
			return;
		tempu8 = GetByteFromQueue(pLC_FIFO_ByteBuffer);
		switch(tempu8)
		{
		case LC_R_GROUP1:
			CheckResult = LC_CHECK_RESULT_R_GROUP1;
			break;
		case LC_R_GROUP2:
			CheckResult = LC_CHECK_RESULT_R_GROUP2;
			break;
		case LC_R_GROUP3:
			CheckResult = LC_CHECK_RESULT_R_GROUP3;
			break;
		case LC_R_GROUP4:
			CheckResult = LC_CHECK_RESULT_R_GROUP4;
			break;
		case LC_R_CMD:
			CheckResult = LC_CHECK_RESULY_CMD;
			break;
		default:
			CheckResult = LC_CHECK_RESULT_NONE;
		}
	}

	if(LC_CHECK_RESULT_R_GROUP1 == CheckResult && Queue_byteCount(pLC_FIFO_ByteBuffer) >= NUM_R1_BYTE)
	{
		CheckResult = LC_CHECK_RESULT_NONE;
		LC_SetCarData();
		////延时后开启数据发送////
		COM_delay_ms(45);
		LC_SendEnable = LC_TRUE;	  
	}
	else if(LC_CHECK_RESULT_R_GROUP2 == CheckResult && Queue_byteCount(pLC_FIFO_ByteBuffer) >= NUM_R2_BYTE)
	{
		CheckResult = LC_CHECK_RESULT_NONE;
		LC_SetCarData2();
		////延时后开启数据发送////
		COM_delay_ms(45);
		LC_SendEnable = LC_TRUE;	  
	}
	else if(LC_CHECK_RESULT_R_GROUP3 == CheckResult && Queue_byteCount(pLC_FIFO_ByteBuffer) >= NUM_R3_BYTE)
	{
		CheckResult = LC_CHECK_RESULT_NONE;
		LC_SetCarData3();
		////延时后开启数据发送////
		COM_delay_ms(45);
		LC_SendEnable = LC_TRUE;	  
	}
	else if(LC_CHECK_RESULT_R_GROUP4 == CheckResult && Queue_byteCount(pLC_FIFO_ByteBuffer) >= NUM_R4_BYTE)
	{
		CheckResult = LC_CHECK_RESULT_NONE;
		LC_SetCarData4();
		////延时后开启数据发送////
		COM_delay_ms(45);
		LC_SendEnable = LC_TRUE;	  
	}
    else if(LC_CHECK_RESULY_CMD == CheckResult && Queue_byteCount(pLC_FIFO_ByteBuffer) >= NUM_R_CMD_BYTE)
    {
		const LC_Uint8 cmd = GetByteFromQueue(pLC_FIFO_ByteBuffer);
		const LC_Uint8 var = GetByteFromQueue(pLC_FIFO_ByteBuffer);
    	CheckResult = LC_CHECK_RESULT_NONE;
		if(CMD_STOP_SEND==cmd)
			LC_SendEnable = LC_FALSE;
		else if(CMD_START_SEND==cmd)
			LC_SendEnable = LC_TRUE; 
		else if(CMD_SEND_REC == cmd)
		{
			COM_delay_ms(40);
			f_SEND_BYTE(LC_S_NO1);
			f_SEND_BYTE(LC_S_NO2);
			switch(var)
			{
			case 1:
				f_SEND_BYTE(LC_S_REC_GROUP1);
				LC_SendDataReceived1();
				break;
			case 2:
				f_SEND_BYTE(LC_S_REC_GROUP2);
				LC_SendDataReceived2();
				break;
			case 3:
				f_SEND_BYTE(LC_S_REC_GROUP3);
				LC_SendDataReceived3();
				break;
			case 4:
				f_SEND_BYTE(LC_S_REC_GROUP4);
				LC_SendDataReceived4();
				break;
			}
			COM_delay_ms(50);
		}
		else if(LC_CMD_CHANGE_SEND == cmd)
			LC_Num_SendEnable = var;
		else if(LC_CMD01_DISLC == cmd)
			StopCarAndLCSned();
		else if(LC_CMD02_ENLC == cmd)
			StartLCSendAndRunCar();
		else if(LC_CMD_ENDIAN == cmd)
			LC_SendCMD(CMD_S_LC_ENDIAN,LC_Is_LittleEndian());
#ifdef CMD01_f
		else if(LC_CMD01 == cmd)  
      		CMD01_f;
#endif      
#ifdef CMD02_f
		else if(LC_CMD02 == cmd)
			CMD02_f;
#endif
#ifdef CMD03_f
			else if(LC_CMD03 == cmd)
				CMD03_f;
#endif
#ifdef CMD03_f
			else if(LC_CMD03 == cmd)
				CMD03_f;
#endif

    }
}

void EnableLCSend(void)
{
	LC_SendEnable = LC_TRUE;
}
void DisenableLCSend(void)
{
	LC_SendEnable = LC_FALSE;
}
void LC_SendOnce(void)
{
	EnableLCSend();
	LC_SendData();
	DisenableLCSend();
}
 
void StopCarAndLCSned(void)
{
#ifdef CMD01_f
	CMD01_f;
#endif
	DisenableLCSend();
}
void StartLCSendAndRunCar(void)
{
	EnableLCSend();
#ifdef CMD01_f
	CMD02_f;
#endif
}
