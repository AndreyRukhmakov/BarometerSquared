#ifndef NOKIA1110LCD_H_INCLUDED
#define NOKIA1110LCD_H_INCLUDED

#include "fonts.h"
#include "pictures.h"

#define USING_LCD_EMULATOR

#define LCD_WIDTH		            84
#define LCD_HEIGHT		            48
#define LCD_MEMORY_SIZE_IN_BYTES	((LCD_WIDTH * LCD_HEIGHT) / 8)

#define REAL_X_RESOLUTION           84
#define REAL_Y_RESOLUTION           48

#define MOSI_ON      DISPLAY_PORT->ODR |= MOSI_PIN;
#define MOSI_OFF     DISPLAY_PORT->ODR &= ~MOSI_PIN;
#define SCLK_ON      DISPLAY_PORT->ODR |= SCLK_PIN;
#define SCLK_OFF     DISPLAY_PORT->ODR &= ~SCLK_PIN;
#define CS_OFF       DISPLAY_PORT->ODR |= CS_PIN;
#define CS_ON        DISPLAY_PORT->ODR &= ~CS_PIN;
#define DS_OFF       DISPLAY_PORT->ODR |= DS_PIN;
#define DS_ON        DISPLAY_PORT->ODR &= ~DS_PIN;
#define RST_OFF      DISPLAY_PORT->ODR |= RST_PIN;
#define RST_ON       DISPLAY_PORT->ODR &= ~RST_PIN;

typedef enum
{
    white = 0,
    black,
}colorType;

typedef struct
{
    unsigned int x;
    unsigned int y;
}positionType;

//----------------------------------------------------------------------------------------


void LcdInit(void);
void LcdFrameBufferClear(colorType color);
void DrawPixel(positionType pos, colorType color);

void DrawLine(positionType pos1, positionType pos2, colorType color);
void DrawRectangle(positionType pos1, positionType pos2, colorType color);
void DrawFilledRectangle(positionType pos1, positionType pos2, colorType color);
void DrawCircle(positionType position, unsigned char radius, colorType color);
void DrawFilledCircle(positionType position, int radius, colorType color);

void DrawPicture(positionType position, const unsigned char *picture, colorType color);
void PutStr(positionType pos, char *str, colorType color);
void SetCurrentFont(const unsigned char *font);
void SetFontClearance(unsigned char clearanceInPixel);
void SetSpaceLength(unsigned char spaceLengthInPixels);
void GetFontHeight(unsigned char *fontHeight);
void LcdPrint(void);


#endif /* NOKIA1110LCD_H_INCLUDED */
