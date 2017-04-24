/*************************************************************
#include "LowerComputer_V2.h"
* �޸�ʱ�䣺2014��8��2��
*************************************************************/

#ifndef LIUSC_H
#define LIUSC_H


/****************** �ṹ������ *****************************/
#define NUM_MES_DATA 64
typedef struct QUEUE_byte 
{
  unsigned char date[NUM_MES_DATA];  //����һ������
  unsigned char Front;     //������ͷָ��
  unsigned char Rear;     //������βָ��
  unsigned char FullFlag;
}QUEUE_byte;

/****************** �������� *****************************/


/****************** �������� *****************************/
void HIT_LC_Init(void);
void LC_SendData(void);
void LC_ReceivedData(void);
void LC_PutByteInFIFObuffer(unsigned char LC_byte);
void EnableLCSend(void);
void DisenableLCSend(void);
void LC_SendOnce(void);


#endif
