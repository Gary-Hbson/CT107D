#ifndef _TIME_H
#define _TIME_H

#include "io.h"

void FreqInit();
uint16_t ReadFreq();
uint16_t Filter(uint16_t value) ;
#endif