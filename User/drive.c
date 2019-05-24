#include "drive.h"

uint32_t SysTickCNT=0;

/**
  * @brief  写HC573
  * @param  num: 573所对应的使能脚序号
  *			数码管段选：7	数码管位选：6	led:4
  *			ULN2003:5
  * @param  dat: 573要写入的数据
  * @retval None
  * @Note
  */
void HC573(char num, char dat)
{

    P2 = P2 & 0x1f;			//清除地址线数据，关闭573位选使能
	P0 = dat;				//送入数据
    P2 = P2 | (num << 5);	//开启地址线选通
    P2 = P2 & 0x1f;			//清除地址线数据
	//P0=0xff;
}

/**
  * @brief  关闭无用外设
  * @param  None
  * @retval None
  * @Note
  */
void ClsBuzz()
{
    HC573(4, 0xff);		//关闭所有led
    HC573(5, 0xa0);	//关闭蜂鸣器继电器，0xA0?
    HC573(7, 0xff);	//关闭数码管
}

/**
  * @brief  毫秒延时函数
  * @param  ms: 延时时间
  * @retval None
  * @Note	阻塞延时
  */
void delay(int ms)
{
    unsigned char i, j;

    while (ms--)
    {
        _nop_();
        _nop_();
        _nop_();
        i = 11;
        j = 190;
        do
        {
            while (--j)
                ;
        } while (--i);
    }
}

/**
  * @brief  定时器初始化函数
  * @param  None
  * @retval None
  * @Note 1T,1ms,15F2K61S2
  * PWM时钟源
  */
void SysTickInit(void)		//1毫秒@11.0592MHz
{	
    AUXR |= 0x04;                   //定时器2为1T模式
	T2L = 0xCD;                //初始化计时值
    T2H = 0xD4;
    AUXR |= 0x10;                   //定时器2开始计时
    
    IE2 |= 0x04;                    //开定时器2中断
    EA = 1;
}

uint32_t millis()
{
	return SysTickCNT;
}


/**
  * @brief  串口初始化
  * @param  None
  * @retval None
  * @Note	None
  */
void UartInit(void)		//115200bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xE8;		//设定定时初值
	TH1 = 0xFF;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	ES=0;
	EA=1;
}

/**
  * @brief  重写PUTCHAR
  * @param  传入的字符
  * @retval 返回传入字符
  * @Note	None
  */
char putchar(char c)
{
    SBUF=c;
    while(TI!=1);   //等待发送成功
    TI=0;           //清除发送中断标志
    return c;
}

/**
  * @brief  串口发送字符串
  * @param  str 要发送的字符串指针
  * @retval None
  * @Note	None
  */
void SendStr(unsigned char *str)
{
    unsigned char *p;

    p = str;
    while(*p != '\0')
    {
        SBUF = *p;
		while(TI == 0);
		TI = 0;
        p++;
    }
}


