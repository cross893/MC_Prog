#include "Common.h"

//------------------------------------------------------------------------------------------------------------
sPosition::sPosition(unsigned short xPos, unsigned short yPos, unsigned short screenWidth, unsigned short len)
	: xPosStruct(xPos), yPosStruct(yPos), screenWidthStruct(screenWidth), lenStruct(len)
{
}




//------------------------------------------------------------------------------------------------------------
sTextPosition::sTextPosition(unsigned short xPos, unsigned short yPos, unsigned short screenWidth, unsigned short attributes)
	: xPosStruct(xPos), yPosStruct(yPos), screenWidthStruct(screenWidth), attributesStruct(attributes)
{
}




//------------------------------------------------------------------------------------------------------------
sAreaPosition::sAreaPosition(unsigned short xPos, unsigned short yPos, unsigned short screenWidth, unsigned char width, unsigned char height)
	: xPosStruct(xPos), yPosStruct(yPos), screenWidthStruct(screenWidth), widthStruct(width), heightStruct(height)
{
}




//------------------------------------------------------------------------------------------------------------
aSymbol::aSymbol(wchar_t mainSymbol, unsigned short attributes, wchar_t startSymbol, wchar_t endSymbol)
	: mainSymbolStruct(mainSymbol), attributesStruct(attributes), startSymbolStruct(startSymbol), endSymbolStruct(endSymbol)
{
}
