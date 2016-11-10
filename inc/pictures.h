#ifndef PICTURES_H_INCLUDED
#define PICTURES_H_INCLUDED


typedef struct
{
    unsigned char heigth;
    unsigned char width;
    unsigned char offset;
}pictureParametersType;

//----------------------------------------------------------------------------------------


const unsigned char *pStartScreen;
const unsigned char *pAboutScreen;
const unsigned char *pSearchGpsScreen;
const unsigned char *pCourseSearchScreen;

const unsigned char *pFullBatteryPicture;
const unsigned char *pMiddleBatteryPicture;
const unsigned char *pLowBatteryPicture;
const unsigned char *pEmptyBatteryPicture;

const unsigned char *pFlag;
#endif /* PICTURES_H_INCLUDED */
