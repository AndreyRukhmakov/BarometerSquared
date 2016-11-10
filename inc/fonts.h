#ifndef FONTS_H_INCLUDED
#define FONTS_H_INCLUDED


typedef struct
{
    unsigned char height;
    unsigned char width;
    unsigned char offsetFromASCII;
    unsigned char offsetToData;
    unsigned char ucQtyForOneChar;
}fontParametersType;

//----------------------------------------------------------------------------------------

const unsigned char *pFontH5xW5;
const unsigned char *pFontH7xW5;
const unsigned char *pFontShortNarrowH5xW5;
const unsigned char *pFontShortH10xW5;
const unsigned char *pFontShortH12xW6;
const unsigned char *pFontShortH15xW6;
const unsigned char *pFontShortH15xW9;

#endif /* FONTS_H_INCLUDED */
