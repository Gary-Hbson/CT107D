#ifndef _IO_H_
#define _IO_H_

#include <STC15F2K60S2.H>
#include "intrins.h"
#include "stdio.h"

#include "drive.h"

//���尴��״̬ö�ٱ������ͣ����𣬰��£��ȴ�
typedef enum{KEYUP=0,KEYDOWN=1,KEYWAIT=2,LONGPRESS=3} eKeyStatus;
//���尴����ֵö�ٱ�������
typedef enum{   S7=0x11,S11=0x12,S15=0x14,S19=0x18,
                S6=0x21,S10=0x22,S14=0x24,S18=0x28,
                S5=0x41,S9=0x42,S13=0x44,S17=0x48,
                S4=0x81,S8=0x82,S12=0x84,S16=0x88} eKeyValue;
//          �����أ�  �½���
typedef enum {RISING=0,FAILLING,NONE}eTrigger;

//���尴����Ϣ�ṹ��
typedef struct
{
    eKeyStatus  KeyStatus;
    eKeyValue   KeyValue;
    eTrigger    Trigger;
}sKeyInfo;

extern sKeyInfo KeyInfo;

void LedBit(char num);
void GroupLed(char dat);
uint8_t SingleLed(char num,char dat);

void clear();
void DisPlay(void);
void DisBit(char num, char dat);
void DisNum(unsigned long int num);

char ScanKey(void);
void ReadKey(void);
sKeyInfo ReadKeyValue(void);


#endif



