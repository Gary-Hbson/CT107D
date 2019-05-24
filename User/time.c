#include "io.h"

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5


uint16_t length=0;                       //存储信号的时间长度(count1 - count0)

void FreqInit()
{

	uint8_t temp;

	temp = P_SW1;
	temp &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
	temp |= CCP_S0;                  //(P3.4/ECI_2, P3.5/CCP0_2, P3.6/CCP1_2, P3.7/CCP2_2)
	P_SW1 = temp;  
	
	CCON = 0;                       //初始化PCA控制寄存器
                                    //PCA定时器停止
                                    //清除CF标志
                                    //清除模块中断标志
    CL = 0;                         //复位PCA寄存器
    CH = 0;
    CCAP0L = 0xff;
    CCAP0H = 0xff;
    CMOD = 0x06;                    //设置PCA时钟源为外部脉冲输入，关闭溢出中断
    CCAPM0 = 0x00;                  //PCA模块0为定时器模式，PCA计数器和比较捕获计数器相同时触发中断
	
	CR = 1;                         //PCA定时器开始工作
    EA = 1;
}
	

uint16_t ReadFreq()
{
	return length;
}

uint16_t Filter(uint16_t value) 
{
	#define N 8
	 
	static uint16_t value_buf[N];
	static uint16_t i=0;
	 

    uint16_t count;
    uint32_t sum=0;
    value_buf[i++] = value;
    if (i == N)
        i = 0;
    for (count=0;count<N;count++)
        sum += value_buf[count];
    return (uint16_t)(sum/N);
}

//定时器中断用户函数，1ms执行一次
void INTLoop()
{
	static uint32_t TriggerTime=0;
	
	if(millis()>TriggerTime+100)
	{
		uint16_t freq=0;
		
		TriggerTime=millis();
		
		length=(CL+CH*256)*10;                         //复位PCA寄存器
		length=Filter(length);
		CL = 0;                         //复位PCA寄存器
		CH = 0;
	}
}