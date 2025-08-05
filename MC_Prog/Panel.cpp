#include "Panel.h"

//------------------------------------------------------------------------------------------------------------
aFileDescriptor::aFileDescriptor(unsigned int attributes, unsigned int fileSizeLow, unsigned int fileSizeHigh, wchar_t* fileName)
: attributesStruct(attributes), fileNameStruct(fileName)
{
    fileSizeStruct = ((unsigned long long)fileSizeHigh << 32) | (unsigned long long)fileSizeLow;
}




//------------------------------------------------------------------------------------------------------------
aPanel::aPanel(unsigned short xPos, unsigned short yPos, unsigned short width, unsigned short height, CHAR_INFO* screenBuffer, unsigned short screenWidth)
: xPosStruct(xPos), yPosStruct(yPos), widthStruct(width), heightStruct(height), screenBufferStruct(screenBuffer), screenWidthStruct(screenWidth)
{
}




//------------------------------------------------------------------------------------------------------------
void aPanel::draw()
{
    drawPanels();
    drawFiles();
}




//------------------------------------------------------------------------------------------------------------
void aPanel::getDirectoryFiles()
{
    HANDLE searchHandle;
    WIN32_FIND_DATAW findData{};
    searchHandle = FindFirstFileW(L"*.*", &findData);

    while (FindNextFileW(searchHandle, &findData))
    {
        aFileDescriptor* fileDescriptor = new aFileDescriptor(findData.dwFileAttributes, findData.nFileSizeLow, findData.nFileSizeHigh, findData.cFileName);
        files.push_back(fileDescriptor);
    }
}




//------------------------------------------------------------------------------------------------------------
void aPanel::drawPanels()
{
    //┌┴┐│─┼├┤└┬┘ ╔╩╗║═╬╠╣╚╦╝ ╒╓╕╖╘╙╛╜╞╟╡╢╤╥╧╨╪╫
    aSymbol drawLineSymbol(0, 0, 0, 0);
    sPosition drawLinePos{0, 0, 0, 0};
    sAreaPosition areaPosition(0, 0, 0, 0, 0);

    // Общий фон
    drawLineSymbol.mainSymbolStruct = L' ';
    drawLineSymbol.attributesStruct = 0x1b;
    drawLineSymbol.startSymbolStruct = L' ';
    drawLineSymbol.endSymbolStruct = L' ';
    areaPosition.xPosStruct = xPosStruct + 1;
    areaPosition.yPosStruct = yPosStruct + 1;
    areaPosition.screenWidthStruct = screenWidthStruct;
    areaPosition.widthStruct = widthStruct - 2;
    areaPosition.heightStruct = heightStruct - 2;
    clearArea(screenBufferStruct, areaPosition, drawLineSymbol);

    // 1.1. Верхняя линия
    drawLineSymbol.mainSymbolStruct = L'═';
    drawLineSymbol.attributesStruct = 0x1b;
    drawLineSymbol.startSymbolStruct = L'╔';
    drawLineSymbol.endSymbolStruct = L'╗';
    drawLinePos.xPosStruct = xPosStruct;
    drawLinePos.yPosStruct = yPosStruct;
    drawLinePos.screenWidthStruct = screenWidthStruct;
    drawLinePos.lenStruct = widthStruct - 2;
    drawLineHorizontal(screenBufferStruct, drawLinePos, drawLineSymbol);

    // 1.3. Нижняя линия
    drawLineSymbol.mainSymbolStruct = L'═';
    drawLineSymbol.attributesStruct = 0x1b;
    drawLineSymbol.startSymbolStruct = L'╚';
    drawLineSymbol.endSymbolStruct = L'╝';
    drawLinePos.xPosStruct = xPosStruct;
    drawLinePos.yPosStruct = yPosStruct + heightStruct - 1;
    drawLinePos.screenWidthStruct = screenWidthStruct;
    drawLinePos.lenStruct = widthStruct - 2;
    drawLineHorizontal(screenBufferStruct, drawLinePos, drawLineSymbol);

    // 2.1. Левая линия
    drawLineSymbol.mainSymbolStruct = L'║';
    drawLineSymbol.attributesStruct = 0x1b;
    drawLineSymbol.startSymbolStruct = L'╔';
    drawLineSymbol.endSymbolStruct = L'╚';
    drawLinePos.xPosStruct = xPosStruct;
    drawLinePos.yPosStruct = yPosStruct;
    drawLinePos.screenWidthStruct = screenWidthStruct;
    drawLinePos.lenStruct = heightStruct - 2;
    drawLineVertical(screenBufferStruct, drawLinePos, drawLineSymbol);

    // 2.3. Правая линия
    drawLineSymbol.mainSymbolStruct = L'║';
    drawLineSymbol.attributesStruct = 0x1b;
    drawLineSymbol.startSymbolStruct = L'╗';
    drawLineSymbol.endSymbolStruct = L'╝';
    drawLinePos.xPosStruct = xPosStruct + widthStruct - 1;
    drawLinePos.yPosStruct = yPosStruct;
    drawLinePos.screenWidthStruct = screenWidthStruct;
    drawLinePos.lenStruct = heightStruct - 2;
    drawLineVertical(screenBufferStruct, drawLinePos, drawLineSymbol);

    // 1.2. Средняя горизонтальная линия
    drawLineSymbol.mainSymbolStruct = L'─';
    drawLineSymbol.attributesStruct = 0x1b;
    drawLineSymbol.startSymbolStruct = L'╟';
    drawLineSymbol.endSymbolStruct = L'╢';
    drawLinePos.xPosStruct = xPosStruct;
    drawLinePos.yPosStruct = yPosStruct + heightStruct - 3;
    drawLinePos.screenWidthStruct = screenWidthStruct;
    drawLinePos.lenStruct = widthStruct - 2;
    drawLineHorizontal(screenBufferStruct, drawLinePos, drawLineSymbol);

    // 2.2. Средняя вертикальная линия
    drawLineSymbol.mainSymbolStruct = L'║';
    drawLineSymbol.attributesStruct = 0x1b;
    drawLineSymbol.startSymbolStruct = L'╦';
    drawLineSymbol.endSymbolStruct = L'╨';
    drawLinePos.xPosStruct = xPosStruct + widthStruct / 2;
    drawLinePos.yPosStruct = yPosStruct;
    drawLinePos.screenWidthStruct = screenWidthStruct;
    drawLinePos.lenStruct = heightStruct - 4;
    drawLineVertical(screenBufferStruct, drawLinePos, drawLineSymbol);

    //drawLineVertical(screenBufferStruct, drawLinePos, symbol);
    //showColors(screenBufferStruct, drawLinePos, symbol);
}




//------------------------------------------------------------------------------------------------------------
void aPanel::drawFiles()
{
    unsigned short attributes;
    int xOffset = 0;
    int yOffset = 0;

    for (auto* file : files)
    {
        if (file->attributesStruct & FILE_ATTRIBUTE_DIRECTORY)
            attributes = 0x1f;
        else
            attributes = 0x1b;

        sTextPosition pos(xPosStruct + xOffset + 1, yPosStruct + yOffset + 2, screenWidthStruct, attributes);
        drawBotText(screenBufferStruct, pos, file->fileNameStruct.c_str() );

        ++yOffset;

        if (yOffset >= heightStruct - 5)
        {
            if (xOffset == 0)
            {
                xOffset += widthStruct / 2;
                yOffset = 0;
            }
            else
                break;
        }
    }
}
