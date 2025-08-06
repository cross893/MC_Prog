#include "Commander.h"

//------------------------------------------------------------------------------------------------------------
aMenuItem::aMenuItem(unsigned short xPos, unsigned short yPos, unsigned short len, const wchar_t* keyMenu, const wchar_t* nameMenu)
: xPosStruct(xPos), yPosStruct(yPos), lenStruct(len), keyMenuStruct(keyMenu), nameMenuStruct(nameMenu)
{
}




//------------------------------------------------------------------------------------------------------------
void aMenuItem::draw(CHAR_INFO* screenBuffer, unsigned short screenWidth)
{
    int keyStrLen;
    sTextPosition keyPos(xPosStruct, yPosStruct, screenWidth, 0x07);
    keyStrLen = drawBotText(screenBuffer, keyPos, keyMenuStruct);

    sTextPosition namePos(xPosStruct + keyStrLen, yPosStruct, screenWidth, 0xb0);
    drawBotLimitedText(screenBuffer, namePos, nameMenuStruct, lenStruct);
}




//------------------------------------------------------------------------------------------------------------
asCommander::~asCommander()
{
    if (!SetConsoleActiveScreenBuffer(stdOutHandle))
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());

    delete leftPanel;
    delete rightPanel;
    delete screenBuffer;
}




//------------------------------------------------------------------------------------------------------------
bool asCommander::init()
{
    SMALL_RECT srctWriteRect;
    int screenBufferSize;

    // Get a handle to the STDOUT screen buffer to copy from and
    // create a new screen buffer to copy to.
    stdInHandle = GetStdHandle(STD_INPUT_HANDLE);
    stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    screenBufferHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CONSOLE_TEXTMODE_BUFFER, 0);
    if (stdInHandle == INVALID_HANDLE_VALUE || stdOutHandle == INVALID_HANDLE_VALUE || screenBufferHandle == INVALID_HANDLE_VALUE)
    {
        printf("CreateConsoleScreenBuffer failed - (%d)\n", GetLastError());
        return false;
    }

    // Make the new screen buffer the active screen buffer.
    if (!SetConsoleActiveScreenBuffer(screenBufferHandle))
    {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return false;
    }

    if (!GetConsoleScreenBufferInfo(screenBufferHandle, &screenBufferInfo))
    {
        printf("GetConsoleScreenBufferInfo failed - (%d)\n", GetLastError());
        return false;
    }

    screenBufferSize = (int)screenBufferInfo.dwSize.X * (int)screenBufferInfo.dwSize.Y;
    screenBuffer = new CHAR_INFO[screenBufferSize];
    memset(screenBuffer, 0, screenBufferSize * sizeof(CHAR_INFO));

    //// Copy the block from the screen buffer to the temp. buffer.
    //fSuccess = ReadConsoleOutput(stdOutHandle, chiBuffer, coordBufSize, coordBufCoord, &srctReadRect); // screen buffer source rectangle
    //if (!fSuccess)
    //{
    //    printf("ReadConsoleOutput failed - (%d)\n", GetLastError());
    //    return 1;
    //}

    // Set the destination rectangle.
    srctWriteRect.Top = 10;    // top lt: row 10, col 0
    srctWriteRect.Left = 0;
    srctWriteRect.Bottom = 11; // bot. rt: row 11, col 79
    srctWriteRect.Right = 79;

    int halfWidth = screenBufferInfo.dwSize.X / 2;

    leftPanel = new aPanel(0, 0, halfWidth, screenBufferInfo.dwSize.Y - 2, screenBuffer, screenBufferInfo.dwSize.X);
    rightPanel = new aPanel(halfWidth, 0, halfWidth, screenBufferInfo.dwSize.Y - 2, screenBuffer, screenBufferInfo.dwSize.X);

    buildMenu();

    leftPanel->getDirectoryFiles();

    return true;
}




