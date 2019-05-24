#include "io.h"

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5


void PWMInit(uint16_t freq)
{
	
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 65535-FOCS/256/freq;		//���ö�ʱ��ֵ
	TH0 = (65535-(FOCS/256/freq))>>8;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	
//	ACC = P_SW1;
//    ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
//    P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    
	
    CCON = 0;                       //��ʼ��PCA���ƼĴ���
                                    //PCA��ʱ��ֹͣ
                                    //���CF��־
                                    //���ģ���жϱ�־
    CL = 0;                         //��λPCA�Ĵ���
    CH = 0;
    CMOD = 0x04;                    //����PCAʱ��ԴΪ��ʱ������ж�
                                    //��ֹPCA��ʱ������ж�
	
    PCA_PWM0 = 0x00;                //PCAģ��0������8λPWM
    CCAP0H = 0x80;
	CCAP0L = 0x80;         			//PWM0��Ĭ�����ռ�ձ�Ϊ50% 
    CCAPM0 = 0x73;                  //PCAģ��0Ϊ8λPWMģʽ,�����½��ش���

    CR = 1;                         //PCA��ʱ����ʼ����
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	EA=1;
}

void SetDuty(uint8_t duty)
{
	CCAP0H = duty;
	CCAP0L = duty;
}

