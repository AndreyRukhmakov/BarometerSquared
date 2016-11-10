#include "GPIO.h"
#include "string.h"
#include "Nokia1110Lcd.h"


static unsigned char lcdFrameBuffer[LCD_MEMORY_SIZE_IN_BYTES]; // 184 bytes for the current resolution
static unsigned char fontsClearance = 1;
const unsigned char *pCurrentFont = ((void *)0);
const fontParametersType *fontParameters;
static unsigned char emptyLinesCounter = 0;
static unsigned char spaceDetected = 0;
static unsigned char spaceLength = 2;
//----------------------------------------------------------------------


void Senddata(unsigned char data)
{
	unsigned char Mask = 0x80;

	for(; Mask; Mask >>= 1)
    {
		if(data & Mask)
		{
           MOSI_ON;
		}
		else
		{
           MOSI_OFF;
		}
		SCLK_ON;
		SCLK_OFF;
	}
}

//--------------------------------------------------------------------------------------


void LcdInit(void)
{
	RST_ON;
	RST_OFF;
	CS_ON;
	DS_ON;
	Senddata(0x21);     // extended mode
	Senddata(0xC1);
	Senddata(0x06);		// temperature data 4 - 7
	Senddata(0x13);		// Bias 0b0001 0xxx - contrast
	Senddata(0x20); 	// normal mode
	Senddata(0b1100);	// 0b1100 - normal mode
                        // 0b1101 - invert mode
		                // 0b1001 - black screen
                        // 0b1000 - white screen
    DS_OFF;
	CS_OFF;
}

//----------------------------------------------------------------------------------------


void LcdFrameBufferClear(colorType color)
{
    if( color == white)
    {
        memset(lcdFrameBuffer, 0, LCD_MEMORY_SIZE_IN_BYTES);
    }
    else
    {
        memset(lcdFrameBuffer, 255, LCD_MEMORY_SIZE_IN_BYTES);
    }

}

//---------------------------------------------------------------------------------------


void DrawPixel(positionType position, colorType color)
{
    unsigned short addr = (position.y >> 3) * LCD_WIDTH + position.x;
    unsigned char data = (1 << ( position.y & 0x07));

    if((position.x > LCD_WIDTH) || (position.y > LCD_HEIGHT))
    {
        return;
    }
    if(color)
    {
        lcdFrameBuffer[addr] |= data;
    }
    else
    {
        lcdFrameBuffer[addr] &= ~data;
    }
}

//----------------------------------------------------------------------------------------


void DrawLine(positionType pos1, positionType pos2, colorType color)
{
  signed short   dx, dy, sx, sy;
  positionType pos = { 0, 0 };
  unsigned short  mdx, mdy, l;

  dx = pos2.x - pos1.x;
  dy = pos2.y - pos1.y;

  if(dx >= 0)
  {
    mdx = dx;
    sx = 1;
  }
  else
  {
    mdx = pos1.x - pos2.x;
    sx = -1;
  }
  if(dy >= 0)
  {
    mdy = dy;
    sy = 1;
  }
  else
  {
    mdy = pos1.y - pos2.y;
    sy = -1;
  }
  pos.x = pos1.x;
  pos.y = pos1.y;

  if( mdx >= mdy)
  {
    l = mdx;
    while(l > 0)
    {
      if (dy > 0)
      {
        pos.y = pos1.y + mdy * (pos.x - pos1.x) / mdx;
      }
      else
      {
        pos.y = pos1.y - mdy * (pos.x - pos1.x) / mdx;
      }
      DrawPixel(pos, color);
      pos.x = pos.x + sx;
      l--;
     }
  }
  else
  {
    l = mdy;
    while(l > 0)
    {
      if(dy > 0)
      {
        pos.x = pos1.x + ((mdx * (pos.y - pos1.y)) / mdy);
      }
      else
      {
        pos.x = pos1.x + ((mdx * (pos1.y - pos.y)) / mdy);
      }
      DrawPixel(pos, color);
      pos.y = pos.y + sy;
      l--;
    }
  }
  DrawPixel(pos2, color);
}

//----------------------------------------------------------------------------------------


