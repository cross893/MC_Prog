#include "Main.h"
//------------------------------------------------------------------------------------------------------------
int main(void)
{
    asCommander commander;

    if (!commander.init())
        return -1;

    if (!commander.draw())
        return -1;

    return 0;
}
