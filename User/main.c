#include "app.h" 
#include "time.h"
#include "1602.h"

void send_wave(void);
void main()
{
	ClsBuzz();
	SysTickInit();
	UartInit();
	EA=1;
	
	printf("System Init...\r\n");
	steup();

	while(1)
	{
		loop();
	}
}


void steup()
{
	DisNum(123);
	printf("1602 Init...\r\n");
	InitLcd1602();
	printf("1602 Init success...\r\n");
	DisNum(321);
	LcdShowStr(1,1,"hello");
}


void loop()
{
	static uint8_t num=0;
	if(KeyInfo.KeyValue==S6)
	{
		
	}else
	if(KeyInfo.KeyValue==S5)
	{
		
	}else
	if(KeyInfo.Trigger==FAILLING&&KeyInfo.KeyValue==S7)
	{
		KeyInfo.Trigger=NONE;
		DisNum(num++);
	}else
	if(KeyInfo.KeyStatus==FAILLING&&KeyInfo.KeyValue==(S4|S6))
	{
		static uint32_t TriggerTime=0;
	
		if(millis()>TriggerTime+200)
		{
			TriggerTime=millis();
			DisNum(0xff);
			DisBit(7,15);
		}
	}else
	if(KeyInfo.Trigger==FAILLING)
	{
		KeyInfo.Trigger=NONE;
		DisNum(KeyInfo.KeyValue);
	}
	if(KeyInfo.KeyStatus==LONGPRESS&&KeyInfo.KeyValue==S7)
	{
		static uint32_t TriggerTime=0;
	
		if(millis()>TriggerTime+200)
		{
			TriggerTime=millis();
			DisNum(num++);
		}
	}
}





