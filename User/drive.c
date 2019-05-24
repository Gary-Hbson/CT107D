#include "drive.h"

uint32_t SysTickCNT=0;

/**
  * @brief  дHC573
  * @param  num: 573����Ӧ��ʹ�ܽ����
  *			����ܶ�ѡ��7	�����λѡ��6	led:4
  *			ULN2003:5
  * @param  dat: 573Ҫд�������
  * @retval None
  * @Note
  */
void HC573(char num, char dat)
{

    P2 = P2 & 0x1f;			//�����ַ�����ݣ��ر�573λѡʹ��
	P0 = dat;				//��������
    P2 = P2 | (num << 5);	//������ַ��ѡͨ
    P2 = P2 & 0x1f;			//�����ַ������
	//P0=0xff;
}

/**
  * @brief  �ر���������
  * @param  None
  * @retval None
  * @Note
  */
void ClsBuzz()
{
    HC573(4, 0xff);		//�ر�����led
    HC573(5, 0xa0);	//�رշ������̵�����0xA0?
    HC573(7, 0xff);	//�ر������
}

/**
  * @brief  ������ʱ����
  * @param  ms: ��ʱʱ��
  * @retval None
  * @Note	������ʱ
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
  * @brief  ��ʱ����ʼ������
  * @param  None
  * @retval None
  * @Note 1T,1ms,15F2K61S2
  * PWMʱ��Դ
  */
void SysTickInit(void)		//1����@11.0592MHz
{	
    AUXR |= 0x04;                   //��ʱ��2Ϊ1Tģʽ
	T2L = 0xCD;                //��ʼ����ʱֵ
    T2H = 0xD4;
    AUXR |= 0x10;                   //��ʱ��2��ʼ��ʱ
    
    IE2 |= 0x04;                    //����ʱ��2�ж�
    EA = 1;
}

uint32_t millis()
{
	return SysTickCNT;
}


/**
  * @brief  ���ڳ�ʼ��
  * @param  None
  * @retval None
  * @Note	None
  */
void UartInit(void)		//115200bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xE8;		//�趨��ʱ��ֵ
	TH1 = 0xFF;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	ES=0;
	EA=1;
}

/**
  * @brief  ��дPUTCHAR
  * @param  ������ַ�
  * @retval ���ش����ַ�
  * @Note	None
  */
char putchar(char c)
{
    SBUF=c;
    while(TI!=1);   //�ȴ����ͳɹ�
    TI=0;           //��������жϱ�־
    return c;
}

/**
  * @brief  ���ڷ����ַ���
  * @param  str Ҫ���͵��ַ���ָ��
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


