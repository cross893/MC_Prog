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
    int screenBufferSize;
    wchar_t currDir[MAX_PATH];

    GetCurrentDirectory(MAX_PATH, currDir);

    stdInHandle = GetStdHandle(STD_INPUT_HANDLE);
    stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    screenBufferHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CONSOLE_TEXTMODE_BUFFER, 0);
    if (stdInHandle == INVALID_HANDLE_VALUE || stdOutHandle == INVALID_HANDLE_VALUE || screenBufferHandle == INVALID_HANDLE_VALUE)
    {
        printf("CreateConsoleScreenBuffer failed - (%d)\n", GetLastError());
        return false;
    }

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

    int halfWidth = screenBufferInfo.dwSize.X / 2;

    leftPanel = new aPanel(0, 0, halfWidth, screenBufferInfo.dwSize.Y - 2, screenBuffer, screenBufferInfo.dwSize.X);
    rightPanel = new aPanel(halfWidth, 0, halfWidth, screenBufferInfo.dwSize.Y - 2, screenBuffer, screenBufferInfo.dwSize.X);

    buildMenu();

    leftPanel->getDirectoryFiles(std::wstring(currDir));

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
                        case VK_RETURN:
                            leftPanel->onEnter();
                            needRedraw = true;
                            break;

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

    for (cycleMenuItemDraw = 0; cycleMenuItemDraw < 10; cycleMenuItemDraw++)
    {
        if (menuItems[cycleMenuItemDraw] != 0)
            menuItems[cycleMenuItemDraw]->draw(screenBuffer, screenBufferInfo.dwSize.X);
    }

    if (!WriteConsoleOutput(screenBufferHandle, screenBuffer, screenBufferInfo.dwSize, screenBufferPos, &screenBufferInfo.srWindow))
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
