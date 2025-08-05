#pragma once

#include <stdio.h>

#include "Panel.h"

//------------------------------------------------------------------------------------------------------------
class aMenuItem
{
public:
	aMenuItem(unsigned short xPos, unsigned short yPos, unsigned short len,	const wchar_t* keyMenu, const wchar_t* nameMenu);

	void draw(CHAR_INFO* screenBuffer, unsigned short screenWidth);

	unsigned short xPosStruct;
	unsigned short yPosStruct;
	unsigned short lenStruct;
	const wchar_t* keyMenuStruct;
	const wchar_t* nameMenuStruct;
};




//------------------------------------------------------------------------------------------------------------
class asCommander
{
public:
	~asCommander();

	bool init();
	bool draw();

private:
	void addNextMenuItem(int& index, int& xPos, int xStep, const wchar_t* keyMenu, const wchar_t* nameMenu);
	void buildMenu();
	HANDLE stdHandle = 0;
	HANDLE screenBufferHandle = 0;
	CHAR_INFO* screenBuffer = 0;
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo{};

	aPanel* leftPanel = 0;
	aPanel* rightPanel = 0;

	aMenuItem* menuItems[10]{};
};