void DrawRectangle(positionType pos1, positionType pos2, colorType color)
{
  positionType position, position1;

  position.x = pos1.x;
  position.y = pos1.y;
  position1.x = pos2.x;
  position1.y = pos1.y;
  DrawLine(position, position1, color);

  position.x = pos1.x;
  position.y = pos2.y;
  position1.x = pos2.x;
  position1.y = pos2.y;
  DrawLine(position, position1, color);

  position.x = pos1.x;
  position.y = pos1.y;
  position1.x = pos1.x;
  position1.y = pos2.y;
  DrawLine(position, position1, color);

  position.x = pos2.x;
  position.y = pos1.y;
  position1.x = pos2.x;
  position1.y = pos2.y;
  DrawLine(position, position1, color);
}

//----------------------------------------------------------------------------------------


void DrawFilledRectangle(positionType pos1, positionType pos2, colorType color)
{
  int i = 0;
  unsigned short temp;

  temp = pos2.y;

  for(i = pos1.y; i <= temp; i++)
  {
    pos1.y = i;
    pos2.y = i;
    DrawLine( pos1, pos2, color );
  }
}

//----------------------------------------------------------------------------------------


void DrawCircle(positionType position, unsigned char radius, colorType color)
{
  signed int f = 1 - radius;
  signed int ddF_x = 1;
  signed int ddF_y = -2 * radius;
  signed int x = 0;
  signed int y = radius;
  positionType positionTemporary = { 0, 0 };

  positionTemporary.x = position.x;
  positionTemporary.y = position.y + radius;
  DrawPixel(positionTemporary, color);

  positionTemporary.x = position.x;
  positionTemporary.y = position.y - radius;
  DrawPixel(positionTemporary, color);

  positionTemporary.x = position.x + radius;
  positionTemporary.y = position.y;
  DrawPixel(positionTemporary, color);

  positionTemporary.x = position.x - radius;
  positionTemporary.y = position.y;
  DrawPixel(positionTemporary, color);

  while ( x < y )
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    positionTemporary.x = position.x + x;
    positionTemporary.y = position.y + y;
    DrawPixel(positionTemporary, color);

    positionTemporary.x = position.x - x;
    positionTemporary.y = position.y + y;
    DrawPixel(positionTemporary, color);

    positionTemporary.x = position.x + x;
    positionTemporary.y = position.y - y;
    DrawPixel(positionTemporary, color);

    positionTemporary.x = position.x - x;
    positionTemporary.y = position.y - y;
    DrawPixel(positionTemporary, color);

    positionTemporary.x = position.x + y;
    positionTemporary.y = position.y + x;
    DrawPixel(positionTemporary, color);

    positionTemporary.x = position.x - y;
    positionTemporary.y = position.y + x;
    DrawPixel(positionTemporary, color);

    positionTemporary.x = position.x + y;
    positionTemporary.y = position.y - x;
    DrawPixel(positionTemporary, color);

    positionTemporary.x = position.x - y;
    positionTemporary.y = position.y - x;
    DrawPixel(positionTemporary, color);

  }
}

//----------------------------------------------------------------------------------------


