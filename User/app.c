#include "app.h"

void DisRunCount(void)
{
	if(ReadEEPROM(0x01)!=0)
	{
		WriteEEPROM(0x01,0);	//运行标志位
		WriteEEPROM(0x00,1);	//运行次数
	}
	else
	{
		uint8_t RunCount;
		RunCount=ReadEEPROM(0x00);
		RunCount++;
		WriteEEPROM(0x00,RunCount);
		DisNum(RunCount);
	}
}

void Distemp()
{
	float temp;
	static uint32_t TriggerTime=0;
	
	if(millis()>TriggerTime+1000)
	{
		TriggerTime=millis();
		temp=ReadTempFloat();
		printf("Temperature is %2.2f\r\n",temp);
		DisNum((uint16_t)temp);
	}
}

void DisADC()
{
	uint8_t ADCvalue;
	static uint32_t TriggerTime=0;
	if(millis()>TriggerTime+300)
	{
		TriggerTime=millis();
		ADCvalue= ReadADC(1); 
		printf("ADC Value is %d\r\n",(uint16_t)ADCvalue);
		DisNum(ADCvalue);
	}
	
}

void DisKeuValue()
{
	DisNum(KeyInfo.KeyValue);
	printf("Key Value is %d\r\n",(uint16_t)KeyInfo.KeyValue);
	
}

void DisTime()
{
	static uint32_t TriggerTime=0;
	
	if(millis()>TriggerTime+1000)
	{
		Ds1302Read();
		DisBit(2,17);
		DisBit(5,17);
		DisBit(0,NowDateArray[0]%10);
		DisBit(1,NowDateArray[0]/10);
		DisBit(3,NowDateArray[1]%10);
		DisBit(4,NowDateArray[1]/10);
		DisBit(6,NowDateArray[2]%10);
		DisBit(7,NowDateArray[2]/10);
	}
	
}

