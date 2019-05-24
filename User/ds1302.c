/*
  ����˵��: DS1302��������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨ 8051��12MHz
  ��    ��: 2011-8-9
*/

#include <reg52.h>
#include <intrins.h>

//�롢�֡�ʱ���ա��¡���
code unsigned char DateArray[7] = {50, 59, 23, 1, 1, 20, 12};
unsigned char NowDateArray[7] = {50, 59, 23, 1, 1, 20, 12};

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302��λ												

void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK=0;
		SDA=temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
	unsigned char num;
	num=(dat/10<<4)|(dat%10);	//ʮ����ת��Ϊʮ��������	
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
	address = address & 0xFE;
 	Write_Ds1302(address);	
 	Write_Ds1302(num);		
 	RST=0; 
}

unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
	unsigned char temp_l,temp_h;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;		_nop_();
 	SCK=0;		_nop_();
	SCK=1;		_nop_();
	SDA=0;		_nop_();
	SDA=1;		_nop_();
	temp_h=temp/16;
	temp_l=temp%16;
	temp=temp_h*10+temp_l;
	return (temp);			
}

void Ds1302Init()
{
    char i;
    char add = 0x80;
    Write_Ds1302_Byte(0x8e, 0x00);
    for (i = 0; i < 7; i++)
    {
        Write_Ds1302_Byte(add, DateArray[i]);
        add += 2;
    }
    Write_Ds1302_Byte(0x8e, 0x80);
}

void Ds1302Read()
{
    char i;
    char add = 0x81;
    Write_Ds1302_Byte(0x8e, 0x00);
    for (i = 0; i < 7; i++)
    {
        NowDateArray[i] = Read_Ds1302_Byte(add);
        add += 2;
    }
     Write_Ds1302_Byte(0x8e, 0x80);
}

//  num:1  Сʱ	  2 ��    3 ��   ��dat ����
void Ds1302Amend(char num, char dat)
{
    char add = 0x86;
    if (num == 1 || num == 2 || num == 3)
    {
         Write_Ds1302_Byte(0x8e, 0x00);
         Write_Ds1302_Byte(add - 2 * num, dat);
         Write_Ds1302_Byte(0x8e, 0x80);
    }
}