#pragma once

//------------------------------------------------------------------------------------------------------------
struct sPosition
{
    sPosition(unsigned short xPos, unsigned short yPos, unsigned short screenWidth, unsigned short len);

    unsigned short xPosStruct;
    unsigned short yPosStruct;
    unsigned short screenWidthStruct;
    unsigned short lenStruct;
};




//------------------------------------------------------------------------------------------------------------
struct sTextPosition
{
    sTextPosition(unsigned short xPos, unsigned short yPos, unsigned short screenWidth, unsigned short attributes);

    unsigned short xPosStruct;
    unsigned short yPosStruct;
    unsigned short screenWidthStruct;
    unsigned short attributesStruct;
};




//------------------------------------------------------------------------------------------------------------
struct sAreaPosition
{
    sAreaPosition(unsigned short xPos, unsigned short yPos, unsigned short screenWidth, unsigned char width, unsigned char height);

    unsigned short xPosStruct;
    unsigned short yPosStruct;
    unsigned short screenWidthStruct;
    unsigned char widthStruct;
    unsigned char heightStruct;
};




//------------------------------------------------------------------------------------------------------------
class aSymbol
{
public:
    aSymbol(wchar_t mainSymbol, unsigned short attributes, wchar_t startSymbol, wchar_t endSymbol);

    void draw();

    wchar_t mainSymbolStruct;
    unsigned short attributesStruct;
    wchar_t startSymbolStruct;
    wchar_t endSymbolStruct;
};
