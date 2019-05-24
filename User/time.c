#include "io.h"

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5


uint16_t length=0;                       //�洢�źŵ�ʱ�䳤��(count1 - count0)

void FreqInit()
{

	uint8_t temp;

	temp = P_SW1;
	temp &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
	temp |= CCP_S0;                  //(P3.4/ECI_2, P3.5/CCP0_2, P3.6/CCP1_2, P3.7/CCP2_2)
	P_SW1 = temp;  
	
	CCON = 0;                       //��ʼ��PCA���ƼĴ���
                                    //PCA��ʱ��ֹͣ
                                    //���CF��־
                                    //���ģ���жϱ�־
    CL = 0;                         //��λPCA�Ĵ���
    CH = 0;
    CCAP0L = 0xff;
    CCAP0H = 0xff;
    CMOD = 0x06;                    //����PCAʱ��ԴΪ�ⲿ�������룬�ر�����ж�
    CCAPM0 = 0x00;                  //PCAģ��0Ϊ��ʱ��ģʽ��PCA�������ͱȽϲ����������ͬʱ�����ж�
	
	CR = 1;                         //PCA��ʱ����ʼ����
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

//��ʱ���ж��û�������1msִ��һ��
void INTLoop()
{
	static uint32_t TriggerTime=0;
	
	if(millis()>TriggerTime+100)
	{
		uint16_t freq=0;
		
		TriggerTime=millis();
		
		length=(CL+CH*256)*10;                         //��λPCA�Ĵ���
		length=Filter(length);
		CL = 0;                         //��λPCA�Ĵ���
		CH = 0;
	}
}