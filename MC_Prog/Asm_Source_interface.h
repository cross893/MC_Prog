#pragma once

#include "Common.h"

//------------------------------------------------------------------------------------------------------------
extern "C" int makeSum(int firstValue, int secondValue);
extern "C" void drawLineHorizontal(CHAR_INFO* screenBuffer, sPosition pos, aSymbol symbol);
extern "C" void drawLineVertical(CHAR_INFO* screenBuffer, sPosition pos, aSymbol symbol);
extern "C" void showColors(CHAR_INFO* screenBuffer, sPosition pos, CHAR_INFO symbol);
extern "C" void clearArea(CHAR_INFO* screenBuffer, sAreaPosition areaPos, aSymbol symbol);
extern "C" int drawBotText(CHAR_INFO* screenBuffer, sTextPosition pos, const wchar_t* str);
extern "C" void drawBotLimitedText(CHAR_INFO* screenBuffer, sTextPosition pos, const wchar_t* str, unsigned short limit);
