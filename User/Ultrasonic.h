#ifndef Ultrasonic_H_
#define Ultrasonic_H_

#include "drive.h"

sbit TX=P1^0;
sbit RX=P1^1;

uint16_t distance();

#endif
