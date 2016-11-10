#include "barometer.h"
#include "Nokia1110Lcd.h"
#include "string.h"

//#define BLINK_LAST_DATA
#define CUBE_GRAPHICS_TYPE
#define SEGMENT_LINES_QTY               10
#define SEGMENTS_QTY                    12
#define ONE_SEGMENT_LENGTH              4
#define ONE_SEGMENT_THICKNESS           2
#define VALUE_OF_DIVISION               10 // 10Hpa per one segment

typedef enum
{
    down = 0,
    up = 1
}pressureDir;


static unsigned char barometerMassive[SEGMENT_LINES_QTY][SEGMENTS_QTY]; // 10-lines, 12x2-hours
positionType currentPosition = { ( SEGMENT_LINES_QTY - 1 ), 0 }, blinkingPosition;
unsigned short oldPressureValue;
unsigned char currentPressureDirection = 1;


void OffsetAllDataInTableUp(unsigned short offset);
void OffsetAllDataInTableDown(unsigned short offset);
void OffsetAllDataInTableLeft(void);

//------------------------------------------------------------------------------


void BarometerReset(void)
{
    memset ( barometerMassive, 0, sizeof(barometerMassive) );
    currentPosition.x = 0;
    currentPosition.y = ( SEGMENT_LINES_QTY - 1 );
    blinkingPosition = currentPosition;
    oldPressureValue = 0;
}

//------------------------------------------------------------------------------


void BarometerFillData(unsigned long currentPressureValue)
{
    signed char offsetIntervalInTable = 0;


    currentPressureValue /= 100; //Pressure in Hp

    if( currentPressureValue > oldPressureValue )
    {
        currentPressureDirection = up;
    }
    else if( currentPressureValue < oldPressureValue )
    {
        currentPressureDirection = down;
    }

    if( oldPressureValue == 0 )
    {
        barometerMassive[currentPosition.y][currentPosition.x] = 1;
        blinkingPosition = currentPosition;
        currentPosition.x++;
        oldPressureValue = currentPressureValue;
        return;
    }

    offsetIntervalInTable = ( ( (signed short)currentPressureValue - (signed short)oldPressureValue ) / VALUE_OF_DIVISION );
    oldPressureValue = currentPressureValue;

    if( offsetIntervalInTable < 0 )
    {
        offsetIntervalInTable = ( 0 - offsetIntervalInTable );
    }

    if( currentPosition.x > ( SEGMENTS_QTY - 1 ) ) // check for table border
    {
        currentPosition.x = ( SEGMENTS_QTY - 1 ); // will be always as last element
        OffsetAllDataInTableLeft();
    }

    if( offsetIntervalInTable == 0 )
    {
        barometerMassive[currentPosition.y][currentPosition.x] = 1;
        blinkingPosition = currentPosition;
        currentPosition.x++;
        return;
    }

    if( offsetIntervalInTable )
    {
        if( currentPressureDirection == up )
        {
            if( currentPosition.y < offsetIntervalInTable )
            {
                OffsetAllDataInTableDown( offsetIntervalInTable - currentPosition.y );
                currentPosition.y = 0;
            }
            else
            {
                currentPosition.y -= offsetIntervalInTable;
            }

            barometerMassive[currentPosition.y][currentPosition.x] = 1;
            blinkingPosition = currentPosition;
            currentPosition.x++;
            return;
        }
        else
        {
            if( ( SEGMENT_LINES_QTY - 1 ) < currentPosition.y + offsetIntervalInTable  )
            {

                OffsetAllDataInTableUp( ( currentPosition.y + offsetIntervalInTable ) - ( SEGMENT_LINES_QTY - 1 ));
                currentPosition.y = ( SEGMENT_LINES_QTY - 1 );
            }
            else
            {
                currentPosition.y += offsetIntervalInTable;
            }

            barometerMassive[currentPosition.y][currentPosition.x] = 1;
            blinkingPosition = currentPosition;
            currentPosition.x++;
        }
    }
}

//------------------------------------------------------------------------------


