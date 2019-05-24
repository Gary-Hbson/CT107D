#ifndef _DRIVE_H_
#define _DRIVE_H_

#include <STC15F2K60S2.H>
#include "intrins.h"
#include "stdio.h"

#define FOCS 12000000UL

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;

#define uint unsigned int
#define uchar unsigned char
typedef long int32_t;
typedef int int16_t;
typedef unsigned long uint32_t;
typedef unsigned int uint16_t;
typedef unsigned char uint8_t;

extern uint32_t SysTickCNT;

void steup();
void loop();

void ClsBuzz();
void SysTickInit(void);
void delay(int ms);
uint32_t millis();
void HC573(char num, char dat);

void UartInit(void);
void SendStr(unsigned char *str);
char putchar(char c);

void INTLoop();

#endif



