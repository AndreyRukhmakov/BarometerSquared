#include "stm32f10x_conf.h"
#include "Nokia1110Lcd.h"
#include "GPIO.h"
#include "barometer.h"

//----------------------------------------------------------------------------------------


void Delay_1ms(unsigned int ms)
{
    volatile unsigned int nCount;

    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq (&RCC_Clocks);

    nCount = (RCC_Clocks.HCLK_Frequency / 10000)*ms;
    for (; nCount != 0; nCount--);
}

//----------------------------------------------------------------------------------------


int main(void)
{
    positionType pos, pos1;
    unsigned char fontHeight;


    LcdGpioInit();
    LcdInit();
    LcdFrameBufferClear(black);

    SetCurrentFont(pFontH7xW5);
    SetFontClearance(1);
    SetSpaceLength(2);
    GetFontHeight(&fontHeight);

    BarometerReset();

    BarometerFillData(60000);
    BarometerFillData(70000);
    BarometerFillData(62000);
    BarometerFillData(64000);
    BarometerFillData(64000);
    BarometerFillData(65000);
    BarometerFillData(66000);
    BarometerFillData(67000);
    BarometerFillData(68000);
    BarometerFillData(69000);
    BarometerFillData(70000);
    BarometerFillData(71000);

    while(1)
    {
        Delay_1ms(1000);
        DrawBarometerField( 17, 15, white, black, white );
        LcdPrint();
    }

    return 1;
}

//----------------------------------------------------------------------------------------
