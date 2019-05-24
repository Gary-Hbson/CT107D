#ifndef __DS1302_H
#define __DS1302_H

extern unsigned char NowDateArray[7];

void Write_Ds1302(unsigned char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte( unsigned char address );

void Ds1302Init();
void Ds1302Read();
//  num:1  小时	  2 分    3 秒   ，dat 数据
void Ds1302Amend(char num, char dat);
#endif
