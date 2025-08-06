#pragma once

#include <windows.h>
#include <string>
#include <vector>

#include "Asm_Source_interface.h"
//------------------------------------------------------------------------------------------------------------
class aFileDescriptor
{
public:
    aFileDescriptor(unsigned int attributes, unsigned int fileSizeLow, unsigned int fileSizeHigh, wchar_t* fileName);

    unsigned int attributesStruct;
    unsigned long long fileSizeStruct;
    std::wstring fileNameStruct;

};




//------------------------------------------------------------------------------------------------------------
class aPanel
{
public:
    aPanel(unsigned short xPos, unsigned short yPos, unsigned short width, unsigned short height, CHAR_INFO* screenBuffer, unsigned short screenWidth);

    void draw();
    void getDirectoryFiles();

private:
    void drawPanels();
    void drawFiles();
    void drawOneFile(aFileDescriptor* fileDescriptor, int xOffset, int yOffset, unsigned short bgAttributes);
    void drawHighlight();

    unsigned short xPosStruct;
    unsigned short yPosStruct;
    unsigned short widthStruct;
    unsigned short heightStruct;
    unsigned short screenWidthStruct;
    CHAR_INFO* screenBufferStruct;

    int currFileIndex;
    int xOffsetHighlight;
    int yOffsetHighlight;

    std::vector<aFileDescriptor*> files;
};