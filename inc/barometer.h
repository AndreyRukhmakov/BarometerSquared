#ifndef BAROMETER_H
#define BAROMETER_H

#include "Nokia1110Lcd.h"



void BarometerReset(void);
void BarometerFillData(unsigned long currentPressure);
unsigned char BarometerGetPressureDirection(void);

void DrawBarometerField( unsigned char x,
                         unsigned char y,
                         colorType backgroundColor,
                         colorType dataColor,
                         colorType blinkColor );


#endif
