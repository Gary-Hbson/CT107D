#ifndef _1602_H
#define _1602_H

#include "drive.h"

#define LCD1602_DB  P0

sbit LCD1602_RS = P2^0;
sbit LCD1602_RW = P2^1;
sbit LCD1602_E  = P1^2;

void InitLcd1602();
void LcdShowStr(unsigned char x, unsigned char y,unsigned char *str);

#endif
