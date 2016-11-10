#include <GPIO.h>
#include <stm32f10x_gpio.h>


GPIO_InitTypeDef GPIO_InitStructure;

//--------------------------------------------------------------------------------------


void LcdGpioInit(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = (RST_PIN | CS_PIN | MOSI_PIN | DS_PIN | SCLK_PIN);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(DISPLAY_PORT , &GPIO_InitStructure);
}

//--------------------------------------------------------------------------------------

