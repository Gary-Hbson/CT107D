#ifndef _IIC_H
#define _IIC_H

void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 

void operate_delay(unsigned char t);

void init_pcf8591(void);
unsigned char ReadADC(char channel);
void WriteDAC(char dat);
void WriteEEPROM(unsigned char add, unsigned char val);
unsigned char ReadEEPROM(unsigned char add);
void ReadEEPROMData(unsigned char add, unsigned char * val,unsigned int len);
void WriteEEPROMData(unsigned char add, unsigned char * val,unsigned int len);
#endif