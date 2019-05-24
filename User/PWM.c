#include "io.h"

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5


void PWMInit(uint16_t freq)
{
	
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 65535-FOCS/256/freq;		//设置定时初值
	TH0 = (65535-(FOCS/256/freq))>>8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	
//	ACC = P_SW1;
//    ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
//    P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    
	
    CCON = 0;                       //初始化PCA控制寄存器
                                    //PCA定时器停止
                                    //清除CF标志
                                    //清除模块中断标志
    CL = 0;                         //复位PCA寄存器
    CH = 0;
    CMOD = 0x04;                    //设置PCA时钟源为定时器溢出中断
                                    //禁止PCA定时器溢出中断
	
    PCA_PWM0 = 0x00;                //PCA模块0工作于8位PWM
    CCAP0H = 0x80;
	CCAP0L = 0x80;         			//PWM0的默认输出占空比为50% 
    CCAPM0 = 0x73;                  //PCA模块0为8位PWM模式,上升下降沿触发

    CR = 1;                         //PCA定时器开始工作
	TR0 = 1;		//定时器0开始计时
	EA=1;
}

void SetDuty(uint8_t duty)
{
	CCAP0H = duty;
	CCAP0L = duty;
}