void DrawFilledCircle(positionType position, int radius, colorType color)
{
    int x = radius;
    int y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;
    positionType positionTemporary;


    while (x >= y)
    {
        for (int i = position.x - x; i <= position.x + x; i++)
        {
            positionTemporary.x = i;
            positionTemporary.y = position.y + y;
            DrawPixel(positionTemporary, color);

            positionTemporary.y = position.y - y;
            DrawPixel(positionTemporary, color);
        }
        for (int i = position.x - y; i <= position.x + y; i++)
        {
            positionTemporary.x = i;
            positionTemporary.y = position.y + x;
            DrawPixel(positionTemporary, color);

            positionTemporary.y = position.y - x;
            DrawPixel(positionTemporary, color);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

//----------------------------------------------------------------------------------------



void LcdPrint(void)
{
    unsigned char i, j, k;
    unsigned char *pFB = lcdFrameBuffer;

    CS_ON;
	DS_ON;
	Senddata(0x40);
	Senddata(0x80);
	DS_OFF;


    for(j = 0; j < (LCD_HEIGHT / 8) ; j++)
    {
        for(k = 0; k < LCD_WIDTH; k++)
        {
            Senddata(*pFB);
            pFB++;
        }
    }

    CS_OFF;
}


//----------------------------------------------------------------------------------------


void SetCurrentFont(const unsigned char *font)
{
    pCurrentFont = font;
    fontParameters = (fontParametersType *)pCurrentFont;
}

//----------------------------------------------------------------------------------------


void GetFontHeight(unsigned char *fontHeight)
{
    if (pCurrentFont == ((void *)0))
    {
        SetCurrentFont(pFontH7xW5);
    }
    *fontHeight = (unsigned char)fontParameters->height;
}

//----------------------------------------------------------------------------------------


void SetFontClearance(unsigned char clearanceInPixel)
{
    fontsClearance = clearanceInPixel;
}

//----------------------------------------------------------------------------------------


void SetSpaceLength(unsigned char spaceLengthInPixels)
{
    spaceLength = spaceLengthInPixels;
}

//----------------------------------------------------------------------------------------


void PutChar(positionType pos, unsigned char ch, colorType color)
{
	const unsigned char *fontpointer = ((void*)0);
    unsigned int i, j;
    unsigned char bitMask;

    emptyLinesCounter = 0;

    ch = ch - 32 - fontParameters->offsetFromASCII;
    fontpointer = pCurrentFont;
    fontpointer += (ch * fontParameters->ucQtyForOneChar + fontParameters->offsetToData);

    for(j = 0; j < fontParameters->width; j++)
    {
        emptyLinesCounter++;

        for(i = 0, bitMask = 0; i < fontParameters->height; i++, bitMask++)
        {
            if((i == 8)||(i == 16)||(i == 24))
            {
                bitMask = 0;
                fontpointer++;
            }
            if(color)
            {
                if(*fontpointer & (1 << bitMask))
                {
                    DrawPixel(pos, black);
                    emptyLinesCounter = 0;
                }
            }
            else
            {
                if(*fontpointer & (1 << bitMask))
                {
                    DrawPixel(pos, white);
                    emptyLinesCounter = 0;
                }
            }
            pos.y++;
        }

        fontpointer++;
        pos.x++;
        pos.y -= fontParameters->height;
    }

    if(emptyLinesCounter == fontParameters->width)
    {
        emptyLinesCounter = 0;
        spaceDetected = 1;
    }
}

//----------------------------------------------------------------------------------------


void PutStr(positionType pos, char *str, colorType color)
{
    if (pCurrentFont == ((void *)0))
    {
        SetCurrentFont(pFontH7xW5);
    }

    while (*str)
	{
	    PutChar(pos, *str, color);
	    if ( ! emptyLinesCounter)
        {
            if(spaceDetected)
            {
                pos.x += spaceLength - fontsClearance;
                spaceDetected = 0;
            }
            else
            {
               pos.x = pos.x + fontParameters->width + fontsClearance;
            }
        }
        else
        {
            pos.x = pos.x + (fontParameters->width - emptyLinesCounter) + fontsClearance;
        }

		str++;
	}
}

//----------------------------------------------------------------------------------------
#define REVERS_FOR_BYTE

void DrawPicture(positionType position, const unsigned char *picture, colorType color)
{
	unsigned int i, j;
	unsigned char bitMask;
	pictureParametersType *pPictureParameters;
	const unsigned char *pointer;

	pPictureParameters = (pictureParametersType *)picture;
    pointer = picture + pPictureParameters->offset;

    for(j = 0; j < pPictureParameters->width; j++)
    {
#ifdef REVERS_FOR_BYTE
        for(i = 0, bitMask = 7; i < pPictureParameters->heigth; i++, bitMask--)
#else
        for(i = 0, bitMask = 0; i < pPictureParameters->heigth; i++, bitMask++)
#endif
        {
            if((i == 8)||(i == 16)||(i == 24))
            {
#ifdef REVERS_FOR_BYTE
                bitMask = 7;
#else
                bitMask = 0;
#endif
                pointer++;
            }
            if(color)
            {
                if((*pointer) & (1 << bitMask))
                {
                    DrawPixel(position, black);
                }
            }
            else
            {
                if((*pointer) & (1 << bitMask))
                {
                    DrawPixel(position, white);
                }
            }
            position.y++;
        }

        pointer++;
        position.x++;
        position.y -= pPictureParameters->heigth;
    }
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------













