#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

#include <stm32f10x_gpio.h>

#define ON                  1
#define OFF                 0

#define DISPLAY_PORT    GPIOC
#define RST_PIN         GPIO_Pin_4
#define CS_PIN          GPIO_Pin_3
#define DS_PIN          GPIO_Pin_2
#define MOSI_PIN        GPIO_Pin_1
#define SCLK_PIN        GPIO_Pin_0


void LcdGpioInit(void);

#endif /* GPIO_H_INCLUDED */