void DrawBarometerField( unsigned char x,
                         unsigned char y,
                         colorType backgroundColor,
                         colorType dataColor,
                         colorType blinkColor )
{
    positionType position, position2, segmentPosition, thicknessPosition1, thicknessPosition2;
    unsigned char i, j, k;
    static unsigned char blinkingFlag = 0;


    // Draw barometer field
    position.x = x;
    position.y = y;
    position2.x = x + ( SEGMENTS_QTY * ONE_SEGMENT_LENGTH ) + 1; // 1- is offset from border for segments
    position2.y = y + ( SEGMENT_LINES_QTY * ONE_SEGMENT_THICKNESS ) + 1; // 1- is offset from border for segments

    DrawFilledRectangle(position, position2, backgroundColor);

    // Draw segments
    segmentPosition = position;
    // Offset from border for segments
    segmentPosition.x += 1;
    segmentPosition.y += 1;

    for( i = 0; i < SEGMENT_LINES_QTY; i++ )
    {
        for( j = 0; j < SEGMENTS_QTY; j++ )
        {
            if( barometerMassive[i][j] )
            {
                // Draw segment according to thickness
                thicknessPosition1 = segmentPosition;
                thicknessPosition2 = thicknessPosition1;
                thicknessPosition2.x = segmentPosition.x + ( ONE_SEGMENT_LENGTH - 1 );
#ifdef BLINK_LAST_DATA
                    if( ( i == blinkingPosition.y ) && ( j == blinkingPosition.x ) )
                    {
                        if( blinkingFlag )
                        {
#ifdef CUBE_GRAPHICS_TYPE
                            for(k = i; k < ( ( SEGMENT_LINES_QTY * ONE_SEGMENT_THICKNESS ) - i ) ; k++)
#else
                            for(k = 0; k < ONE_SEGMENT_THICKNESS; k++)
#endif  )
                            {
                                DrawLine(thicknessPosition1, thicknessPosition2, dataColor);

                                thicknessPosition1.y++;
                                thicknessPosition2.y++;
                            }
                            blinkingFlag = 0;
                        }
                        else
                        {
#ifdef CUBE_GRAPHICS_TYPE
                            for(k = i; k < ( ( SEGMENT_LINES_QTY * ONE_SEGMENT_THICKNESS ) - i ) ; k++)
#else
                            for(k = 0; k < ONE_SEGMENT_THICKNESS; k++)
#endif
                            {
                                DrawLine(thicknessPosition1, thicknessPosition2, blinkColor);

                                thicknessPosition1.y++;
                                thicknessPosition2.y++;
                            }
                            blinkingFlag = 1;
                        }
                    }
                    else
                    {
#ifdef CUBE_GRAPHICS_TYPE
                        for(k = i; k < ( ( SEGMENT_LINES_QTY * ONE_SEGMENT_THICKNESS ) - i ) ; k++)
#else
                        for(k = 0; k < ONE_SEGMENT_THICKNESS; k++)
#endif
                        {
                            DrawLine(thicknessPosition1, thicknessPosition2, dataColor);

                            thicknessPosition1.y++;
                            thicknessPosition2.y++;
                        }
                    }
#else // BLINK_LAST_DATA

#ifdef CUBE_GRAPHICS_TYPE
                for(k = i; k < ( ( SEGMENT_LINES_QTY * ONE_SEGMENT_THICKNESS ) - i ) ; k++)
#else
                for(k = 0; k < ONE_SEGMENT_THICKNESS; k++)
#endif
                {
                    DrawLine(thicknessPosition1, thicknessPosition2, dataColor);

                    thicknessPosition1.y++;
                    thicknessPosition2.y++;
                }
#endif
            }
            segmentPosition.x += ONE_SEGMENT_LENGTH;
        }
        segmentPosition.y += ONE_SEGMENT_THICKNESS;
        segmentPosition.x = position.x + 1;
    }
}

//------------------------------------------------------------------------------


void OffsetAllDataInTableUp( unsigned short offset )
{
    unsigned char i;


    while(offset)
    {
        for ( i = 0 ; i < ( SEGMENT_LINES_QTY - 1 ); i++ )
        {
            memcpy ( &barometerMassive[i][0], &barometerMassive[i + 1][0], SEGMENTS_QTY );
        }

        memset( &barometerMassive[SEGMENT_LINES_QTY - 1][0], 0, SEGMENTS_QTY);

        offset--;
    }
}

//------------------------------------------------------------------------------


void OffsetAllDataInTableDown( unsigned short offset )
{
    unsigned char i;


    while(offset)
    {
        for ( i = ( SEGMENT_LINES_QTY - 1 ) ; i != 0; i-- )
        {
            memcpy ( &barometerMassive[i][0], &barometerMassive[i - 1][0], SEGMENTS_QTY );
        }

        memset( &barometerMassive[0][0], 0, SEGMENTS_QTY);

        offset--;
    }
}

//------------------------------------------------------------------------------


void OffsetAllDataInTableLeft(void)
{
    unsigned char i, j;


    for ( i = 0 ; i < ( SEGMENTS_QTY - 1 ); i++ )
    {
        for( j = 0; j < SEGMENT_LINES_QTY; j++ )
        {
            memcpy ( &barometerMassive[j][i], &barometerMassive[j][i + 1], 1 );
        }
    }

    for( i = 0; i < SEGMENT_LINES_QTY; i++ )
    {
        memset( &barometerMassive[i][( SEGMENTS_QTY - 1 )], 0, 1);
    }

}

//------------------------------------------------------------------------------


unsigned char BarometerGetPressureDirection(void)
{
    return currentPressureDirection;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
