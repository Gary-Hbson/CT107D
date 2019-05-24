#include "io.h"

void SysTickHandler() interrupt 12
{     
	SysTickCNT++;
	DisPlay();
	if(SysTickCNT%10==0)
	{
		ReadKey();
	}
	
	INTLoop();

}

void PCA_isr() interrupt 7 
{
	if (CCF0)
    {
		CCF0 = 0;
		P13=(CL<CCAP0L)?0:1;
		P12=~P12;
    }
	
}
