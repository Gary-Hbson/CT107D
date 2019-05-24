#include "Ultrasonic.h"

void Delay11us()		//@11.0592MHz
{
	unsigned char i;
	_nop_();
	_nop_();
	i = 30;
	while (--i);
}

//TX���ŷ���40KHz�����ź���������������̽ͷ
//ʹ�������ʱע��RC����Ƶ��
void send_wave(void)
{
	unsigned char i = 8;  //����8������
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

    TMOD &= 0xF0;  //���ö�ʱ������ģʽ
    TH0 = 0;
    TL0 = 0;  
  
    EA = 1;
    ET0 = 0;  //�򿪶�ʱ��0�ж�
    TR0 = 1;  //������ʱ��   

	send_wave();  //���ͷ����ź�
	TR0= 1;  //������ʱ
	while((RX == 1) && (TF0 == 0));  //�ȴ��յ�����
		 TR0 = 0;  //�رռ�ʱ

	//�������
	if(TF0 == 1)
	{
		TF0= 0;
		distance = 999;  //�޷���
	}
	else
	{
		/**  ����ʱ��  */
		t = (TH0<<8)|TL0;
		distance = (unsigned int)(t*0.017);  //�������				
		//distance = (unsigned int)(t*17/1000);  //�������				
	}
	TH0 = 0;
	TL0 = 0;
	return distance;
}