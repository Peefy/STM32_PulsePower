/*************************************************************
#include "LowerComputer_V2.h"
* 修改时间：2014年8月2日
*************************************************************/

#ifndef LIUSC_H
#define LIUSC_H


/****************** 结构体声明 *****************************/
#define NUM_MES_DATA 64
typedef struct QUEUE_byte 
{
  unsigned char date[NUM_MES_DATA];  //创建一个队列
  unsigned char Front;     //队列列头指针
  unsigned char Rear;     //队列列尾指针
  unsigned char FullFlag;
}QUEUE_byte;

/****************** 变量声明 *****************************/


/****************** 函数声明 *****************************/
void HIT_LC_Init(void);
void LC_SendData(void);
void LC_ReceivedData(void);
void LC_PutByteInFIFObuffer(unsigned char LC_byte);
void EnableLCSend(void);
void DisenableLCSend(void);
void LC_SendOnce(void);


#endif
