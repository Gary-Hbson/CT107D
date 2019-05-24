#include "drive.h"
#include "io.h"

code unsigned char tab[] = {
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,			//0-7	[0-7]
	0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00,0x40,	//8-f,black,-	[8-17]
	0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46,   //0.,...,9.,-1		[17-28]
//  H    J    K    L    N    o    P    U     t    G    Q    r   M    y
	0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e		//[29-42]
};      //,-,.

char distab[8] = {16, 16,16, 16, 16, 16, 16}; 
char discom = 0;                        
sKeyInfo KeyInfo={KEYUP,S7,NONE};

/**
  * @brief  控制led
  * @param  dat: 8个led状态，0x00-0xff
  * @retval None
  * @Note	一次控制所有led状态，高电平有效，0x00为熄灭
  */
void GroupLed(char dat)
{
	HC573(4,~dat);
}

/**
  * @brief  控制单个led
  * @param  num: 所要点亮led的序号，从左到右1-8
  * @param  dat: led状态，0或1
  * @retval LedStatus:现在所有led状态
  * @Note	一次控制一个led状态，之前led状态保留
  */
uint8_t SingleLed(char num,char dat)
{
	static uint8_t LedStatus=0x00;
	char status=0;
	if(dat==0)
	{
		status = (~(1<<(num-1)));
		LedStatus&=status;
	}
	else
	{
		status = (1<<(num-1));
		LedStatus|=status;
	}
	HC573(4,~LedStatus);
	return LedStatus;
}

/**
  * @brief  只点亮单个led
  * @param  num: 所要点亮led的序号，从左到有1-8
  * @retval None
  * @Note	输入0，关闭所有led
  */
void LedBit(char num)
{
    HC573(4, (char)~(0x01 << (num - 1)));
}

/**
  * @brief  数码管扫描函数
  * @param  None
  * @retval None
  * @Note	None
  */
void DisPlay(void)
{
    if (discom == 8)
    {
        discom = 0;
    }
    else
    {
        HC573(7, 0xff);					//关闭数码管段选
        HC573(6, 0x01 << (discom));		//数码管位选端口送入数据
        HC573(7, ~tab[distab[discom]]);	//开启数码管位选
        discom++;
    }
}

/**
  * @brief  清空数码管显示缓存，关闭数码管显示
  * @param  None
  * @retval None
  * @Note	None
  */
void clear()
{
    char i;
    for (i = 0; i < 8; i++)
        distab[i] = 16;
}

/**
  * @brief  数码管指定位显示字符
  * @param  num：	第几位数码管，从左至有0-7
  * @retval None
  * @Note	None
  */
void DisBit(char num, char dat)
{
    distab[7 - num] = dat;
}



/**
  * @brief  数码管显示数字
  * @param  num：	数码管要显示的数字
  * @retval None
  * @Note	None
  */
void DisNum(unsigned long int num)
{
	char i=0;
    clear();
	DisBit(0, 0);
	while(num!=0)
	{
		DisBit(i, num % 10);
		num/=10;
		i++;
	}
}

/**
  * @brief  数码管显示小数
  * @param  dat：数码管要显示的数字
  * @param  num：要显示数值的位数
  */
void DisNumFloat(float dat,char num)
{
	char i=0;
    clear();
	DisBit(0, 0);
	if(dat!=0)
	{
		uint16_t Dat_INT;
		float Dat_Float;
		
		Dat_INT=(uint16_t)dat/1.0;
		Dat_Float=dat-Dat_INT;
		
		DisBit(i+num, (Dat_INT %10)+18);
		Dat_INT=Dat_INT / 10;
		while(Dat_INT!=0)
		{
			DisBit(i+num+1, Dat_INT % 10);
			Dat_INT/=10;
			i++;
		}
		
	}
	
}

/**
  * @brief  按键扫描函数
  * @param  None
  * @retval
  * @Note	None
  */
char ScanKey(void)
{
    char col = 0x00;
    char KeyValue = 0xff;

    P3 = 0xf0;
    P42 = 1;
    P44 = 1;

    if ((P3 != 0xf0) || (P42 != 1) || (P44 != 1))
    {
        if (P44 == 0)
            col |= 0x01;
        if (P42 == 0)
            col |= 0x02;
        if ((P3 & 0x20) == 0)
            col |= 0x04;
        if ((P3 & 0x10) == 0)
            col |= 0x08;
		P3=0x0F;
        //P3 = 0x0B;
        P42 = 0;
        P44 = 0;

        if ((P3 & 0x01) == 0)
            col |=0x10;
        if ((P3 & 0x02) == 0)
            col |= 0x20;
        if ((P3 & 0x04) == 0)
            col |=0x40;
        if ((P3 & 0x08) == 0)
            col |= 0x80;
		
		KeyValue=col;
    }
	
	P3=0xff;
    return KeyValue;
}

/**
  * @brief  状态机按键键值读取及消抖
  * @param  None
  * @retval None
  * @Note	None
  */
void ReadKey(void)
{
	char value;
	static uint32_t TrigggerTime=0; 
	value=ScanKey();	//扫描按键

	//有按键按下
	if(value!=(char)0xff)
	{
		if(KeyInfo.KeyStatus==KEYUP)
		{
			KeyInfo.KeyStatus=KEYWAIT;
		}else 
		if(KeyInfo.KeyStatus==KEYWAIT)
		{
			TrigggerTime=millis();
			KeyInfo.KeyStatus=KEYDOWN;
			KeyInfo.Trigger=FAILLING;
		}else 
		if(KeyInfo.KeyStatus==KEYDOWN)
		{
			KeyInfo.KeyStatus=(millis()-TrigggerTime>800)?LONGPRESS:KEYDOWN;
		}
		KeyInfo.KeyValue=value;
	}
	else
	{
		if(KeyInfo.KeyStatus==KEYDOWN)
		{
			KeyInfo.KeyStatus=KEYWAIT;
		}
		else //if(KeyInfo.KeyStatus==KEYWAIT)
		{
			KeyInfo.KeyStatus=KEYUP;
			KeyInfo.Trigger=RISING;
		}

		
	}
}

/**
  * @brief  读取按键信息
  * @param  None
  * @retval None
  * @Note	None
  */
sKeyInfo ReadKeyValue(void)
{
	return KeyInfo;
}