//------------------------------------------------------------------------------------------------------------
void asCommander::run()
{
    unsigned long recordsCount;
    INPUT_RECORD inputRecord[128];
    
    canRun = true;
    needRedraw = true;

    while (canRun)
    {
        if (PeekConsoleInput(stdInHandle, inputRecord, 128, &recordsCount))
        {
            if (ReadConsoleInput(stdInHandle, inputRecord, 1, &recordsCount))
            {
                if (recordsCount != 0)
                {
                    if (inputRecord[0].EventType == KEY_EVENT && inputRecord[0].Event.KeyEvent.bKeyDown)
                    {
                        switch (inputRecord[0].Event.KeyEvent.wVirtualKeyCode)
                        {
                        case VK_F10:
                            canRun = false;
                            break;

                        case VK_UP:
                            leftPanel->moveHighlight(true);
                            needRedraw = true;
                            break;

                        case VK_DOWN:
                            leftPanel->moveHighlight(false);
                            needRedraw = true;
                            break;
                        }
                    }
                }
            }
        }

        if (needRedraw)
        {
            if (!draw())
                return;
            needRedraw = false;
        }

        Sleep(10);
    }
}




//------------------------------------------------------------------------------------------------------------
bool asCommander::draw()
{
    int cycleMenuItemDraw;
    COORD screenBufferPos{};

    leftPanel->draw();
    rightPanel->draw();

    //sTextPosition pos(0, 0, 0, 0);
    //const wchar_t* str = L"1";

    //pos.xPosStruct = 0;
    //pos.yPosStruct = screenBufferInfo.dwSize.Y - 1;
    //pos.screenWidthStruct = screenBufferInfo.dwSize.X;
    //pos.attributesStruct = 0x07;
    //str = L"1";
    //drawBotText(screenBuffer, pos, str);

    //pos.xPosStruct = 1;
    //pos.yPosStruct = screenBufferInfo.dwSize.Y - 1;
    //pos.screenWidthStruct = screenBufferInfo.dwSize.X;
    //pos.attributesStruct = 0xb0;
    //str = L"Help  ";
    //drawBotText(screenBuffer, pos, str);

    for (cycleMenuItemDraw = 0; cycleMenuItemDraw < 10; cycleMenuItemDraw++)
    {
        if (menuItems[cycleMenuItemDraw] != 0)
            menuItems[cycleMenuItemDraw]->draw(screenBuffer, screenBufferInfo.dwSize.X);
    }

    if (!WriteConsoleOutput(screenBufferHandle, screenBuffer, screenBufferInfo.dwSize, screenBufferPos, &screenBufferInfo.srWindow))  // dest. screen buffer rectangle
    {
        printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
        return false;
    }

    return true;
}




//------------------------------------------------------------------------------------------------------------
void asCommander::addNextMenuItem(int& index, int& xPos, int xStep, const wchar_t* keyMenu, const wchar_t* nameMenu)
{
    menuItems[index++] = new aMenuItem(xPos, screenBufferInfo.dwSize.Y - 1, 12, keyMenu, nameMenu);
    xPos += xStep;

    if (index == 2)
        --xPos;
}




//------------------------------------------------------------------------------------------------------------
void asCommander::buildMenu()
{
    int index = 0;
    int xPos = 0;
    int xStep = screenBufferInfo.dwSize.X / 10;

    addNextMenuItem(index, xPos, xStep, L"1", L"Help");
    addNextMenuItem(index, xPos, xStep, L" 2", L"UserMenu");
    addNextMenuItem(index, xPos, xStep, L" 3", L"View");
    addNextMenuItem(index, xPos, xStep, L" 4", L"Edit");
    addNextMenuItem(index, xPos, xStep, L" 5", L"Copy");
    addNextMenuItem(index, xPos, xStep, L" 6", L"RenameMove");
    addNextMenuItem(index, xPos, xStep, L" 7", L"MakeFolder");
    addNextMenuItem(index, xPos, xStep, L" 8", L"Delete");
    addNextMenuItem(index, xPos, xStep, L" 9", L"ConfigMenu");
    addNextMenuItem(index, xPos, xStep, L" 10", L"Quit");
}
