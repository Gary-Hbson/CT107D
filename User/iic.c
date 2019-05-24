/*
  程序说明: IIC总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051，12MHz
  日    期: 2011-8-9
*/

#include "reg52.h"
#include "intrins.h"

#define DELAY_TIME 5

#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}
//总线启动条件
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//总线停止条件
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//发送应答
void IIC_SendAck(bit ackbit)
{
    SCL = 0;
    SDA = ackbit;  					// 0：应答，1：非应答
    IIC_Delay(DELAY_TIME);
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SCL = 0; 
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//等待应答
bit IIC_WaitAck(void)
{
    bit ackbit;
	
    SCL  = 1;
    IIC_Delay(DELAY_TIME);
    ackbit = SDA;
    SCL = 0;
    IIC_Delay(DELAY_TIME);
    return ackbit;
}

//通过I2C总线发送数据
void IIC_SendByte(unsigned char byt)
{
    unsigned char i;

    for(i=0; i<8; i++)
    {
        SCL  = 0;
        IIC_Delay(DELAY_TIME);
        if(byt & 0x80) SDA  = 1;
        else SDA  = 0;
        IIC_Delay(DELAY_TIME);
        SCL = 1;
        byt <<= 1;
        IIC_Delay(DELAY_TIME);
    }
    SCL  = 0;  
}

//从I2C总线上接收数据
unsigned char IIC_RecByte(void)
{
    unsigned char i, da;
    for(i=0; i<8; i++)
    {   
    	SCL = 1;
	IIC_Delay(DELAY_TIME);
	da <<= 1;
	if(SDA) da |= 1;
	SCL = 0;
	IIC_Delay(DELAY_TIME);
    }
    return da;    
}

//驱动延时
void operate_delay(unsigned char t)
{
    unsigned char i;

    while (t--)
    {
        for (i = 0; i < 112; i++)
            ;
    }
}

//向eeprom写入数据
void WriteEEPROM(unsigned char add, unsigned char val)
{
    IIC_Start();
    IIC_SendByte(0xa0);
    IIC_WaitAck();
    IIC_SendByte(add);
    IIC_WaitAck();
    IIC_SendByte(val);
    IIC_WaitAck();
    IIC_Stop();
    operate_delay(10);
}

//读取EEPROM中的数据
unsigned char ReadEEPROM(unsigned char add)
{
    unsigned char da;

    IIC_Start();
    IIC_SendByte(0xa0);
    IIC_WaitAck();
    IIC_SendByte(add);
    IIC_WaitAck();
    IIC_Start();
    IIC_SendByte(0xa1);
    IIC_WaitAck();
    da = IIC_RecByte();
    IIC_SendAck(1);
    IIC_Stop();

    return da;
}

void WriteEEPROMData(unsigned char add, unsigned char * val,unsigned int len)
{
	int i;

	
	
	for(i=0;i<len;i++)
	{
		if((i==0)||((add & 0x07)==0))
		{
			IIC_Stop();
			IIC_Start();
			IIC_SendByte(0xa0);
			IIC_WaitAck();
			IIC_SendByte(add);
			IIC_WaitAck();
		}
		add++;
		IIC_SendByte(val[i]);
		IIC_WaitAck();
		operate_delay(1);
	}
    
    IIC_Stop();
}

void ReadEEPROMData(unsigned char add, unsigned char * val,unsigned int len)
{
	int i;
	IIC_Start();
    IIC_SendByte(0xa0);
	IIC_WaitAck();
    IIC_SendByte(add);
    IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0xa1);
    IIC_WaitAck();
	for (i = 0; i < len; i++)
	{
		val[i] = IIC_RecByte();
		
		if (i !=len - 1)
		{
			IIC_SendAck(0);	
		}
		else
		{
			IIC_SendAck(1);	
		}
	}
	 IIC_Stop();
}

void init_pcf8591(char channel)
{
    IIC_Start();
    IIC_SendByte(0x90);
    IIC_WaitAck();
    IIC_SendByte(channel); //ADC3
    IIC_WaitAck();
    IIC_Stop();
    operate_delay(10);
}

/**
  * @brief  读取ADC
  * @param  channel:	ADC通道
  * @retval AN1光敏电阻输入，AIN2运放输入，AN3	可调电阻输入
  */
unsigned char ReadADC(char channel) 
{
    unsigned char temp;
	init_pcf8591(channel);
	
    IIC_Start();
    IIC_SendByte(0x91);
    IIC_WaitAck();
    temp = IIC_RecByte();
    IIC_SendAck(1);
    IIC_Stop();

    return temp;
}

void WriteDAC(char dat)
{
//	unsigned char temp;
	
    IIC_Start();
    IIC_SendByte(0x90);
    IIC_WaitAck();
	IIC_SendByte(0x40);
	IIC_WaitAck();
    IIC_SendByte(dat);
	IIC_WaitAck();
    IIC_Stop();
}