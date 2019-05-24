#include "Ultrasonic.h"

void Delay11us()		//@11.0592MHz
{
	unsigned char i;
	_nop_();
	_nop_();
	i = 30;
	while (--i);
}

//TX引脚发送40KHz方波信号驱动超声波发送探头
//使用软件延时注意RC振荡器频率
void send_wave(void)
{
	unsigned char i = 8;  //发送8个脉冲
	do
	{
		TX = 1;
		Delay11us();
		TX = 0;
		Delay11us(); 
	}
	while(i--);
}

uint16_t distance()
{
	unsigned int distance,t;

    TMOD &= 0xF0;  //配置定时器工作模式
    TH0 = 0;
    TL0 = 0;  
  
    EA = 1;
    ET0 = 0;  //打开定时器0中断
    TR0 = 1;  //启动定时器   

	send_wave();  //发送方波信号
	TR0= 1;  //启动计时
	while((RX == 1) && (TF0 == 0));  //等待收到脉冲
		 TR0 = 0;  //关闭计时

	//发生溢出
	if(TF0 == 1)
	{
		TF0= 0;
		distance = 999;  //无返回
	}
	else
	{
		/**  计算时间  */
		t = (TH0<<8)|TL0;
		distance = (unsigned int)(t*0.017);  //计算距离				
		//distance = (unsigned int)(t*17/1000);  //计算距离				
	}
	TH0 = 0;
	TL0 = 0;
	return distance;
}